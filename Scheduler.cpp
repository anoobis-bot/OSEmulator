#include "Scheduler.h"

#include <memory>

Scheduler* Scheduler::sharedInstance = nullptr;
std::mutex Scheduler::mtx;

void Scheduler::initialize(ScheduleAlgo scheduleAlgo, int numCores, int tickDuration) {
    if (!sharedInstance)
    {
        sharedInstance = new Scheduler(scheduleAlgo, numCores, tickDuration);
    }
}

// debugging
int Scheduler::getSize()
{
    return this->readyQueue.size();
}
int Scheduler::numCores()
{
    return this->cores.size();
}
void Scheduler::printProcesses()
{
	for (std::shared_ptr<Process> process : readyQueue)
	{
        std::cout << process->getName() << '\n';
	}
}
// ---- end of debugging methods

Scheduler* Scheduler::getInstance()
{
    return sharedInstance;
}


Scheduler::Scheduler(ScheduleAlgo scheduleAlgo, int numCores, int tickDuration) : isRunning(true)
{
    
    // initialize the cores
    for (int i = 0; i < numCores; i++)
    {
        this->cores.push_back(new Core(tickDuration, i));
    }

    this->scheduleAlgo = scheduleAlgo;
    this->tickDuration = tickDuration;

    // Start the thread in the constructor
    this->workerThread = std::thread(&Scheduler::run, this);
    this->workerThread.detach();

    //debugging
    //this->addProcess(std::make_shared<Process>("process1000", 24));
}

void Scheduler::firstComeFirstServe()
{
    // sort the ready queue
    this->sortReadyQueue();

    
	// check each core if tapos na yung process. assign empty cores.
    for (Core* core : cores)
    {
        if (core->hasAttachedProcess() && core->getAttachedProcess()->getState() != Process::FINISHED)
        {
            continue;
        }
		if (this->readyQueue.empty())
		{
            continue;
		}

        core->attachProcess(this->getFirstProcess());
        this->getFirstProcess()->setCoreID(core->getCoreID());
        this->getFirstProcess()->runningState();
        this->removeFirstProcess();

    }
}

void Scheduler::run()
{
    while (true)
    {
        if (this->scheduleAlgo == FCFS)
        {
            firstComeFirstServe();
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(this->tickDuration));
    }

}

void Scheduler::addNewProcess(std::shared_ptr<Process> process)
{
    this->readyQueue.push_back(process);
    this->allProcesses.push_back(process);
}

void Scheduler::reAddProcess(std::shared_ptr<Process> process)
{
    std::lock_guard<std::mutex> lock(mtx); // Lock the mutex before modifying the vector
    this->readyQueue.push_back(process);
}


std::shared_ptr<Process> Scheduler::getFirstProcess()
{ 
	return this->readyQueue.front();
}

void Scheduler::removeFirstProcess()
{
    readyQueue.erase(readyQueue.begin());
}

void Scheduler::sortReadyQueue()
{
    std::sort(this->readyQueue.begin(), this->readyQueue.end(), [](std::shared_ptr<Process> a, std::shared_ptr<Process> b) {
        return a->getCreationTime() < b->getCreationTime();
    });
}

std::vector<std::shared_ptr<Process>>& Scheduler::getAllProcess()
{
    return this->allProcesses;
}
