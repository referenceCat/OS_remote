//
// Created by referenceCat on 06.06.2024.
//

#include <windows.h>
#include <iostream>
#include <string>

#define PIPE_NAME R"(\\.\pipe\MyPipe)"

VOID CALLBACK
ReadCompletionRoutine(DWORD
                      dwErrorCode,
                      DWORD dwNumberOfBytesTransfered, LPOVERLAPPED
                      lpOverlapped) {
    if (dwErrorCode == 0) {
        std::cout << "Received message: " << (char*)lpOverlapped->hEvent <<
                  std::endl;
    } else {
        std::cerr << "Read error: " << dwErrorCode <<
                  std::endl;
    }
}

void ConnectToServer() {
    HANDLE hPipe;
    OVERLAPPED overlapped = {0};
    char buffer[1024];

    hPipe = CreateFile(
            PIPE_NAME,
            GENERIC_READ,
            0,
            nullptr,
            OPEN_EXISTING,
            FILE_FLAG_OVERLAPPED,
            nullptr
    );

    if (hPipe == INVALID_HANDLE_VALUE) return;

    std::cout << "Connected to server." << std::endl;

    while (true) {
        memset(buffer, 0, sizeof(buffer));
        overlapped.hEvent = buffer;

        if (!ReadFileEx(hPipe, buffer, sizeof(buffer), &overlapped, ReadCompletionRoutine) && GetLastError() != ERROR_IO_PENDING) return;
        SleepEx(INFINITE, TRUE);
    }
}

int main() {
    while (true) {
        ConnectToServer();
        Sleep(1000);
        std::cout << "Trying to connect to server...\n";
    }
}

