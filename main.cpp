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
	Scheduler::initialize(Scheduler::FCFS, 4, 300);

	for (int i = 0; i < 10; ++i) {
		std::string processName = "Process_" + std::to_string(i + 1).substr(0, 2);
		auto process = std::make_shared<Process>(processName, i, 100, PrintCommand());
		Scheduler::getInstance()->addNewProcess(process);
	}

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