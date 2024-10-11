#include "Core.h"

std::mutex Core::mtx;

Core::Core(int tickDuration, int coreID) : attachedProcess(nullptr)
{
	this->tickDuration = tickDuration;
	this->coreID = coreID;
	//this->attachedProcess = nullptr;

	// TODO ayos threadding memory exception
	// Start the thread in the constructor
	//this->workerThread = std::thread(&Core::run, this);
	//this->workerThread.detach();
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
	std::lock_guard<std::mutex> lock(mtx);
	if (this->attachedProcess)
	{
		return true;
	}

	return false;
}

bool Core::isAvailable()
{
	std::lock_guard<std::mutex> lock(mtx);
	if (!this->hasAttachedProcess())
	{
		return true;
	}
	if (this->attachedProcess->getState() == Process::FINISHED)
	{
		return true;
	}

	return false;
}

