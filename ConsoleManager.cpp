#include "ConsoleManager.h"
#include <iostream>  // Make sure this line is included

ConsoleManager* ConsoleManager::sharedInstance = nullptr;

ConsoleManager* ConsoleManager::getInstance() {
    return sharedInstance;
}

void ConsoleManager::initialize() {
    sharedInstance = new ConsoleManager();
}

void ConsoleManager::destroy() {
    delete sharedInstance;
}

ConsoleManager::ConsoleManager() : currentConsole(nullptr), previousConsole(nullptr) {}

ConsoleManager::~ConsoleManager() {}

void ConsoleManager::drawConsole() {
    if (currentConsole) {
        currentConsole->display();
    }
}

void ConsoleManager::switchConsole(String consoleName) {
    auto it = consoleTable.find(consoleName);
    if (it != consoleTable.end()) {
        previousConsole = currentConsole;
        currentConsole = it->second;
        currentConsole->onEnabled();
    }
}

void ConsoleManager::switchToScreen(String screenName) {
    if (isScreenRegistered(screenName)) {
        previousConsole = currentConsole;
        currentConsole = consoleTable[screenName];
        currentConsole->onEnabled();
    } else {
        std::cout << "Screen does not exist, create one first." << std::endl;  // Use std::cout and std::endl
    }
}

void ConsoleManager::registerScreen(std::shared_ptr<BaseScreen> screenRef) {
    consoleTable[screenRef->getName()] = screenRef;
}

void ConsoleManager::unregisterScreen(String screenName) {
    consoleTable.erase(screenName);
}

void ConsoleManager::returnToPreviousConsole() {
    if (previousConsole) {
        currentConsole = previousConsole;
    }
}

bool ConsoleManager::isScreenRegistered(const String& screenName) {
    return consoleTable.find(screenName) != consoleTable.end();
}
