#include "BaseScreen.h"
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
    printProcessInfo();
}

void BaseScreen::process() {
    // Custom process logic for the screen
}

void BaseScreen::printProcessInfo() {
    auto currentTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::tm localTime;
    localtime_s(&localTime, &currentTime);

    std::cout << "Process Name: " << name << std::endl;
    std::cout << "Instruction Line: 0 / 100" << std::endl;  // Placeholder values
    std::cout << "Screen Created At: "
        << std::put_time(&localTime, "%m/%d/%Y, %I:%M:%S %p") << std::endl;
}
