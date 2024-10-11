#include "Process.h"
#include <iostream>

Process::Process(String processName) : creationTime(std::chrono::system_clock::now())
{
	this->processName = processName;
	this->processState = READY;
	this->inCoreID = -1;
}
//debugging
//Process::Process(String processName, int hours) : creationTime(std::chrono::system_clock::now() + std::chrono::hours(hours))
//{
//	this->processName = processName;
//}

void Process::run()
{
	std::lock_guard<std::mutex> lock(mtx); // Lock the mutex before modifying the processState
	// print something
	// TODO print it into text file if finished
}

void Process::setCoreID(int coreID)
{
	std::lock_guard<std::mutex> lock(mtx);
	this->inCoreID = coreID;
}

int Process::getCoreID()
{
	return this->inCoreID;
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

//String Process::getFormattedTime()
//{
//	std::time_t time = std::chrono::system_clock::to_time_t(this->creationTime);
//
//	std::tm* localTime = std::localtime_s(&time);
//
//	std::ostringstream oss;
//	oss << "(" << std::put_time(localTime, "%m/%d/%Y %I:%M:%S%p") << ")";
//
//	// Return the formatted time as a string
//	return oss.str();
//}

Process::state Process::getState()
{
	std::lock_guard<std::mutex> lock(mtx); // Lock the mutex before modifying the vector
	return this->processState;
}

String Process::getFormattedTime() {
	auto creationTimeT = std::chrono::system_clock::to_time_t(this->creationTime);
	struct tm timeinfo;
	localtime_s(&timeinfo, &creationTimeT);
	char buffer[100];
	std::strftime(buffer, sizeof(buffer), "(%m/%d/%Y %I:%M:%S%p)", &timeinfo);
	return buffer;
}