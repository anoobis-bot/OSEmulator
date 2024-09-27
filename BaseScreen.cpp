#include "BaseScreen.h"
#include "ConsoleManager.h"
#include <iostream>
#include "utils.h"

BaseScreen::BaseScreen(std::shared_ptr<Process> process, String processName) : AConsole(processName)
{
	this->attachedProcess = process;

	auto currentTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	std::tm timeInfo;
	localtime_s(&timeInfo, &currentTime);
	char buffer[100];
	std::strftime(buffer, sizeof(buffer), "%m/%d/%Y %I:%M:%S%p", &timeInfo);
	this->screenCreationTime = buffer;
}

void BaseScreen::onEnabled()
{
	process();
}

void BaseScreen::process()
{
	if (this->refreshed == false)
	{
		this->refreshed = true;
		this->printProcessInfo();
	}

	std::string command;
	while (command != "exit")
	{
		std::cout << "root:\>";
		std::getline(std::cin, command);

		if (command == "clear" || command == "cls")
		{
			system("cls");
			this->printProcessInfo();
		}
		else if (command == "exit")
		{
			this->refreshed = false;
			ConsoleManager::getInstance()->returnToPreviousConsole();
		}
		else
		{
			printMsgNewLine("Command not found.");
			printMsgNewLine("");
		}
	}
}

void BaseScreen::display()
{
	this->printProcessInfo();
}

void BaseScreen::printProcessInfo() const
{

	std::cout << "Process Name: " << name << std::endl;
	std::cout << "Instruction Line: 0 / 100" << std::endl;
	std::cout << "Screen Created At: "
		<< this->screenCreationTime << std::endl;

}