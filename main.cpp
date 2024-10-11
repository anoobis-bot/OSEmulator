#include "ConsoleManager.h"
#include <iostream>
#include "MainConsole.h"
#include "Scheduler.h"
#include "TypedefRepo.h"

int main()
{
	ConsoleManager::initialize();
	Scheduler::initialize(Scheduler::FCFS, 4, 300);

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