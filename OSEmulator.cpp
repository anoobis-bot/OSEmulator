#include <iostream>
#include "ConsoleManager.h"
#include "MainScreen.h"
#include "utils.h"

int main() {
    ConsoleManager::initialize();
    bool running = true;

    while (running) {
        ConsoleManager::getInstance()->process();
        ConsoleManager::getInstance()->drawConsole();

        running = ConsoleManager::getInstance()->isRunning();
    }


    printMsg("Program Terminated");
    ConsoleManager::destroy();  
    return 0;
}
