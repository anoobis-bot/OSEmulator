#pragma once
#include "Process.h"
#include <thread>

#include "ScheduleAlgo.h"

class Core
{
public:
	Core(int tickDuration, int coreID, ScheduleAlgo scheduleAlgo, unsigned int quantumCycleMax);
	void attachProcess(std::shared_ptr<Process> process);
	std::shared_ptr<Process> getAttachedProcess();
	void runFCFS();
	void runRR();

	bool hasAttachedProcess();

	int getCoreID();

private:
	std::mutex mtx;

	int tickDuration;
	int coreID;
	unsigned int quantumCycle;
	unsigned int quantumCycleMax;
	std::shared_ptr<Process> attachedProcess;
	std::thread workerThread;
};

