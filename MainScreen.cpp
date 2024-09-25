#include <iostream>
#include <chrono>
#include <thread>
#include <sstream>
#include "utils.h"
#include "ConsoleManager.h"
#include "MainScreen.h"

MainScreen::MainScreen(): AConsole("MAIN_MENU") {
}

MainScreen::~MainScreen() {

}

void MainScreen::onExecute() {
    display();
    process();
}

void MainScreen::onEnabled() {

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
        std::cout << "root:\\> ";
        std::getline(std::cin, command);

        if (toLowerCase(command) == "exit") {
            break;
        }

        handleCommand(command); 
        std::cout << std::endl;
    }
}

void MainScreen::handleCommand(String command) {
    std::string formattedInput = toLowerCase(command);  // Lowercase input for comparison
    auto consoleManager = ConsoleManager::getInstance();

    if (formattedInput.substr(0, 6) == "screen") {
        auto [screenCommand, screenName] = parseScreenCommand(formattedInput);

        // validate if the screenCommand is either "-r" or "-s"
        if (screenCommand != "-r" && screenCommand != "-s") {
            printMsgNewLine("Error: Missing or invalid arguments. Use 'screen -r <name>' or 'screen -s <name>'.");
        }
        else if (screenName.empty()) {
            // ensure that a screen name is provided
            printMsgNewLine("Error: Missing process name. Use 'screen -r <name>' or 'screen -s <name>'.");
        }
        else {
            if (screenCommand == "-r") {
                consoleManager->switchToScreen(screenName);
            }
            else if (screenCommand == "-s") {
                auto screen = std::make_shared<BaseScreen>(screenName, std::make_shared<String>(screenName));
                consoleManager->registerScreen(screen);
                consoleManager->switchToScreen(screenName);
            }
        }
    }

    else if (formattedInput == "clear") {
        std::cout << "\033[2J\033[1;1H";  // Clear the screen
        printHeader("3D_CSOPESY.txt");
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
