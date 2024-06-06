#include "utils.h"

int main(int argc, char* argv[]) {
    srand(time(nullptr));
    int id = strtol(argv[1], nullptr, 10);

    // Open handles to semaphores and mutex
    HANDLE writeSemaphores[numberOfPages], readSemaphores[numberOfPages];
    HANDLE ioMutex = OpenMutex(
            MUTEX_MODIFY_STATE | SYNCHRONIZE,
            false,
            mutexName.c_str());
    HANDLE mappedFile = OpenFileMapping(
            GENERIC_READ,
            false,
            mapName.c_str());
    LPVOID pointerToMappedContent = MapViewOfFile(mappedFile, FILE_MAP_WRITE, 0, 0, pageSize * numberOfPages);

    for (int i = 0; i < numberOfPages; i++) {
        writeSemaphores[i] = OpenSemaphore(SEMAPHORE_MODIFY_STATE | SYNCHRONIZE,
                                                         FALSE,
                                                         std::to_string(i).c_str());

        readSemaphores[i] = OpenSemaphore(SEMAPHORE_MODIFY_STATE | SYNCHRONIZE,
                                                        FALSE,
                                                        std::to_string(i + numberOfPages).c_str());
    }

    for (int i = 0; i < 3; i++) {
        logProcessEvent(id, false, WAITING);

        DWORD pageNumber = WaitForMultipleObjects(
                numberOfPages,
                writeSemaphores,
                FALSE,
                INFINITE);

        WaitForSingleObject(
                ioMutex,
                INFINITE);

        logProcessEvent(id, false, RW_OPERATION, pageNumber);

        VirtualLock((char*)pointerToMappedContent + pageSize * pageNumber, pageSize);
        Sleep(rwDelay_ms + rand() % rwDelayDiv_ms);
        VirtualLock((char*)pointerToMappedContent + pageSize * pageNumber, pageSize);

        ReleaseMutex(ioMutex);

        ReleaseSemaphore(readSemaphores[pageNumber], 1, nullptr);
        logProcessEvent(id, false, REALISED);
    }

    for (int i = 0; i < numberOfPages; i++) {
        CloseHandle(writeSemaphores[i]);
        CloseHandle(readSemaphores[i]);
    }
    CloseHandle(ioMutex);
    CloseHandle(mappedFile);

    return 0;
}