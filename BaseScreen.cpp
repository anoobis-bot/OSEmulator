#include "BaseScreen.h"
#include "utils.h"
#include "ConsoleManager.h"
#include <iostream>
#include <chrono>
#include <iomanip>
#include <ctime>

BaseScreen::BaseScreen(String processName, std::shared_ptr<String> attachedProcess)
    : AConsole(processName), attachedProcess(attachedProcess) {}

void BaseScreen::onEnabled() {
    refreshed = true;
}

void BaseScreen::display() {
    system("cls");  
    printProcessInfo(); 
}

void BaseScreen::process() {
    auto consoleManager = ConsoleManager::getInstance();
    String command;
    while (true) {
        String command;
        while (true) {
            std::cout << "root\\";
            std::getline(std::cin, command);

            if (toLowerCase(command) == "exit") {
                consoleManager->switchConsole(MAIN_CONSOLE);
            }
            handleCommand(command);
        }
    }
}

void BaseScreen::handleCommand(String command) {
    std::string formattedInput = toLowerCase(command);  // Lowercase input for comparison

    if (formattedInput == "clear") {
        system("cls");
        printProcessInfo();  
    }
    else {
        printMsgNewLine("Invalid command. Available commands: clear, exit.");
    }
}

void BaseScreen::printProcessInfo() const {
    auto currentTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::tm localTime;
    localtime_s(&localTime, &currentTime);

    std::cout << "Process Name: " << name << std::endl;
    std::cout << "Instruction Line: 0 / 100" << std::endl; 
    std::cout << "Screen Created At: "
        << std::put_time(&localTime, "%m/%d/%Y, %I:%M:%S %p") << std::endl;
}
