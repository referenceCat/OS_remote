#include "utils.h"


// Function to open a semaphore with error handling
HANDLE OpenSemaphoreWithErrorCheck(DWORD accessMode, BOOL inheritHandle, const std::string& semaphoreName) {
    HANDLE semaphore = OpenSemaphore(accessMode, inheritHandle, semaphoreName.c_str());
    if (semaphore == nullptr) {
        std::cerr << "Error opening semaphore: " << semaphoreName << std::endl;
        ExitProcess(1);
    }
    return semaphore;
}

int main(int argc, char* argv[]) {
    srand(time(nullptr));
    int id = strtol(argv[1], nullptr, 10);

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
        writeSemaphores[i] = OpenSemaphoreWithErrorCheck(SEMAPHORE_MODIFY_STATE | SYNCHRONIZE,
                                                         FALSE,
                                                         std::to_string(i));

        readSemaphores[i] = OpenSemaphoreWithErrorCheck(SEMAPHORE_MODIFY_STATE | SYNCHRONIZE,
                                                        FALSE,
                                                        std::to_string(i + numberOfPages));
    }

    for (int i = 0; i < 3; i++) {
        logProcessEvent(id, true, WAITING);
        DWORD pageNumber = WaitForMultipleObjects(
                numberOfPages,
                readSemaphores,
                FALSE,
                INFINITE);
        WaitForSingleObject(
                ioMutex,
                INFINITE);

        logProcessEvent(id, true, RW_OPERATION, pageNumber);
        VirtualLock((char*)pointerToMappedContent + pageSize * pageNumber, pageSize);
        Sleep(rwDelay_ms + rand() % rwDelayDiv_ms);
        VirtualLock((char*)pointerToMappedContent + pageSize * pageNumber, pageSize);

        ReleaseMutex(ioMutex);
        ReleaseSemaphore(writeSemaphores[pageNumber], 1, nullptr);

        logProcessEvent(id, true, REALISED);

    }

    for (int i = 0; i < numberOfPages; i++) {
        CloseHandle(writeSemaphores[i]);
        CloseHandle(readSemaphores[i]);
    }
    CloseHandle(ioMutex);
    CloseHandle(mappedFile);

    return 0;
}