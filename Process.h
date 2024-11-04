#pragma once
#include <chrono>
#include <mutex>

#include "PrintCommand.h"
#include "TypedefRepo.h"
#include <fstream>

class Process
{
public:
	Process(String processName, int id, int totalInstructions, PrintCommand command);
	String getName() const;
	int getCurrentInstruction();
	int getTotalInstructions();
	std::chrono::system_clock::time_point getCreationTime() const;
	String getFormattedTime();
	int getID();

	void run();

	enum state
	{
		READY,
		RUNNING,
		FINISHED
	};

	state getState();

	void readyState();
	void runningState();
	void finishState();

	void setCoreID(int coreID);
	int getCoreID();
	void openLogFile();
	void logPrintCommand(const std::string& command);
	void closeLogFile();

	void printInfo();

private:
	String processName;
	std::chrono::system_clock::time_point creationTime;
	int currentInstruction;
	int totalInstructions;

	std::ofstream logFile;
	std::vector<std::string> printCommands;

	String printStatement;

	state processState;
	int id;
	int inCoreID;

	std::mutex mtx;

	PrintCommand command;
};
