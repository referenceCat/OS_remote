//
// Created by referenceCat on 06.06.2024.
//

// Server.cpp

#include <windows.h>
#include <iostream>
#include <string>

#define PIPE_NAME R"(\\.\pipe\MyPipe)"


void StartServer() {

}

int main() {
    HANDLE hPipe;
    HANDLE hEvent;
    OVERLAPPED overlapped = {0};
    DWORD bytesWritten;
    std::string message;

    bool connected = false;


    int menu_option;
    do {
        std::cout << "Server Menu:\n";
        std::cout << "1. start server\n";
        std::cout << "2. send message\n";
        std::cout << "3. exit\n";
        std::cin >> menu_option;
        std::cin.ignore();  // Ignore the newline character left in the buffer
        switch (menu_option) {
            case 1:
                hPipe = CreateNamedPipe(
                        PIPE_NAME,
                        PIPE_ACCESS_OUTBOUND | FILE_FLAG_OVERLAPPED,
                        PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
                        1,
                        1024,
                        1024,
                        0,
                        NULL
                );

                hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

                overlapped.hEvent = hEvent;

                ConnectNamedPipe(hPipe, &overlapped);
                std::cout << "Pipe connected, now waiting for client" << std::endl;
                WaitForSingleObject(hEvent, INFINITE);
                std::cout << "Client connected." << std::endl;
                connected = true;
                break;

            case 2:
                if (!connected) {
                    std::cout << "Server isn't connected to pipe yet" << std::endl;
                    break;
                }
                std::cout << "Enter message: ";

                std::getline(std::cin, message);
                ResetEvent(hEvent);
                overlapped.hEvent = hEvent;
                WriteFile(hPipe, message.c_str(), message.size() + 1, &bytesWritten, &overlapped);
                WaitForSingleObject(hEvent, INFINITE);
                break;
            case 3:
                DisconnectNamedPipe(hPipe);
                CloseHandle(hPipe);
                CloseHandle(hEvent);
                break;
            default:
                std::cout << "Invalid option. Try again." << std::endl;
                break;
        }
    } while (menu_option != 3);

    return 0;
}