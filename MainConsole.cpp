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
    auto consoleManager = ConsoleManager::getInstance();
    if (command == "exit")
    {
        std::cout << "Exiting the program..." << std::endl;
        exit(0);
        std::terminate();

    }
    else if (command == "initialize")
    {
        printMsg("initialize command recognized. Doing something");
    }

    else if (command == "clear" || command == "cls")
    {
        system("cls");
    }

    else if (command.substr(0, 6) == "screen")
    {
        auto [screenCommand, processName] = parseScreenCommand(formattedInput);

        if (screenCommand.empty() || processName.empty())
        {
            printMsgNewLine("Incomplete arguments. Use 'screen -s <name>' or 'screen -r <name>'.");
        }
        else if (screenCommand == "-s")
        {
            if (!consoleManager->isScreenRegistered(processName))
            {
                // Register new process and switch to the screen
                auto process = std::make_shared<Process>();
                auto processScreen = std::make_shared<BaseScreen>(process, processName);
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
    else
    {
        std::cout << "Invalid command. Please try again." << std::endl;
    }
}