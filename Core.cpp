#include "Core.h"

Core::Core(int tickDuration, int coreID) 
{
	this->tickDuration = tickDuration;
	this->coreID = coreID;
	this->attachedProcess = nullptr;
	//this->attachedProcess = nullptr;

	// Start the thread in the constructor
	this->workerThread = std::thread(&Core::run, this);
	this->workerThread.detach();
}

void Core::attachProcess(std::shared_ptr<Process> process)
{
	this->attachedProcess = process;
}

std::shared_ptr<Process> Core::getAttachedProcess()
{
	return this->attachedProcess;
}



void Core::run()
{
	while(true)
	{
		if (this->hasAttachedProcess())
		{
			if (this->attachedProcess->getState() != Process::FINISHED)
			{
				this->attachedProcess->run();
			}
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(tickDuration));
	}

}

bool Core::hasAttachedProcess()
{
	std::lock_guard<std::mutex> lock(this->mtx);
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
