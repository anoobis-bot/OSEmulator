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

MainConsole::MainConsole() : AConsole("MAIN_CONSOLE")
{
}

void MainConsole::display() {
    if (ACTION == "CLEAR")
    {
        system("cls");
        displayHeader();
        ACTION = "";
    }
}

void MainConsole::onEnabled()
{
    ACTION = "CLEAR";
    this->display();
}

void MainConsole::process() {
    bool handled = false;
    String command;
    while (!handled) {
        std::cout << "root:\\> ";
        std::getline(std::cin, command);
        handled = handleCommand(command);
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

bool MainConsole::handleCommand(String command)
{
    bool handled = true;
    String formattedInput = toLowerCase(command);
    ConsoleManager* consoleManager = ConsoleManager::getInstance();
    if (command == "exit")
    {
        consoleManager->stopRunning();
    }
    else if (command == "initialize")
    {
        printMsgNewLine("initialize command recognized. Doing something");
    }

    else if (command == "clear" || command == "cls")
    {
        system("cls");
        displayHeader();
    }

    else if (command.substr(0, 6) == "screen")
    {
        auto [screenCommand, processName] = parseScreenCommand(formattedInput);

        if (screenCommand.empty() || processName.empty())
        {
            printMsgNewLine("Incomplete arguments. Use 'screen -s <name>' or 'screen -r <name>'.");
            handled = false;
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
                handled = false;
            }
        }
        else if (screenCommand == "-r")
        {
            if (!consoleManager->isScreenRegistered(processName))
            {
                std::cout << "Process '" << processName << "' not found." << std::endl;
                handled = false;
            }
            else
            {
                consoleManager->switchToScreen(processName);
            }
        }
        else
        {
            printMsgNewLine("Incomplete arguments. Use 'screen -s <name>' or 'screen -r <name>'.");
            handled = false;
        }
    }

    else if (command == "scheduler-test")
    {
        printMsgNewLine("scheduler-test command recognized. Doing something");
    }
    else if (command == "scheduler-stop")
    {
        printMsgNewLine("scheduler-stop command recognized. Doing something");
    }
    else if (command == "report-util")
    {
        printMsgNewLine("report-util command recognized. Doing something");
    }
    else
    {
        std::cout << "Invalid command. Please try again." << std::endl;
        handled = false;
    }

    return handled;
}