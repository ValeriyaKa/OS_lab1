#include <windows.h>
#include <iostream>
#include <vector>
#include <chrono>
#include <thread>

// Встраиваем функцию факторизации прямо в файл нагрузки
std::vector<unsigned long long> FactorizeNumber(unsigned long long n) {
    std::vector<unsigned long long> factors;

    while (n % 2 == 0) {
        factors.push_back(2);
        n /= 2;
    }

    for (unsigned long long i = 3; i * i <= n; i += 2) {
        while (n % i == 0) {
            factors.push_back(i);
            n /= i;
        }
    }

    if (n > 1) {
        factors.push_back(n);
    }

    return factors;
}

void GenerateCPULoad(size_t threadsCount, size_t iterationsPerThread, unsigned long long number) {
    auto loadFunction = [number](size_t iterations) {
        for (size_t i = 0; i < iterations; ++i) {
            std::vector<unsigned long long> factors = FactorizeNumber(number);
            volatile size_t dummy = factors.size(); 
        }
                std::wcout << L"Thread completed factorization for " << number << std::endl;

        };

    std::vector<std::thread> threads;

    auto start = std::chrono::high_resolution_clock::now();

    for (size_t i = 0; i < threadsCount; ++i) {
        threads.emplace_back(loadFunction, iterationsPerThread);
    }

    for (auto& t : threads) {
        t.join();
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    std::wcout << L"Time taken : "
        << elapsed.count() << L" seconds." << std::endl;
}
