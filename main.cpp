#include "ConsoleManager.h"
#include <iostream>
#include <fstream>
#include "MainConsole.h"
#include "Scheduler.h"
#include "TypedefRepo.h"
#include "Process.h"
#include "PrintCommand.h"
#include "utils.h"
#include <chrono> 
#include <thread> 

#include "MemoryManager.h"

bool readConfigAndInitializeScheduler() {
    try {
        std::ifstream configFile("config.txt");
        if (!configFile) {
            throw std::runtime_error("Could not open config.txt");
        }

        std::string param;
        ScheduleAlgo scheduleAlgo;
        int numCores = 0;
        unsigned int quantumCycles = 0;
        unsigned int delayPerExec = 0;
        unsigned int minInstructions = 0;
        unsigned int maxInstructions = 0;
        unsigned int batchProcessFreq = 0;

        const unsigned long long maxValue = 4294967296; // 2^32
        bool isValid = true; // Flag to track if all parameters are valid

        std::cout << "Scheduler Configuration:" << std::endl;

        // Reading the config file
        while (configFile >> param) {
            if (param == "scheduler") {
                std::string schedulerType;
                configFile >> schedulerType;

                std::cout << "Scheduler type: ";
                if (schedulerType == "\"rr\"") {
                    scheduleAlgo = RR;
                    std::cout << "Round Robin" << std::endl;
                }
                else if (schedulerType == "\"fcfs\"") {
                    scheduleAlgo = FCFS;
                    std::cout << "FCFS" << std::endl;
                }
                else {
                    throw std::runtime_error("Invalid scheduler type in config.txt.");
                }
            }
            else {
                unsigned long long value; // Use unsigned long long to accommodate large values
                configFile >> value;

                if (param == "num-cpu") {
                    if (value < 1 || value > 128) {
                        std::cerr << "Error: Number of CPUs must be in the range [1, 128]." << std::endl;
                        isValid = false;
                    }
                    else {
                        numCores = static_cast<int>(value);
                        std::cout << "Number of CPUs: " << numCores << std::endl;
                    }
                }
                else if (param == "quantum-cycles") {
                    if (value < 1 || value >= maxValue) {
                        std::cerr << "Error: Quantum Cycles must be in the range [1, 2^32)." << std::endl;
                        isValid = false;
                    }
                    else {
                        quantumCycles = static_cast<unsigned int>(value);
                        std::cout << "Quantum Cycles: " << quantumCycles << std::endl;
                    }
                }
                else if (param == "batch-process-freq") {
                    if (value < 1 || value >= maxValue) {
                        std::cerr << "Error: Batch Process Frequency must be in the range [1, 2^32)." << std::endl;
                        isValid = false;
                    }
                    else {
                        batchProcessFreq = static_cast<unsigned int>(value);
                        std::cout << "Batch Process Frequency: " << batchProcessFreq << std::endl;
                    }
                }
                else if (param == "delay-per-exec") {
                    if (value < 0 || value >= maxValue) {
                        std::cerr << "Error: Delay Per Execution must be in the range [0, 2^32)." << std::endl;
                        isValid = false;
                    }
                    else {
                        delayPerExec = static_cast<unsigned int>(value);
                        std::cout << "Delay Per Execution: " << delayPerExec << std::endl;
                    }
                }
                else if (param == "min-ins") {
                    if (value < 1 || value >= maxValue) {
                        std::cerr << "Error: Minimum Instructions must be in the range [1, 2^32)." << std::endl;
                        isValid = false;
                    }
                    else {
                        minInstructions = static_cast<unsigned int>(value);
                        std::cout << "Minimum Instructions: " << minInstructions << std::endl;
                    }
                }
                else if (param == "max-ins") {
                    if (value < 1 || value >= maxValue) {
                        std::cerr << "Error: Maximum Instructions must be in the range [1, 2^32)." << std::endl;
                        isValid = false;
                    }
                    else {
                        maxInstructions = static_cast<unsigned int>(value);
                        std::cout << "Maximum Instructions: " << maxInstructions << std::endl;
                    }
                }
                else {
                    std::cerr << "Unknown parameter in config.txt: " << param << std::endl;
                }
            }
        }

        // Initialize the Scheduler only if all parameters are valid
        if (isValid) {
            Scheduler::initialize(scheduleAlgo, quantumCycles, numCores, delayPerExec, minInstructions, maxInstructions, batchProcessFreq);
            std::cout << "Scheduler initialized successfully." << std::endl;
            return true; 
        }
        else {
            std::cerr << "Scheduler initialization failed." << std::endl;
            return false; 
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return false; 
    }
}

int main()
{
    bool isInitialized = false;
    bool running = false;
    printHeaderMain("3D_CSOPESY.txt");

    while (!isInitialized)
    {
        std::string command;
        std::cout << "root:\\> ";
        std::cin >> command;

        if (command == "initialize")
        {
            ConsoleManager::initialize();
            if (readConfigAndInitializeScheduler()) { 
                MemoryManager::initialize(1024, 16);
            	running = true;
                isInitialized = true;
            }
            else {
                ConsoleManager::destroy(); 
            }
            std::cin.ignore();
        }
        else if (command == "exit")
        {
            std::cout << "Exiting...\n";
            return 0;
        }
        else
        {
            std::cout << "Invalid command.\n";
        }
    }

    while (running)
    {
        ConsoleManager::getInstance()->process();
        ConsoleManager::getInstance()->drawConsole();
        running = ConsoleManager::getInstance()->isRunning();
    }

    ConsoleManager::destroy();
    return 0;
}
