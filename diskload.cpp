#include "diskload.h"
#include <iostream>
#include <fstream>
#include <chrono>
#include <vector>
#include <string>

const std::wstring filePath = L"D:\\test.txt"; 

void WriteToFile(size_t iterations) {
    const size_t blockSize = 512;  
    char* buffer = new char[blockSize];
    memset(buffer, 'A', blockSize);

    std::ofstream outFile(filePath, std::ios::binary | std::ios::app);
    if (!outFile) {
        std::wcerr << L"Error opening the file for writing:" << filePath << std::endl;
        delete[] buffer;
        return;
    }

    auto start = std::chrono::high_resolution_clock::now();

    for (size_t i = 0; i < iterations; ++i) {
        outFile.write(buffer, blockSize);  
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    delete[] buffer;
    outFile.close();

    std::wcout << L"Disk write completed for " << iterations
        << L" iterations. Time taken: " << elapsed.count() << L" seconds." << std::endl;
}
