#include "Core.h"

Core::Core(int tickDuration)
{
	this->tickDuration = tickDuration;
	this->attachedProcess = nullptr;
}

void Core::attachProcess(Process* process)
{
	this->attachedProcess = process;
}


void Core::run()
{
	while(true)
	{
		if (!attachedProcess)
		{
			this->attachedProcess->run();
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(tickDuration));
	}

}

