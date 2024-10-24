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

void readConfigAndInitializeScheduler() {
    try {
        std::ifstream configFile("config.txt");
        if (!configFile) {
            throw std::runtime_error("Could not open config.txt");
        }

        std::string param;
        ScheduleAlgo scheduleAlgo; 
        int numCores;  
        unsigned int quantumCycles;
        unsigned int delayPerExec;  
        unsigned int minInstructions; 
        unsigned int maxInstructions;
		unsigned int batchProcessFreq;

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
            }
            else {
                int value;
                configFile >> value;

                if (param == "num-cpu") {
                    numCores = value;
					std::cout << "Number of CPUs: " << numCores << std::endl;
                }
                else if (param == "quantum-cycles") {
                    quantumCycles = value;
					std::cout << "Quantum Cycles: " << quantumCycles << std::endl;
                }
                else if (param == "batch-process-freq") {
                    batchProcessFreq = value;
					std::cout << "Batch Process Frequency: " << batchProcessFreq << std::endl;
                }
                else if (param == "delay-per-exec") {
                    delayPerExec = value;
					std::cout << "Delay Per Execution: " << delayPerExec << std::endl;
                }
                else if (param == "min-ins") {
                    minInstructions = value;
					std::cout << "Minimum Instructions: " << minInstructions << std::endl;
                }
                else if (param == "max-ins") {
                    maxInstructions = value;
					std::cout << "Maximum Instructions: " << maxInstructions << std::endl;
                }
                else {
                    std::cout << "Unknown parameter in config.txt: " << param << std::endl;
                }
            }
        }

        // Initialize the Scheduler
        Scheduler::initialize(scheduleAlgo, quantumCycles, numCores, delayPerExec, minInstructions, maxInstructions, batchProcessFreq);
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
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
            readConfigAndInitializeScheduler();
            running = true;       
            isInitialized = true;
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
