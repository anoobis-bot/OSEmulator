#include "Process.h"
#include <iostream>

#include "PrintCommand.h"

Process::Process(String processName, int totalInstructions ,PrintCommand command) : creationTime(std::chrono::system_clock::now())
{
	this->processName = processName;
	this->processState = READY;
	this->inCoreID = -1;
	this->command = command;
	this->totalInstructions = totalInstructions;
	this->currentInstruction = 0;
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
	this->command.run();
	this->currentInstruction = currentInstruction + 1;

	if (currentInstruction >= totalInstructions)
	{
		this->processState = FINISHED;
	}
	
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

Process::state Process::getState()
{
	std::lock_guard<std::mutex> lock(mtx); // Lock the mutex before modifying the vector
	return this->processState;
}

void Process::runningState()
{
	std::lock_guard<std::mutex> lock(mtx); // Lock the mutex before modifying the vector
	this->processState = RUNNING;
}

void Process::readyState()
{
	std::lock_guard<std::mutex> lock(mtx); // Lock the mutex before modifying the vector
	this->processState = READY;
}

void Process::finishState()
{
	std::lock_guard<std::mutex> lock(mtx); // Lock the mutex before modifying the vector
	this->processState = FINISHED;
}



String Process::getFormattedTime() {
	auto creationTimeT = std::chrono::system_clock::to_time_t(this->creationTime);
	struct tm timeinfo;
	localtime_s(&timeinfo, &creationTimeT);
	char buffer[100];
	std::strftime(buffer, sizeof(buffer), "(%m/%d/%Y %I:%M:%S%p)", &timeinfo);
	return buffer;
}