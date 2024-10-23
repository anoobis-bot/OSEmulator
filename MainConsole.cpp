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
    String formattedInput = toLowerCase(command);
    static bool isInitialized = false; //default to false
    auto consoleManager = ConsoleManager::getInstance();
    if (command == "exit")
    {
        std::cout << "Exiting the program..." << std::endl;
        exit(0);
        std::terminate();

    }
    else if (command == "initialize")  // see if the initialization is working
    {
        if (isInitialized) {
            printMsg("Already initialized. Please restart to reinitialize.");
            return;
        }

        std::ifstream configFile("config.txt");
        if (!configFile) {
            throw std::runtime_error("Could not open config.txt");
        }

        String param;
        int value;

        while (configFile >> param >> value) {
            if (param == "num-cpu") {
                if (value < 1 || value > 128) { // inputed a temp limit
                    throw std::out_of_range("num-cpu must be in the range [1, 128]");
                }
                // Set number of CPUs in the Scheduler instance
                Scheduler::getInstance()->setNumCores(value);
            }
            else if (param == "scheduler") {
                if (value == 0) { 
                    Scheduler::getInstance()->setSchedulerType("rr");
                }
                else {
                    Scheduler::getInstance()->setSchedulerType("fcfs");
                }
            }
            else if (param == "quantum-cycles") {
                if (value < 1 || value >(1 << 32)) {
                    throw std::out_of_range("quantum-cycles must be in the range [1, 232]");
                }
                Scheduler::getInstance()->setQuantumCycles(value);
            }
            else if (param == "batch-process-freq") {
                if (value < 1 || value >(1 << 32)) {
                    throw std::out_of_range("batch-process-freq must be in the range [1, 232]");
                }
                Scheduler::getInstance()->setBatchProcessFreq(value);
            }
            else if (param == "min-ins") {
                if (value < 1 || value >(1 << 32)) {
                    throw std::out_of_range("min-ins must be in the range [1, 232]");
                }
                Scheduler::getInstance()->setMinInstructions(value);
            }
            else if (param == "max-ins") {
                if (value < 1 || value >(1 << 32)) {
                    throw std::out_of_range("max-ins must be in the range [1, 232]");
                }
                Scheduler::getInstance()->setMaxInstructions(value);
            }
            else if (param == "delay-per-exec") {
                if (value < 0 || value >(1 << 32)) {
                    throw std::out_of_range("delay-per-exec must be in the range [0, 232]");
                }
                Scheduler::getInstance()->setDelayPerExec(value);
            }
            else {
                printMsg("Unknown parameter in config.txt: " + param);
            }
        }

        isInitialized = true; // Mark initialization as complete
        printMsg("Initialization complete.");
    }

    else if (!isInitialized) {
        printMsg("Please run the 'initialize' command first.");
    }

    else if (command == "clear" || command == "cls")
    {
        system("cls");
        displayHeader();
    }

    else if (command.substr(0, 6) == "screen")
    {
        auto [screenCommand, processName] = parseScreenCommand(formattedInput);

        if (screenCommand.empty())
        {
            printMsgNewLine("Incomplete arguments. Use 'screen -s <name>' or 'screen -r <name>'.");
        }

        else if (screenCommand == "-ls")
        {
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
                    if (process->getCoreID() != -1 && process->getState() != Process::FINISHED)
                    {
                        std::cout << std::left << std::setw(25) << process->getName()  
                            << std::setw(30) << process->getFormattedTime()
                            << "Core:" << process->getCoreID() << '\t'
                            << '\t' << process->getCurrentInstruction() << "/"
                            << process->getTotalInstructions() << '\n';
                    }
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
                // Register new process and switch to the screen
                String toPrint = "Hello world from " + processName;
                auto process = std::make_shared<Process>(processName, Scheduler::getInstance()->getAllProcess().size(), 100, PrintCommand(toPrint));
                auto processScreen = std::make_shared<BaseScreen>(process, processName);

                Scheduler::getInstance()->addNewProcess(process);

                consoleManager->registerScreen(processScreen);
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
        printMsg("scheduler-test command recognized. Doing something");
    }
    else if (command == "scheduler-stop")
    {
        printMsg("scheduler-stop command recognized. Doing something");
    }
    else if (command == "report-util")
    {
        printMsg("report-util command recognized. Doing something");
    }
    else if (command == "debug-info")
    {
        std::cout << "Size of Ready Queue: " << Scheduler::getInstance()->getSize() << '\n';
        std::cout << "Number of cores: " << Scheduler::getInstance()->numCores() << '\n';
        std::cout << "Process List: \n";
        Scheduler::getInstance()->printProcesses();
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
