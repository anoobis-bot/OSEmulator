#include <iostream>
#include <chrono>
#include <thread>
#include <sstream>
#include "utils.h"
#include "ConsoleManager.h"
#include "MainScreen.h"
#include <memory>

MainScreen::MainScreen(): BaseScreen("MAIN_CONSOLE", std::make_shared<String>("MAIN_CONSOLE")) {
}

MainScreen::~MainScreen() {

}

void MainScreen::onExecute() {
    display();
    process();
}

void MainScreen::onEnabled() {
    system("cls");
    onExecute();
}

void MainScreen::display() {
    printHeader("3D_CSOPESY.txt");
}

std::pair<String, String> parseScreenCommand(String userInput) {
    std::string command, name;
    std::stringstream ss(userInput);
    ss >> command;  // Gets 'screen'
    if (ss >> command && (command == "-r" || command == "-s")) {
        ss >> name;  // Gets the <name>
    }
    return { command, name };
}

void MainScreen::process() {
    String command;
    printMsgNewLine("Type 'exit' to quit, 'clear' to clear the screen.");
    while (true) {
        auto consoleManager = ConsoleManager::getInstance(); 
        auto currentConsole = consoleManager->getCurrentConsole();
        std::cout << "root:\\> ";
        std::getline(std::cin, command);

        if (toLowerCase(command) == "exit") {
            if (currentConsole->getName() == "MAIN_CONSOLE") {
                break; // Exit the program
            }
            else {
                std::dynamic_pointer_cast<BaseScreen>(currentConsole)->exitScreen();
            }
        }

        handleCommand(command); 
        std::cout << std::endl;
    }
}

void MainScreen::handleCommand(String command) {
    std::string formattedInput = toLowerCase(command);  // Lowercase input for comparison
    auto consoleManager = ConsoleManager::getInstance();

    if (formattedInput.substr(0, 6) == "screen") {
        auto [screenCommand, processName] = parseScreenCommand(formattedInput);

        if (screenCommand == "-s") {
            if (processName.empty()) {
                std::cout << "Command not recognized! Please provide a process name." << std::endl;
            }
            else {
                if (!consoleManager->isScreenRegistered(processName)) { // Check if the screen exists
                    consoleManager->createProcessScreen(processName);
                }
                else {
                    std::cout << "Process already exists or has existed. Please provide a different name." << std::endl;
                }
            }
        }
        else if (screenCommand == "-r") {
            if (processName.empty()) {
                std::cout << "Command not recognized! Please provide a process name." << std::endl;
            }
            else {
                if (!consoleManager->isScreenRegistered(processName)) {
                    std::cout << "Process " << processName << " not found." << std::endl;
                }
                else {
                    consoleManager->switchConsole(processName);
                }
            }
        }
    }

    else if (formattedInput == "clear") {
        system("cls");  // Clear the screen
    }
    else if (formattedInput == "initialize") {
        printMsg("initialize command recognized. Doing something");
    }
    else if (formattedInput == "scheduler-test") {
        printMsg("scheduler-test command recognized. Doing something");
    }
    else if (formattedInput == "scheduler-stop") {
        printMsg("scheduler-stop command recognized. Doing something");
    }
    else if (formattedInput == "report-util") {
        printMsg("report-util command recognized. Doing something");
    }
    else {
        printMsgNewLine("Command not found. These are the available commands: ");
        printMsg("initialize, screen, scheduler-test, scheduler-stop, report-util, clear, and exit.");
    }

    consoleManager->drawConsole();
}
