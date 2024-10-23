#pragma once
#include "Process.h"
#include <thread>

#include "ScheduleAlgo.h"

class Core
{
public:
	Core(unsigned int tickDelay, int coreID, ScheduleAlgo scheduleAlgo, unsigned int quantumCycleMax);
	void attachProcess(std::shared_ptr<Process> process);
	std::shared_ptr<Process> getAttachedProcess();
	void runFCFS();
	void runRR();

	bool hasAttachedProcess();

	bool finishedQuantumCycle();
	void resetQuantumCycle();

	int getCoreID();

	void resetTickDelay();

private:
	std::mutex mtx;

	unsigned int tickDelay;
	unsigned int currentTickDelay;
	int coreID;

	void incrementTickDelay();

	void detachProcess();

	unsigned int quantumCycle;
	unsigned int quantumCycleMax;
	std::shared_ptr<Process> attachedProcess;
	std::thread workerThread;
};

