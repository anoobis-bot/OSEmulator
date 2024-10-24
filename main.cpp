#include "ConsoleManager.h"
#include <iostream>
#include "MainConsole.h"
#include "Scheduler.h"
#include "TypedefRepo.h"
#include "Process.h"
#include "PrintCommand.h"

int main()
{
	ConsoleManager::initialize();
	Scheduler::initialize(RR, 5, 4, 100000);

	bool running = true;
	while (running)
	{
		ConsoleManager::getInstance()->process();
		ConsoleManager::getInstance()->drawConsole();
		running = ConsoleManager::getInstance()->isRunning();
	}

	ConsoleManager::destroy();
	return 0;
}