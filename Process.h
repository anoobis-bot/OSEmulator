#pragma once
#include <chrono>
#include <mutex>

#include "PrintCommand.h"
#include "TypedefRepo.h"
#include <fstream>

class Process
{
public:
	Process(String processName, int id, int totalInstructions, PrintCommand command, size_t memoryRequired);
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

	size_t getMemoryRequired();
	bool isInMemory();
	void setInMemory(bool inMemory);

	//Time::time_point getTimeInMemory();
	std::vector<size_t> getAllocatedFrames();
	void clearAllocatedFrames();

	void setStartingMemIndex(size_t index);
	size_t getStartingMemIndex();

	void addAllocatedFrame(size_t frameIndex);

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

	size_t memoryRequired;
	bool inMemory;
	std::vector<size_t> allocatedFrames;
	size_t startingMemIndex;
	//Time::time_point timeInMemory;

	std::mutex mtx;

	PrintCommand command;
};
