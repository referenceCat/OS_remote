//
// Created by referenceCat on 03.06.2024.
//

#include <iostream>
#include <iomanip>
#include <omp.h>
#include <chrono>
#include <cmath>

using namespace std;


const size_t blockSize = 10 * 230704;
const size_t N = 100000000;

long double calculatePi(int threadsN);

int main() {
    int tests[] = {1, 2, 4, 8, 12, 16, 32};

    for (auto threadsN: tests) {
        std::cout << "threads: " << threadsN;
        auto t1 = std::chrono::high_resolution_clock::now( );
        std::cout << " result: " << std::setprecision(20) << calculatePi(threadsN);
        auto t2 = std::chrono::high_resolution_clock::now( );
        auto dt = duration_cast<std::chrono::duration<double,std::micro>>(t2-t1).count( );
        std::cout << " time: " << round(dt) << std::endl;

    }
    return 0;
}

long double calculatePi(int threadsN) {

    long double summaryResult = 0.0;
    long double x;
    // omp_set_num_threads(threadsN);
    #pragma omp parallel reduction(+: summaryResult) num_threads(localThreads)
    {
    #pragma omp for schedule(dynamic, blockSize) nowait
        for (int i = 0; i < N; i++)
        {
            x = (i + 0.5) / N ;
            summaryResult += 1 / (x * x + 1);
        }
    }

    summaryResult = summaryResult / N * 4;

    return summaryResult;
}
