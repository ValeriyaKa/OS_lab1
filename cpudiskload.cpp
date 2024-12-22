#include <windows.h>
#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include <fstream>
#include <string>
#include "cpuload.h"
#include "diskload.h"
#include "cpudiskload.h"

void RunCombinedLoad(size_t cpuThreads, size_t cpuIterations, unsigned long long number,size_t diskIterations) {
    std::thread cpuThread([=]() {
        GenerateCPULoad(cpuThreads, cpuIterations, number);
        });

    std::thread diskThread([=]() {
        WriteToFile(diskIterations);
        });

    cpuThread.join();
    diskThread.join();

    std::wcout << L"Combined CPU and Disk load complete.\n";
}
