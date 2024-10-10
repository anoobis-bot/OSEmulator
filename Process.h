#pragma once
#include <chrono>
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

	void run();

private:
	String processName;
	std::chrono::system_clock::time_point creationTime;
	int currentInstruction;
	int totalInstructions;

	String printStatement;
	
};