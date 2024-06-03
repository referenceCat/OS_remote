//
// Created by referenceCat on 03.06.2024.
//

#include <iostream>
#include <windows.h>
#include <iomanip>
#include <list>
#include <chrono>
#include <cmath>

HANDLE iterationMutex;
HANDLE resultAccessMutex;
const int blockSize = 10 * 230704;
const int N = 100000000;
long double result = 0;
int currentBlock;


DWORD WINAPI threadJob(LPVOID threadData) {
    while (true) {
        WaitForSingleObject(iterationMutex, INFINITE);
        int from = currentBlock * blockSize;
        if (from >= N) return 0;
        int to = std::min((currentBlock + 1) * blockSize, N);
        currentBlock++;
        ReleaseMutex(iterationMutex);
        long double blockResult = 0;
        for (int i = from; i < to; i++) {
            long double x = i + 0.5;
            x /= N;
            x *= x;
            x = 1 / (x + 1);
            blockResult += x;
        }
        WaitForSingleObject(resultAccessMutex, INFINITE);
        result += blockResult;
        ReleaseMutex(resultAccessMutex);
    }
}

long double calculatePi(int threadsN) {
    result = 0;
    currentBlock = 0;
    auto *threads = new HANDLE[threadsN];
    iterationMutex = CreateMutex(nullptr, FALSE, nullptr);
    resultAccessMutex = CreateMutex(nullptr, FALSE, nullptr);
    for (int i = 0; i < threadsN; i++) {
        threads[i] = CreateThread(nullptr, 0, threadJob, nullptr, CREATE_SUSPENDED, nullptr);
    }

    for (int i = 0; i < threadsN; i++) {
        ResumeThread(threads[i]);
    }

    WaitForMultipleObjects(threadsN, threads, true, INFINITE);
    result /= N;
    result *= 4;
    for (int i = 0; i < threadsN; i++) {
        CloseHandle(threads[i]);
    }
    delete[] threads;
    return result;
}


int main() {
    int tests[] = {1, 2, 4, 8, 12, 16, 32};

    for (auto threadsN: tests) {
        std::cout << "threads: " << threadsN;
        auto t1 = std::chrono::high_resolution_clock::now( );
        std::cout << " result: " << std::setprecision(10) << calculatePi(threadsN);
        auto t2 = std::chrono::high_resolution_clock::now( );
        auto dt = duration_cast<std::chrono::duration<double,std::micro>>(t2-t1).count( );
        std::cout << " time: " << round(dt) << std::endl;

    }
    return 0;
}
