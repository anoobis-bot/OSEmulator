#pragma once
#include "Process.h"

class Core
{
public:
	Core(int tickDuration);
	void attachProcess(Process *process);
	void run();

private:
	int tickDuration;
	Process *attachedProcess;
};

