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

int main() {
    srand(time(nullptr));

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

    // Open handles to standard output and semaphores
    HANDLE stdOutHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    for (int i = 0; i < numberOfPages; i++) {
        writeSemaphores[i] = OpenSemaphoreWithErrorCheck(SEMAPHORE_MODIFY_STATE | SYNCHRONIZE,
                                                         FALSE,
                                                         std::to_string(i));

        readSemaphores[i] = OpenSemaphoreWithErrorCheck(SEMAPHORE_MODIFY_STATE | SYNCHRONIZE,
                                                        FALSE,
                                                        std::to_string(i + numberOfPages));
    }

    // Perform read operations
    for (int i = 0; i < 3; i++) {
        LogWrite("Wait | Semaphore | " + std::to_string(GetTickCount()) + "\n");

        // Wait for any available page
        DWORD page = WaitForMultipleObjects(
                numberOfPages,
                readSemaphores,
                FALSE,
                INFINITE);
        LogWrite("Take | Semaphore | " + std::to_string(GetTickCount()) + "\n");

        // Wait for the I/O mutex
        WaitForSingleObject(
                ioMutex,
                INFINITE);
        LogWrite("Take | Mutex | " + std::to_string(GetTickCount()) + "\n");

        Sleep(500 + rand() % 1000);
        LogWrite("Read | Page: " + std::to_string(page) + " | " + std::to_string(GetTickCount()) + "\n");

        // Release the I/O mutex
        ReleaseMutex(ioMutex);
        LogWrite("Free | Mutex | " + std::to_string(GetTickCount()) + "\n");

        // Release the write semaphore for the read page
        ReleaseSemaphore(writeSemaphores[page], 1, nullptr);
        LogWrite("Free | Semaphore | " + std::to_string(GetTickCount()) + "\n\n");
    }

    // Close handles
    for (int i = 0; i < numberOfPages; i++) {
        CloseHandle(writeSemaphores[i]);
        CloseHandle(readSemaphores[i]);
    }
    CloseHandle(ioMutex);
    CloseHandle(mappedFile);

    return 0;
}