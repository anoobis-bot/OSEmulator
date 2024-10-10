#include "Process.h"
#include <iostream>

Process::Process(String processName) : creationTime(std::chrono::system_clock::now())
{
	this->processName = processName;
}
//debugging
//Process::Process(String processName, int hours) : creationTime(std::chrono::system_clock::now() + std::chrono::hours(hours))
//{
//	this->processName = processName;
//}

void Process::run()
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

std::chrono::system_clock::time_point Process::getCreationTime() const
{
	return this->creationTime;
}

//std::string Process::getCreationTime() const {
//	auto creationTimeT = std::chrono::system_clock::to_time_t(creationTime);
//	struct tm timeinfo;
//	localtime_s(&timeinfo, &creationTimeT);
//	char buffer[100];
//	std::strftime(buffer, sizeof(buffer), "%m/%d/%Y %I:%M:%S%p", &timeinfo);
//	return buffer;
//}