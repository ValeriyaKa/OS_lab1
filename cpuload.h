#ifndef CPULOAD_H
#define CPULOAD_H

#include <cstddef>
#include <vector>

std::vector<unsigned long long> FactorizeNumber(unsigned long long n);


void GenerateCPULoad(size_t threadsCount, size_t iterationsPerThread, unsigned long long number);

#endif
