#include "Scheduler.h"
#include "ConsoleManager.h"
#include <iomanip>
#include <memory>
#include <random>

Scheduler* Scheduler::sharedInstance = nullptr;
std::mutex Scheduler::mtx;

// Initialize the shared instance of the Scheduler
void Scheduler::initialize(ScheduleAlgo scheduleAlgo, unsigned int quantumCycleMax, int numCores,
    double delayPerExec, unsigned minInstructions, unsigned maxInstruction, unsigned batchProcessFreq) {
    if (!sharedInstance)
    {
        sharedInstance = new Scheduler(scheduleAlgo, quantumCycleMax, numCores, delayPerExec, minInstructions, maxInstruction, batchProcessFreq);
    }
}

// Debugging method to get the size of the ready queue
int Scheduler::getSize()
{
    return this->readyQueue.size();
}

// Get the number of cores
int Scheduler::numCores()
{
    return this->cores.size();
}

// Print the names of all processes in the ready queue
void Scheduler::printProcesses()
{
    for (std::shared_ptr<Process> process : readyQueue)
    {
        std::cout << process->getName() << '\n';
    }
}
// ---- end of debugging methods

// Get the singleton instance of the Scheduler
Scheduler* Scheduler::getInstance()
{
    return sharedInstance;
}

// Constructor for Scheduler
Scheduler::Scheduler(ScheduleAlgo scheduleAlgo, unsigned int quantumCycleMax, int numCores,
    double delayPerExec, unsigned minInstructions, unsigned maxInstruction, unsigned batchProcessFreq) : isRunning(true)
{
    // Initialize the cores
    for (int i = 0; i < numCores; i++)
    {
        this->cores.push_back(new Core(delayPerExec, i, scheduleAlgo, quantumCycleMax));
    }

    this->scheduleAlgo = scheduleAlgo;
    this->quantumCycleMax = quantumCycleMax;
	this->delayPerExec = delayPerExec;
	this->minInstructions = minInstructions;
	this->maxInstructions = maxInstruction;
	this->batchProcessFreq = batchProcessFreq;

    // Start the worker thread
    this->workerThread = std::thread(&Scheduler::run, this);
    this->workerThread.detach();

    // Debugging (uncomment to add a test process)
    // this->addProcess(std::make_shared<Process>("process1000", 24));
}

// First-Come, First-Serve scheduling method
void Scheduler::firstComeFirstServe()
{
    // Check each core for attached processes and assign empty cores.
    for (Core* core : cores)
    {
        if (core->hasAttachedProcess() && core->getAttachedProcess()->getState() == Process::RUNNING)
        {
            continue;
        }
        if (this->readyQueue.empty())
        {
            continue;
        }

        // Attach the first process to the core
        core->attachProcess(this->getFirstProcess());
        core->resetTickDelay();
        this->getFirstProcess()->setCoreID(core->getCoreID());
        this->getFirstProcess()->runningState();
        this->removeFirstProcess();
    }
}

// Round Robin scheduling method
void Scheduler::roundRobin()
{
    // Check each core for attached processes and assign empty cores.
    for (Core* core : cores)
    {
        if (core->hasAttachedProcess() && core->getAttachedProcess()->getState() != Process::FINISHED && !core->finishedQuantumCycle())
        {
            continue;
        }
        if (this->readyQueue.empty())
        {
            if (core->hasAttachedProcess() && core->getAttachedProcess()->getState() == Process::RUNNING)
            {
                core->resetQuantumCycle();
            }
            continue;
        }

        mtx.lock(); // Lock mutex for thread safety
        if (core->hasAttachedProcess() && core->getAttachedProcess()->getState() != Process::FINISHED && core->finishedQuantumCycle())
        {
            core->getAttachedProcess()->setCoreID(-1);
            core->getAttachedProcess()->readyState();
            reAddProcess(core->getAttachedProcess());
        }
        core->attachProcess(this->getFirstProcess());
        core->resetQuantumCycle();
        core->resetTickDelay();
        this->getFirstProcess()->setCoreID(core->getCoreID());
        this->getFirstProcess()->runningState();
        this->removeFirstProcess();
        mtx.unlock(); // Unlock mutex
    }
}

// Main run loop for the scheduler
void Scheduler::run()
{
    while (true)
    {
        if (this->scheduleAlgo == FCFS)
        {
            firstComeFirstServe();
        }
        else if (this->scheduleAlgo == RR)
        {
            roundRobin();
        }
        std::this_thread::sleep_for(std::chrono::duration<double>(this->delayPerExec));
    }
}

// Add a new process to the ready queue
void Scheduler::addNewProcess(std::shared_ptr<Process> process)
{
    std::lock_guard<std::mutex> lock(mtx); // Uncomment for thread safety
    this->readyQueue.push_back(process);
    this->allProcesses.push_back(process);
}

// Re-add a process back to the ready queue
void Scheduler::reAddProcess(std::shared_ptr<Process> process)
{
     //std::lock_guard<std::mutex> lock(mtx); // Uncomment for thread safety
    this->readyQueue.push_back(process);
}

// Get the first process in the ready queue
std::shared_ptr<Process> Scheduler::getFirstProcess()
{
    return this->readyQueue.front();
}

// Remove the first process from the ready queue
void Scheduler::removeFirstProcess()
{
    readyQueue.erase(readyQueue.begin());
}

// Sort the ready queue based on process creation time
void Scheduler::sortReadyQueue()
{
    std::sort(this->readyQueue.begin(), this->readyQueue.end(), [](std::shared_ptr<Process> a, std::shared_ptr<Process> b) {
        return a->getCreationTime() < b->getCreationTime();
        });
}

// Get all processes in the system
std::vector<std::shared_ptr<Process>>& Scheduler::getAllProcess()
{
    return this->allProcesses;
}

void Scheduler::schedulerTest()
{
    std::lock_guard<std::mutex> lock(mtx);
    if (!schedulerTestFlag) {
        schedulerTestFlag = true;
        testThread = std::thread(&Scheduler::schedulerTestLoop, this);
    }
}

void Scheduler::schedulerStop()
{
    {
        std::lock_guard<std::mutex> lock(mtx);
        if (schedulerTestFlag) {
            schedulerTestFlag = false;
        }
    }
    cv.notify_all();  // wake up the scheduler test loop immediately

    if (testThread.joinable()) {
        testThread.join();  // Ensure the thread is properly joined
    }
}


void Scheduler::schedulerTestLoop()
{
    std::unique_lock<std::mutex> lock(mtx);
    while (schedulerTestFlag) {
        processCounter++;
        int processID = processCounter;
        createProcess(processID);

        // wait with the ability to wake up if schedulerTestFlag is set to false
        cv.wait_for(lock, std::chrono::duration<double>(batchProcessFreq), [this] {
            return !schedulerTestFlag;
        });
    }
}


void Scheduler::createProcess(int processID)
{
    std::random_device rd;  // Random number seed
    std::mt19937 gen(rd()); // Random number generator (Mersenne Twister)
    std::uniform_int_distribution<> dis(minInstructions, maxInstructions);

    int totalinstructions = dis(gen);

    std::string processName = "Process_" + std::to_string(processID).substr(0, 2);
    String toPrint = "Hello world from " + processName;
    auto process = std::make_shared<Process>(processName, processID, totalinstructions, PrintCommand(toPrint));
    auto processScreen = std::make_shared<BaseScreen>(process, processName);
    ConsoleManager::getInstance()->registerScreen(processScreen);
	addNewProcess(process);
}

float Scheduler::getCPUUtilization()
{
	int totalCores = cores.size();
	int activeCores = 0;
	for (Core* core : cores)
	{
		if (core->hasAttachedProcess())
		{
            activeCores++;
		}
	}
	return (float)activeCores / totalCores;
}

int Scheduler::getAvailableCores()
{
    int availableCores = 0;
    for (Core* core : cores)
    {
        if (!core->hasAttachedProcess())
        {
            availableCores++;
        }
    }
    return availableCores;
}

int Scheduler::getNumberOfCoresUsed()
{
	int coresUsed = 0;
	for (Core* core : cores)
	{
		if (core->hasAttachedProcess())
		{
			coresUsed++;
		}
	}
	return coresUsed;
}

unsigned int Scheduler::getMinInstructions()
{
	return minInstructions;
}

unsigned int Scheduler::getMaxInstructions()
{
	return maxInstructions;
}