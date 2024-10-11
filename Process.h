#pragma once
#include <chrono>
#include <mutex>

#include "TypedefRepo.h"

class Process
{
public:
	Process(String processName);
	//Process(String processName, int hours);
	String getName() const;
	int getCurrentInstruction() const;
	int getTotalInstructions() const;
	//String getCreationTime() const;
	std::chrono::system_clock::time_point getCreationTime() const;
	String getFormattedTime();

	void run();

	enum state
	{
		READY,
		RUNNING,
		FINISHED
	};

	state getState();

	void finishState();

	void setCoreID(int coreID);
	int getCoreID();

private:
	String processName;
	std::chrono::system_clock::time_point creationTime;
	int currentInstruction;
	int totalInstructions;

	String printStatement;

	state processState;
	int inCoreID;

	std::mutex mtx;
	
};