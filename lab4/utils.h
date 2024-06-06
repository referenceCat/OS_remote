#include <iostream>
#include <windows.h>
#include <ctime>
#include <windows.h>
#include <string>

const int pageSize = 4096,
            numberOfPages = 14,
            numberOfReaders = 7,
            numberOfWriters = 7;

const std::string mutexName = "IOMutex",
                mapName = "mapped_file",
                fileName = "file";

// Function to write log messages
void LogWrite(const std::string& message) {
    WriteFile(
            GetStdHandle(STD_OUTPUT_HANDLE),
            message.c_str(),
            message.length(),
            nullptr,
            nullptr);
}