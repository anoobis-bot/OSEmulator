#include <iostream>
#include "ConsoleManager.h"
#include "MainScreen.h"
#include "utils.h"

int main() {
    ConsoleManager::initialize();
    auto consoleManager = ConsoleManager::getInstance();

    MainScreen mainScreen;
    mainScreen.onExecute(); 

    printMsg("Program Terminated");
    ConsoleManager::destroy();  
    return 0;
}
