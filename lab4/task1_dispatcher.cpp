#include "utils.h"


HANDLE CreateNewProcess(const std::string&, const std::string&);

int main() {
    HANDLE writeSemaphores[numberOfPages], readSemaphores[numberOfPages];
    HANDLE ioMutex = CreateMutex(
            nullptr,
            false,
            mutexName.c_str());

    HANDLE fileHandle = CreateFile(
            fileName.c_str(),
            GENERIC_WRITE | GENERIC_READ,
            0, nullptr,
            CREATE_ALWAYS,
            FILE_ATTRIBUTE_NORMAL,
            nullptr);

    HANDLE mapFile = CreateFileMapping(
            fileHandle,
            nullptr,
            PAGE_READWRITE,
            0,
            pageSize * numberOfPages,
            mapName.c_str());

    HANDLE readers[numberOfReaders], writers[numberOfWriters];


    // Map the file to the memory
    LPVOID fileView = MapViewOfFile(
            mapFile,
            FILE_MAP_ALL_ACCESS,
            0,
            0,
            pageSize * numberOfPages);

    for (int i = 0; i < numberOfPages; i++) {
        writeSemaphores[i] = CreateSemaphore(
                nullptr,
                1,
                1,
                std::to_string(i).c_str());

        readSemaphores[i] = CreateSemaphore(
                nullptr,
                0,
                1,
                std::to_string(i + numberOfPages).c_str());
    }

    // Create writer and reader processes
    for (int i = 0; i < numberOfWriters; i++) {
        std::string logName = R"(C:\Users\referenceCat\CLionProjects\OS\lab4\task1_logs\writeLog_)" + std::to_string(i) + ".txt";
        writers[i] = CreateNewProcess("lab4_task1_writer.exe", logName);
    }

    for (int i = 0; i < numberOfReaders; i++) {
        std::string logName = R"(C:\Users\referenceCat\CLionProjects\OS\lab4\task1_logs\readLog_)" + std::to_string(i) + ".txt";
        readers[i] = CreateNewProcess("lab4_task1_reader.exe", logName);
    }

     WaitForMultipleObjects(
            numberOfWriters,
            writers,
            true,
            INFINITE);

    WaitForMultipleObjects(
            numberOfReaders,
            readers,
            true,
            INFINITE);

    UnmapViewOfFile(fileView);
    CloseHandle(mapFile);
    CloseHandle(fileHandle);
    for (int i = 0; i < numberOfPages; i++) {
        CloseHandle(writeSemaphores[i]);
        CloseHandle(readSemaphores[i]);
    }
    CloseHandle(ioMutex);
    return 0;
}

HANDLE CreateNewProcess(const std::string& exePath, const std::string& logName) {
    // Create the process with specified path to the executable file and arguments
    STARTUPINFOA si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    char buffer[512];
    strcpy_s(buffer, exePath.c_str());
    std::string commandLine = exePath + " " + logName;
    if (!CreateProcessA(
            exePath.c_str(),
            const_cast<char*>(commandLine.c_str()),
            nullptr,
            nullptr,
            false,
            0,
            nullptr,
            nullptr,
            &si,
            &pi)) {
        std::cerr << "Error creating process: " << GetLastError() << std::endl;
        ExitProcess(1);
    }
    CloseHandle(pi.hThread);
    return pi.hProcess;
}