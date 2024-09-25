#include "ConsoleManager.h"
#include <iostream>  

#include "MainScreen.h"

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

ConsoleManager::ConsoleManager() {
    this->running = true;

    const std::shared_ptr<MainScreen> mainConsole = std::make_shared<MainScreen>();
    // const std::shared_ptr<MarqueeConsole> marqueeConsole = std::make_shared<MarqueeConsole>();
    // const std::shared_ptr<SchedulingConsole> schedulingConsole = std::make_shared<SchedulingConsole>();
    // const std::shared_ptr<MemorySimulationConsole> memoryConsole = std::make_shared<MemorySimulationConsole>();

    consoleTable[MAIN_CONSOLE] = mainConsole;
    //this->consoleTable[MARQUEE_CONSOLE] = marqueeConsole;
    //this->consoleTable[SCHEDULING_CONSOLE] = schedulingConsole;
    //this->consoleTable[MEMORY_CONSOLE] = memoryConsole;

    switchConsole(MAIN_CONSOLE);
}

//ConsoleManager::ConsoleManager() : currentConsole(nullptr) {}

//ConsoleManager::~ConsoleManager() {}

void ConsoleManager::drawConsole() const {
    /*
    if (currentConsole) {
        currentConsole->display();
    }
    */

    if (currentConsole != nullptr) {
        currentConsole->display();
    }
    else {
        std::cerr << "There is no assigned console. Please check." << std::endl;
    }
}

void ConsoleManager::process() const {
    if (currentConsole != nullptr) {
        currentConsole->process();
    }
    else {
        std::cerr << "There is no assigned console. Please check." << std::endl;
    }
}

void ConsoleManager::switchConsole(String consoleName) {
    /*
    auto switchConsole = consoleTable.find(consoleName);
    if (switchConsole != consoleTable.end()) {
        system("cls");
        previousConsole = currentConsole;
        //currentConsole = switchConsole->second;
        currentConsole = consoleTable[consoleName];
        currentConsole->onEnabled();
    }
    else {
        std::cerr << "Console name " << consoleName << " not found. Was it initialized?" << std::endl;
    }
    */

    if (consoleTable.contains(consoleName)) {
        system("cls");
        previousConsole = currentConsole;
        currentConsole = consoleTable[consoleName];
        currentConsole->onEnabled();
    }
    else {
        std::cerr << "Console name " << consoleName << " not found. Was it initialized?" << std::endl;
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
    if (previousConsole != nullptr) {
        currentConsole = previousConsole;
        previousConsole = nullptr;
        system("cls");
        currentConsole->onEnabled();
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
    //return this->consoleTable.find(screenName) != this->consoleTable.end();
    return consoleTable.contains(screenName);
}

void ConsoleManager::exitApplication() {
    running = false;
}

bool ConsoleManager::isRunning() const {
    return running;
}
