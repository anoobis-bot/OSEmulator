#include "Process.h"
#include <iostream>
#include <filesystem> 

#include "PrintCommand.h"

Process::Process(String processName, int id, int totalInstructions ,PrintCommand command) : creationTime(std::chrono::system_clock::now()), command(command)
{
	this->processName = processName;
	this->processState = READY;
	this->id = id;
	this->inCoreID = -1;
	this->command = command;
	this->totalInstructions = totalInstructions;
	this->currentInstruction = 0;

	this->openLogFile();
}

void Process::run()
{
	std::lock_guard<std::mutex> lock(mtx); // Lock the mutex before modifying the processState
	// print something
	this->command.run();
	this->logPrintCommand(this->command.getToPrint());
	
	this->currentInstruction = currentInstruction + 1;
	if (currentInstruction >= totalInstructions)
	{
		this->processState = FINISHED;
		this->closeLogFile();
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

void Process::openLogFile() {

	const std::string directory = "Process Text"; // directory file

	if (!std::filesystem::exists(directory)) {
		std::filesystem::create_directory(directory);
	}


	std::string filename = directory + "/" + processName + ".txt";


	logFile.open(filename);
	if (!logFile.is_open()) {
		std::cerr << "Failed to open log file for Process " << id << ".\n";
	}
	else {
		logFile << "Process: " << processName << "\nLogs:\n\n";
	}
}

void Process::logPrintCommand(const std::string& command) {
	std::time_t timestamp = std::time(nullptr);
	std::tm timeInfo;
	localtime_s(&timeInfo, &timestamp);
	char timeBuffer[25];
	std::strftime(timeBuffer, sizeof(timeBuffer), "%m/%d/%Y %I:%M:%S%p", &timeInfo);
	logFile << "(" << timeBuffer << ") Core: " << inCoreID << " \"" << command << "\"\n";
}

void Process::closeLogFile() {
	logFile.close();
}