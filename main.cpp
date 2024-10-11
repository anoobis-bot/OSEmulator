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

	std::cout << "Generating dummy processes. . .\n";
	for (int i = 0; i < 10; ++i) {
		std::string processName = "Process_" + std::to_string(i + 1).substr(0, 2);
		auto process = std::make_shared<Process>(processName, i, 100, PrintCommand());
		Scheduler::getInstance()->addNewProcess(process);
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
	std::cout << "Generated \n";

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