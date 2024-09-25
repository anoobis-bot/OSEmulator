// OSEmulator.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <chrono>
#include <thread>
#include <sstream>
#include "utils.h"
#include "ConsoleManager.h"
#include "BaseScreen.h"

std::pair<String, String> parseScreenCommand(String userInput) {
    std::string command, name;
    std::stringstream ss(userInput);
    ss >> command;  // Gets 'screen'
    if (ss >> command && (command == "-r" || command == "-s")) {
        ss >> name;  // Gets the <name>
    }
    return {command, name};
}

int main() {
    ConsoleManager::initialize();
    auto consoleManager = ConsoleManager::getInstance();

    printHeader("3D_CSOPESY.txt");
    
    std::string userInput = "";
    std::string formattedInput = "";
    printMsgNewLine("Type 'exit' to quit, 'clear' to clear the screen.");

    while (formattedInput != "exit") {
        printMsg("Command: ");
        std::getline(std::cin, userInput);  // Use getline to capture full input
        formattedInput = toLowerCase(userInput);

        if (formattedInput.substr(0, 6) == "screen") {
            auto [command, screenName] = parseScreenCommand(formattedInput);

            if (command == "-r") {
                consoleManager->switchToScreen(screenName);
            } else if (command == "-s") {
                auto screen = std::make_shared<BaseScreen>(screenName, std::make_shared<String>(screenName));
                consoleManager->registerScreen(screen);
                consoleManager->switchToScreen(screenName);
            }
        } 
        else if (formattedInput == "exit") {
            consoleManager->returnToPreviousConsole();
        } 
        else if (formattedInput == "clear") {
            std::cout << "\033[2J\033[1;1H"; // Clear the screen
            printHeader("3D_CSOPESY.txt");
        }
        else if (formattedInput == "initialize") {
            printMsgNewLine("initialize command recognized. Doing something");
        }
        else if (formattedInput == "scheduler-test") {
            printMsgNewLine("scheduler-test command recognized. Doing something");
        }
        else if (formattedInput == "scheduler-stop") {
            printMsgNewLine("scheduler-stop command recognized. Doing something");
        }
        else if (formattedInput == "report-util") {
            printMsgNewLine("report-util command recognized. Doing something");
        }
        else {
            printMsgNewLine("Command not found. These are the available commands: ");
            printMsgNewLine("initialize, screen, scheduler-test, scheduler-stop, report-util, clear, and exit.");
        }

        consoleManager->drawConsole();  // Draw the current console after processing input
    }

    printMsg("Program Terminated");
    ConsoleManager::destroy();  // Clean up resources
    return 0;
}
