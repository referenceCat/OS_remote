//
// Created by referenceCat on 06.06.2024.
//

// Writer.cpp

#include <fstream>
#include <iostream>
#include <string>
#include <windows.h>

const int PAGE_SIZE = 4096;       // Размер страницы в байтах (обычно 4KB)
const int STUDENT_NUMBER_SUM = 14;//  Cумма цифр студака
const int NUM_PAGES = STUDENT_NUMBER_SUM;

const char *FILE_MAPPING_NAME = "SharedMemoryFile.txt";
const char *SEMAPHORE_NAME = "BufferSemaphore";


void LogEvent(const std::string &filename, const std::string &event) {
    std::ofstream logFile(filename, std::ios::app);
    if (logFile.is_open()) {
        DWORD time = timeGetTime();
        logFile << time << ": " << event << std::endl;
        logFile.close();
    }
}


std::string GetLastErrorAsString() {
    //Get the error message ID, if any.
    DWORD errorMessageID = ::GetLastError();
    if (errorMessageID == 0) {
        return std::string();//No error message has been recorded
    }

    LPSTR messageBuffer = nullptr;

    //Ask Win32 to give us the string version of that message ID.
    //The parameters we pass in, tell Win32 to create the buffer that holds the message for us (because we don't yet know how long the message string will be).
    size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                                 NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR) &messageBuffer, 0, NULL);

    //Copy the error message into a std::string.
    std::string message(messageBuffer, size);

    //Free the Win32's string's buffer.
    LocalFree(messageBuffer);

    return message;
}


void Writer(int writerId) {
    std::string logFilename = "Writer" + std::to_string(writerId) + ".log";

    // Открытие семафора и мьютекса
    HANDLE hSemaphore = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, SEMAPHORE_NAME);

    // Открытие проецируемого файла
    HANDLE hMapFile = CreateFileMapping(INVALID_HANDLE_VALUE, NULL,  PAGE_READWRITE, FALSE, NUM_PAGES * PAGE_SIZE, FILE_MAPPING_NAME);
    if (hMapFile == NULL) {
        std::cout << "Could not create map view: "
                  << GetLastErrorAsString() << std::endl;


        CloseHandle(hMapFile);

        std::cin.get();
        exit(1);
    }
    LPVOID pBuf = MapViewOfFile(hMapFile, FILE_MAP_WRITE, 0, 0, NUM_PAGES * PAGE_SIZE);
    if (pBuf == NULL) {
        std::cout << "Could not map view of file: "
                  << GetLastErrorAsString() << std::endl;


        CloseHandle(hMapFile);

        std::cin.get();
        exit(1);
    }

    while (true) {
        LogEvent(logFilename, "Waiting");

        // Ожидание семафора
        WaitForSingleObject(hSemaphore, INFINITE);

        LogEvent(logFilename, "Writing");

        // Запись данных (пример)
        for (int i = 0; i < NUM_PAGES; i++) {
            char* page = (char*)pBuf + i * PAGE_SIZE;
            // Пример записи: установка первого байта страницы в значение идентификатора писателя
            page[0] = 'A' + writerId;
            LogEvent(logFilename, "Wrote page " + std::to_string(i));
        }



        // Освобождение семафора
        ReleaseSemaphore(hSemaphore, 1, NULL);
        LogEvent(logFilename, "Released");

        // Задержка
        Sleep(500 + rand() % 1001); // От 0.5 до 1.5 сек.
    }

    UnmapViewOfFile(pBuf);
    CloseHandle(hMapFile);
    CloseHandle(hSemaphore);
}

int main(int argc, char* argv[]) {
    if (argc!= 2) {
        std::cout << "Usage: " << argv[0] << " <writerId>" << std::endl;
        return 1;
    }
    setlocale(LC_ALL, "Russian");
    srand(GetTickCount());
    int writerId = std::atoi(argv[1]); // Уникальный идентификатор писателя
    Writer(writerId);
    return 0;
}