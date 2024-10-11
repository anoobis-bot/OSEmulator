#pragma once
#include "Process.h"
#include <thread>

class Core
{
public:
	Core(int tickDuration, int coreID);
	void attachProcess(std::shared_ptr<Process> process);
	std::shared_ptr<Process> getAttachedProcess();
	void run();

	bool hasAttachedProcess();
	bool isAvailable();

private:
	std::mutex mtx;

	int tickDuration;
	int coreID;
	std::shared_ptr<Process> attachedProcess;
	std::thread workerThread;
};

