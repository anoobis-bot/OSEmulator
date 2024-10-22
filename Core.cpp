#include "Core.h"

Core::Core(int tickDuration, int coreID, ScheduleAlgo scheduleAlgo, unsigned int quantumCycleMax)
{
	this->tickDuration = tickDuration;
	this->coreID = coreID;
	this->quantumCycle = 0;
	this->quantumCycleMax = quantumCycleMax;
	this->attachedProcess = nullptr;
	//this->attachedProcess = nullptr;

	// Start the thread in the constructor
	if (scheduleAlgo == FCFS)
	{
		this->workerThread = std::thread(&Core::runFCFS, this);
	}
	else if (scheduleAlgo == RR)
	{
		this->workerThread = std::thread(&Core::runRR, this);
	}

	this->workerThread.detach();
}

void Core::attachProcess(std::shared_ptr<Process> process)
{
	std::lock_guard<std::mutex> lock(this->mtx);
	this->attachedProcess = process;
}

std::shared_ptr<Process> Core::getAttachedProcess()
{
	return this->attachedProcess;
}



void Core::runFCFS()
{
	while(true)
	{
		this->mtx.lock();
		if (this->hasAttachedProcess())
		{
			if (this->attachedProcess->getState() != Process::FINISHED)
			{
				this->attachedProcess->run();
			}
		}
		this->mtx.unlock();

		//std::this_thread::sleep_for(std::chrono::milliseconds(tickDuration));
	}

}

void Core::runRR()
{
	while (true)
	{
		this->mtx.lock();
		if (this->hasAttachedProcess())
		{
			if (!this->finishedQuantumCycle()
				&& this->attachedProcess->getState() == Process::RUNNING)
			{
				this->attachedProcess->run();
				this->quantumCycle = this->quantumCycle + 1;
			}
		}
		this->mtx.unlock();

		//std::this_thread::sleep_for(std::chrono::milliseconds(tickDuration));
	}
}

void Core::resetQuantumCycle()
{
	std::lock_guard<std::mutex> lock(this->mtx);
	this->quantumCycle = 0;
}

bool Core::finishedQuantumCycle()
{
	if (this->quantumCycle >= this->quantumCycleMax)
	{
		return true;
	}

	return false;
	//return this->quantumCycle >= this->quantumCycleMax;
}



bool Core::hasAttachedProcess()
{
	//std::lock_guard<std::mutex> lock(this->mtx);
	if (this->attachedProcess)
	{
		return true;
	}

	return false;
}

int Core::getCoreID()
{
	return this->coreID;
}
