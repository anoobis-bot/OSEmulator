#include "ConsoleManager.h"
#include <iostream>
#include "MainConsole.h"
#include "TypedefRepo.h"

int main()
{
	ConsoleManager::initialize();

	bool running = true;
	while (ConsoleManager::getInstance()->isRunning())
	{
		ConsoleManager::getInstance()->process();
		ConsoleManager::getInstance()->drawConsole();

	}
	ConsoleManager::destroy();
	std::cout << "Exiting the program...";
	return 0;
}