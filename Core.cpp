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
	this->attachedProcess->run();
}

