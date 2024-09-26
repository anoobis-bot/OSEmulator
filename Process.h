#pragma once
#include <chrono>
#include "TypedefRepo.h"

class Process
{
public:
	Process();
	String getName() const;
	int getCurrentInstruction() const;
	int getTotalInstructions() const;
	String getCreationTime() const;

private:
	String processName;
	std::chrono::system_clock::time_point creationTime;
	int currentInstruction;
	int totalInstructions;
};