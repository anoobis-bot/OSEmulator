#include "Process.h"
#include <iostream>

Process::Process()
{
}

std::string Process::getName() const {
	return processName;
}

int Process::getCurrentInstruction() const {
	return currentInstruction;
}

int Process::getTotalInstructions() const {
	return totalInstructions;
}

std::string Process::getCreationTime() const {
	auto creationTimeT = std::chrono::system_clock::to_time_t(creationTime);
	struct tm timeinfo;
	localtime_s(&timeinfo, &creationTimeT);
	char buffer[100];
	std::strftime(buffer, sizeof(buffer), "%m/%d/%Y %I:%M:%S%p", &timeinfo);
	return buffer;
}