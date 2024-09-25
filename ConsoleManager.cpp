#include "ConsoleManager.h"
#include "MainScreen.h"
#include <iostream>  

ConsoleManager* ConsoleManager::sharedInstance = nullptr;

ConsoleManager* ConsoleManager::getInstance() {
    if (!sharedInstance) {
        sharedInstance = new ConsoleManager();
        sharedInstance->initialize();
    }
    return sharedInstance;
}

void ConsoleManager::initialize() {
    sharedInstance = new ConsoleManager(); 
    std::shared_ptr<BaseScreen> mainScreen = std::make_shared<MainScreen>();
    sharedInstance->registerScreen(mainScreen); 
    sharedInstance->currentConsole = mainScreen;
}

void ConsoleManager::destroy() {
    delete sharedInstance;
}

ConsoleManager::ConsoleManager() : currentConsole(nullptr) {}

ConsoleManager::~ConsoleManager() {}

void ConsoleManager::drawConsole() {
    if (currentConsole) {
        currentConsole->display();
    }
}

void ConsoleManager::switchConsole(String consoleName) {
    auto switchConsole = consoleTable.find(consoleName);
    if (switchConsole != consoleTable.end()) {
        previousConsole = currentConsole;
        currentConsole = switchConsole->second;
        currentConsole->onEnabled();
    }
}

void ConsoleManager::switchToScreen(String screenName) {
    if (isScreenRegistered(screenName)) {
        system("cls");
        previousConsole = currentConsole; 
        currentConsole = consoleTable[screenName];  
        currentConsole->onEnabled();  
    }
    else {
        std::cout << "Screen does not exist, create one first." << std::endl;
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
        previousConsole = nullptr;
        system("cls");
        currentConsole->onExecute();
    }
    else {
        std::cerr << "No previous screen to return to." << std::endl;
    }
}

void ConsoleManager::createProcessScreen(String processName) {
    auto processScreen = std::make_shared<BaseScreen>(processName, std::make_shared<String>(processName));

    registerScreen(processScreen);  

    switchToScreen(processScreen->getName());
}


bool ConsoleManager::isScreenRegistered(const String& screenName) {
    return consoleTable.find(screenName) != consoleTable.end();
}

std::shared_ptr<AConsole> ConsoleManager::getCurrentConsole() {
    return currentConsole; 
}