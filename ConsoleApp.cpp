#include <Windows.h>
#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include "cpuload.h"
#include "diskload.h"
#include "cpudiskload.h"
#include "ntdll.h"
#include <thread>

#pragma comment(lib, "ntdll")

void ExecuteFactorization(const std::wstring& numberStr, int iterations) {
    try {
        unsigned long long number = std::stoull(numberStr);

        if (number == 0) {
            std::wcerr << L"Factorization of 0 is undefined." << std::endl;
            return;
        }

        auto start = std::chrono::high_resolution_clock::now();

        for (int i = 0; i < iterations; ++i) {
            std::vector<unsigned long long> factors = FactorizeNumber(number);

            std::wcout << L"Iteration " << i + 1 << L": ";
            for (size_t j = 0; j < factors.size(); ++j) {
                std::wcout << factors[j];
                if (j < factors.size() - 1) {
                    std::wcout << L" * ";
                }
            }
            std::wcout << std::endl;
        }

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end - start;

        std::wcout << L"Execution time for " << iterations << L" iterations: " << elapsed.count() << L" seconds." << std::endl;
    }
    catch (...) {
        std::wcerr << L"Invalid input. Please provide a valid positive number." << std::endl;
    }
}

void CreateProcessWithArguments(const std::wstring& executablePath, const std::vector<std::wstring>& arguments) {
    std::wstring ntPath = L"\\??\\" + executablePath;

    std::wstring commandLine = executablePath;
    for (const auto& arg : arguments) {
        commandLine += L" " + arg;
    }

    UNICODE_STRING NtImagePath;
    RtlInitUnicodeString(&NtImagePath, const_cast<PWSTR>(ntPath.c_str()));

    PRTL_USER_PROCESS_PARAMETERS ProcessParameters = NULL;
    NTSTATUS status = RtlCreateProcessParametersEx(
        &ProcessParameters,
        &NtImagePath,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        RTL_USER_PROCESS_PARAMETERS_NORMALIZED
    );

    if (!NT_SUCCESS(status)) {
        std::wcerr << L"Failed to create process parameters. Error: " << std::hex << status << std::endl;
        return;
    }

    PS_CREATE_INFO CreateInfo = { 0 };
    CreateInfo.Size = sizeof(CreateInfo);
    CreateInfo.State = PsCreateInitialState;

    PPS_ATTRIBUTE_LIST AttributeList = (PS_ATTRIBUTE_LIST*)RtlAllocateHeap(RtlProcessHeap(), HEAP_ZERO_MEMORY, sizeof(PS_ATTRIBUTE));
    if (!AttributeList) {
        std::wcerr << L"Failed to allocate memory for attribute list." << std::endl;
        RtlDestroyProcessParameters(ProcessParameters);
        return;
    }
    AttributeList->TotalLength = sizeof(PS_ATTRIBUTE_LIST) - sizeof(PS_ATTRIBUTE);
    AttributeList->Attributes[0].Attribute = PS_ATTRIBUTE_IMAGE_NAME;
    AttributeList->Attributes[0].Size = NtImagePath.Length;
    AttributeList->Attributes[0].Value = (ULONG_PTR)NtImagePath.Buffer;

    HANDLE hProcess = NULL, hThread = NULL;

    auto start = std::chrono::high_resolution_clock::now();

    status = NtCreateUserProcess(
        &hProcess,
        &hThread,
        PROCESS_ALL_ACCESS,
        THREAD_ALL_ACCESS,
        NULL,
        NULL,
        NULL,
        NULL,
        ProcessParameters,
        &CreateInfo,
        AttributeList
    );

    if (!NT_SUCCESS(status)) {
        std::wcerr << L"Failed to create process. Error: " << std::hex << status << std::endl;
    }
    else {
        std::wcout << L"Successfully launched: " << executablePath << std::endl;

        WaitForSingleObject(hProcess, INFINITE);

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end - start;

        std::wcout << L"Execution time: " << elapsed.count() << L" seconds." << std::endl;
    }

    if (hProcess) CloseHandle(hProcess);
    if (hThread) CloseHandle(hThread);
    RtlFreeHeap(RtlProcessHeap(), 0, AttributeList);
    RtlDestroyProcessParameters(ProcessParameters);
}

int wmain(int argc, wchar_t* argv[]) {
    if (argc > 1) {
        std::wstring command = argv[1];

        if (command == L"cpu-load") {
            size_t threadsCount = 1;
            size_t iterationsPerThread = 100000000;
            unsigned long long number = 1232310;

            GenerateCPULoad(threadsCount, iterationsPerThread, number);
        }
        else if (command == L"disk-load") {
            size_t diskIterations = 10050000;

            WriteToFile(diskIterations);
        }
        else if (command == L"factorize") {
            if (argc >= 4) {
                std::wstring numberStr = argv[2];
                int iterations = std::stoi(argv[3]);
                ExecuteFactorization(numberStr, iterations);
            }
            else {
                std::wcerr << L"Usage: factorize <number> <iterations>" << std::endl;
            }
        }
        else if (command == L"cpudisk-load") {
            size_t cpuThreads = 1;
            size_t cpuIterations = 100000000;
            size_t diskIterations = 10000000;
            unsigned long long number = 1232310;

            RunCombinedLoad(cpuThreads, cpuIterations, diskIterations, number);
        }
        else {
            CreateProcessWithArguments(L"C:\\Windows\\System32\\" + command, {});
        }
        return 0;
    }

    while (true) {
        std::wcout << L"Enter program to execute (cpu-load, disk-load, cpudisk-load): ";
        std::wstring commandLine;
        std::getline(std::wcin, commandLine);

        if (commandLine == L"exit") {
            break;
        }
        else if (commandLine == L"cpu-load") {
            size_t threadsCount = 1;
            size_t iterationsPerThread = 100000000;
            unsigned long long number = 1232310;

            GenerateCPULoad(threadsCount, iterationsPerThread, number);
        }
        else if (commandLine == L"disk-load") {
            size_t diskIterations = 10050000;

            WriteToFile(diskIterations);
        }
        else if (commandLine == L"factorize") {
            std::wcout << L"Enter the number to factorize: ";
            std::wstring number;
            std::getline(std::wcin, number);

            std::wcout << L"Enter number of iterations: ";
            int iterations;
            std::wcin >> iterations;
            std::wcin.ignore();

            ExecuteFactorization(number, iterations);
        }
        else if (commandLine == L"cpudisk-load") {
            size_t cpuThreads = 1;
            size_t cpuIterations = 100000000;
            size_t diskIterations = 10000000;
            unsigned long long number = 1232310;

            RunCombinedLoad(cpuThreads, cpuIterations, diskIterations, number);
        }
        else {
            CreateProcessWithArguments(L"C:\\Windows\\System32\\" + commandLine, {});
        }
    }
    return 0;
}
