//
// Created by referenceCat on 06.06.2024.
//

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


void Reader(int readerId) {
    std::string logFilename = "Reader" + std::to_string(readerId) + ".log";

    // Открытие семафора и мьютекса
    HANDLE hSemaphore = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, SEMAPHORE_NAME);

    // Открытие проецируемого файла
    HANDLE hMapFile = OpenFileMapping(FILE_MAP_READ, FALSE, FILE_MAPPING_NAME);
    LPVOID pBuf = MapViewOfFile(hMapFile, FILE_MAP_READ, 0, 0, NUM_PAGES * PAGE_SIZE);

    while (true) {
        LogEvent(logFilename, "Waiting");

        // Ожидание семафора
        WaitForSingleObject(hSemaphore, INFINITE);

        LogEvent(logFilename, "Reading");

        // Чтение данных (пример)
        for (int i = 0; i < NUM_PAGES; i++) {
            char* page = (char*)pBuf + i * PAGE_SIZE;
            // Пример чтения: вывод первого байта страницы
            std::cout << "Reader " << readerId << " read page " << i << ": " << page[0] << std::endl;
            LogEvent(logFilename, "Read page " + std::to_string(i));
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
        std::cout << "Usage: " << argv[0] << " <readerId>" << std::endl;
        return 1;
    }
    srand(GetTickCount());
    int readerId = std::atoi(argv[1]); // Уникальный идентификатор читателя
    Reader(readerId);
    return 0;
}