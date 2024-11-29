#pragma once
#include "MainConsole.h"
#include "TypedefRepo.h"
#include "ConsoleManager.h"
#include "Process.h"
#include "AConsole.h"
#include <iostream>
#include "utils.h"
#include <chrono>
#include <thread>
#include <sstream>
#include <memory>
#include <iomanip>
#include "PrintCommand.h"
#include "Scheduler.h"
#include <fstream>
#include <stdexcept>
#include <random>
#include "MemoryManager.h"
#include <unordered_set>

extern ConsoleManager consoleManager;

MainConsole::MainConsole() : AConsole("MAIN_CONSOLE")
{
}

void MainConsole::display() {
    displayHeader();
    process();
}

void MainConsole::onEnabled()
{
    displayHeader();
}

void MainConsole::process() {
    String command;
    while (true) {
        std::cout << "root:\\> ";
        std::getline(std::cin, command);
        handleCommand(command);
        std::cout << std::endl;
    }
}

void MainConsole::displayHeader() {
    printHeader("3D_CSOPESY.txt");
}

std::pair<String, String> parseScreenCommand(String userInput) {
    String command;
    String name;
    std::stringstream ss(userInput);
    ss >> command;  // Gets 'screen'
    if (ss >> command && (command == "-r" || command == "-s")) {
        ss >> name;  // Gets the <name>
    }
    return { command, name };
}

void MainConsole::handleCommand(String command)
{
    auto consoleManager = ConsoleManager::getInstance();
    if (command == "exit")
    {
        std::cout << "Exiting the program..." << std::endl;
        exit(0);
        std::terminate();

    }    
    else if (command == "clear" || command == "cls")
    {
        system("cls");
        displayHeader();
    }

    else if (command.substr(0, 6) == "screen")
    {
        auto [screenCommand, processName] = parseScreenCommand(command);

        if (screenCommand.empty())
        {
            printMsgNewLine("Incomplete arguments. Use 'screen -s <name>' or 'screen -r <name>'.");
        }

        else if (screenCommand == "-ls")
        {
            std::lock_guard<std::mutex> lock(mtx); // Lock the mutex before modifying the vector
            std::cout << "CPU Utilization: "
                << std::fixed << std::setprecision(2)
                << Scheduler::getInstance()->getCPUUtilization() * 100  
                << "%" << '\n';
			std::cout << "Cores used: " << Scheduler::getInstance()->getNumberOfCoresUsed() << '\n';
			std::cout << "Cores available: " << Scheduler::getInstance()->getAvailableCores() << '\n';
            std::cout << '\n';
            std::cout << "---------------------------------------" << '\n';
            std::vector<std::shared_ptr<Process>>& allProcesses = Scheduler::getInstance()->getAllProcess();

            if (allProcesses.empty())
            {
                std::cout << "You currently don't have any processes." << '\n';
            }
            else
            {
                std::cout << "Running Processes:\n";
            	for (const std::shared_ptr<Process>& process : allProcesses)
                {
                    process->printInfo();
                    //if (process->getCoreID() != -1 && process->getState() != Process::FINISHED)
                    //{
                    //    std::cout << std::left << std::setw(25) << process->getName()  
                    //        << std::setw(30) << process->getFormattedTime()
                    //        << "Core:" << process->getCoreID() << '\t'
                    //        << '\t' << process->getCurrentInstruction() << "/"
                    //        << process->getTotalInstructions() << '\n';
                    //}
                }
                
                std::cout << "\nFinished Processes:\n";

                for (const std::shared_ptr<Process>& process : allProcesses)
                {
                    if (process->getState() == Process::FINISHED) 
                    {
                        std::cout << std::left << std::setw(25) << process->getName() 
                            << std::setw(30) << process->getFormattedTime()
                            << "FINISHED" << '\t'
                            << '\t' << process->getCurrentInstruction() << "/"
                            << process->getTotalInstructions() << '\n';
                    }
                }
            }
            std::cout << "---------------------------------------" << '\n';
        }


        else if (processName.empty())
        {
            printMsgNewLine("Incomplete arguments. Use 'screen -s <name>' or 'screen -r <name>'.");
        }
        else if (screenCommand == "-s")
        {
            if (!consoleManager->isScreenRegistered(processName))
            {
                std::random_device rd;  // Random number seed
                std::mt19937 gen(rd()); // Random number generator (Mersenne Twister)

                // Generate random instructions
                std::uniform_int_distribution<> disInstructions(Scheduler::getInstance()->getMinInstructions(), Scheduler::getInstance()->getMaxInstructions());
                int totalInstructions = disInstructions(gen);

                // Calculate min and max powers of 2 within the range
                int minPower = std::ceil(std::log2(Scheduler::getInstance()->getMinMemPerProc()));
                int maxPower = std::floor(std::log2(Scheduler::getInstance()->getMaxMemPerProc()));

                // Ensure minPower and maxPower are valid
                if (minPower > maxPower) {
                    throw std::invalid_argument("No power of 2 values within the specified memory range");
                }

                // Randomly select a power of 2
                std::uniform_int_distribution<> disPower(minPower, maxPower);
                int randomPower = disPower(gen);
                int memPerProc = 1 << randomPower; // 2^randomPower

                // Get the next process ID
                int processID = Scheduler::getInstance()->getAllProcess().size();

                // Add the processID and memPerProc to the processMemoryMap
                Scheduler::getInstance()->getProcessMemoryMap()[processID] = memPerProc;

                // Create the process and screen
                std::string toPrint = "Hello world from " + processName;
                auto process = std::make_shared<Process>(
                    processName,
                    processID,
                    totalInstructions,
                    PrintCommand(toPrint),
                    memPerProc
                );

                auto processScreen = std::make_shared<BaseScreen>(process, processName);

                // Add the new process and register the screen
                Scheduler::getInstance()->addNewProcess(process);
                consoleManager->registerScreen(processScreen);

                // Switch to the newly created screen
                consoleManager->switchToScreen(processName);
            }
            else
            {
                std::cout << "Process '" << processName << "' already exists or has existed. Please provide a different name." << std::endl;
            }
        }
        else if (screenCommand == "-r")
        {
            if (!consoleManager->isScreenRegistered(processName))
            {
                std::cout << "Process '" << processName << "' not found." << std::endl;
            }
            else
            {
                consoleManager->switchToScreen(processName);
            }
        }
        else
        {
            printMsgNewLine("Incomplete arguments. Use 'screen -s <name>' or 'screen -r <name>'.");
        }
    }

    else if (command == "scheduler-test")
    {
		Scheduler::getInstance()->schedulerTest();
    }
    else if (command == "scheduler-stop")
    {
        Scheduler::getInstance()->schedulerStop();
    }
    else if (command == "report-util")
    {
        saveReport();
    }
    else if (command == "process-smi")
    {
        std::lock_guard<std::mutex> lock(mtx);
        auto scheduler = Scheduler::getInstance();
        auto memoryManager = MemoryManager::getInstance();


        std::cout << "----------------------------------------------" << std::endl;
        std::cout << "| PROCESS-SMI V01.00 Driver Version: 01.00 |" << std::endl;
        std::cout << "----------------------------------------------" << std::endl;
        // Display CPU Utilization
        double cpuUtil = scheduler->getCPUUtilization() * 100;
        std::cout << "CPU Utilization: " << std::fixed << std::setprecision(2) << cpuUtil << "%" << std::endl;

        // Display Memory Usage
        size_t totalMemory = memoryManager->getMemorySize();
        size_t usedMemory = 0;
        const auto& frameTable = memoryManager->getFrameTable();

        // Calculate used memory based on frame table
        for (const auto& [frameIndex, frameData] : frameTable)
        {
            if (std::get<bool>(frameData)) // If frame is allocated
            {
                usedMemory += memoryManager->getMemPerFrame();
            }
        }

        double memoryUtil = (static_cast<double>(usedMemory) / totalMemory) * 100;
        std::cout << "Memory Usage: " << usedMemory << " MiB / " << totalMemory << " MiB" << std::endl;
        std::cout << "Memory Util: " << std::fixed << std::setprecision(2) << memoryUtil << "%" << std::endl;

        // Display Running Processes
        std::cout << "\n----------------------------------------------" << std::endl;
        std::cout << "Running Processes and memory usage:\n";
        std::cout << "----------------------------------------------" << std::endl;

        const auto& allProcesses = scheduler->getAllProcess();
        const auto& processMemoryMap = scheduler->getProcessMemoryMap();
        std::unordered_set<int> activePIDs;

        // Collect PIDs of processes in memory
        for (const auto& [frameIndex, frameData] : frameTable)
        {
            if (std::get<bool>(frameData)) // If frame is allocated
            {
                auto process = std::get<std::shared_ptr<Process>>(frameData);
                if (process)
                {
                    activePIDs.insert(process->getID());
                }
            }
        }

        if (activePIDs.empty())
        {
            std::cout << "No running processes in memory.\n";
        }
        else
        {
            for (const auto& process : allProcesses)
            {
                int processID = process->getID();
                if (activePIDs.find(processID) != activePIDs.end())
                {
                    std::string processName = process->getName();
                    size_t memoryUsed = 0;

                    // Calculate memory usage for the process
                    for (const auto& [frameIndex, frameData] : frameTable)
                    {
                        if (std::get<bool>(frameData)) // If frame is allocated
                        {
                            auto allocatedProcess = std::get<std::shared_ptr<Process>>(frameData);
                            if (allocatedProcess && allocatedProcess->getID() == processID)
                            {
                                memoryUsed += memoryManager->getMemPerFrame();
                            }
                        }
                    }

                    std::cout << processName << " (PID: " << processID << ") " << memoryUsed << " MiB" << std::endl;
                }
            }
        }
        std::cout << "----------------------------------------------\n";
    }

    else if (command == "debug-info")
    {
        std::cout << "Size of Ready Queue: " << Scheduler::getInstance()->getSize() << '\n';
        std::cout << "Number of cores: " << Scheduler::getInstance()->numCores() << '\n';
        std::cout << "Process List: \n";
        Scheduler::getInstance()->printProcesses();
    }
    else if (command == "initialize")
    {
        std::cout << "The emulator has already been initialized." << std::endl;
    }
    else
    {
        std::cout << "Invalid command. Please try again." << std::endl;
    }
}

String MainConsole::formatNA(int num)
{
	if (num == -1)
	{
        return "N/A";
	}

	return std::to_string(num);

}

void MainConsole::saveReport()
{
    std::ofstream file("csopesy-log.txt");
    if (!file) {
        std::cerr << "Error opening file." << std::endl;
        return;
    }

    file << "CPU Utilization: "
        << std::fixed << std::setprecision(2)
        << Scheduler::getInstance()->getCPUUtilization() * 100
        << "%" << '\n';
    file << "Cores used: " << Scheduler::getInstance()->getNumberOfCoresUsed() << '\n';
    file << "Cores available: " << Scheduler::getInstance()->getAvailableCores() << '\n';
    file << '\n';
    file << "---------------------------------------" << '\n';
    std::vector<std::shared_ptr<Process>>& allProcesses = Scheduler::getInstance()->getAllProcess();

    if (allProcesses.empty())
    {
        file << "You currently don't have any processes." << '\n';
    }
    else
    {
        file << "Running Processes:\n";

        for (const std::shared_ptr<Process>& process : allProcesses)
        {
            if (process->getCoreID() != -1 && process->getState() != Process::FINISHED)
            {
                file << std::left << std::setw(25) << process->getName()
                    << std::setw(30) << process->getFormattedTime()
                    << "Core:" << process->getCoreID() << '\t'
                    << '\t' << process->getCurrentInstruction() << "/"
                    << process->getTotalInstructions() << '\n';
            }
        }

        file << "\nFinished Processes:\n";

        for (const std::shared_ptr<Process>& process : allProcesses)
        {
            if (process->getState() == Process::FINISHED)
            {
                file << std::left << std::setw(25) << process->getName()
                    << std::setw(30) << process->getFormattedTime()
                    << "FINISHED" << '\t'
                    << '\t' << process->getCurrentInstruction() << "/"
                    << process->getTotalInstructions() << '\n';
            }
        }
    }
    file << "---------------------------------------" << '\n';
    std::cout << "Report generated at csopesy-log.txt!" << std::endl;
}