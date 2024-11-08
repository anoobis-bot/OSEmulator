#include "Scheduler.h"
#include "ConsoleManager.h"
#include <iomanip>
#include <memory>
#include <random>
#include <unordered_set> 

#include "MemoryManager.h"

Scheduler* Scheduler::sharedInstance = nullptr;
std::mutex Scheduler::mtx;

// Initialize the shared instance of the Scheduler
void Scheduler::initialize(ScheduleAlgo scheduleAlgo, unsigned int quantumCycleMax, int numCores,
    double delayPerExec, unsigned minInstructions, unsigned maxInstruction, unsigned batchProcessFreq, size_t memPerProc) {
    if (!sharedInstance)
    {
        sharedInstance = new Scheduler(scheduleAlgo, quantumCycleMax, numCores, delayPerExec, minInstructions, maxInstruction, batchProcessFreq, memPerProc);
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
    double delayPerExec, unsigned minInstructions, unsigned maxInstruction, unsigned batchProcessFreq, size_t memPerProc) : isRunning(true)
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
    this->memPerProc = memPerProc;

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
    static int quantumCycleCounter = 0;
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

        while (!getFirstProcess()->isInMemory())
        {
            if(MemoryManager::getInstance()->allocate(getFirstProcess()->getID(), getFirstProcess()->getMemoryRequired()))
            {
                getFirstProcess()->setInMemory(true);
            }
            else
            {
                reAddProcess(getFirstProcess());
                removeFirstProcess();
            }
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
    // std::lock_guard<std::mutex> lock(mtx); // Uncomment for thread safety
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
    //TODO set mem per proc in config.txt
    auto process = std::make_shared<Process>(processName, processID, totalinstructions, PrintCommand(toPrint), memPerProc);
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

size_t Scheduler::getMemPerProc()
{
    return memPerProc;
}

void Scheduler::memoryReport(int counter) {
    // Get timestamp
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);

    // Create the filename
    std::ostringstream fileName;
    fileName << "memory_stamp_" << counter << ".txt";

    std::ofstream reportFile(fileName.str());

    if (!reportFile.is_open()) {
        std::cerr << "Could not open file " << fileName.str() << " for writing.\n";
        return;
    }

    // Convert to local time using localtime_s
    std::tm localTime;
    localtime_s(&localTime, &in_time_t);

    // Write timestamp to file
    reportFile << "Timestamp: " << std::put_time(&localTime, "%Y-%m-%d %X") << "\n";

    // Calculate the number of processes in memory
    std::unordered_set<int> uniqueProcesses;

    for (auto const& frame : MemoryManager::getInstance()->getAllocationMap()) {
        if (frame.second.first) {  // If the frame is allocated
            uniqueProcesses.insert(frame.second.second);  // Insert the process id into the set
        }
    }

    int numProcesses = uniqueProcesses.size();
    reportFile << "Number of processes in memory: " << numProcesses << "\n";

    // Calculate external fragmentation
    size_t externalFragmentation = 0;
    bool isFragmenting = false;
    for (auto const& frame : MemoryManager::getInstance()->getAllocationMap()) {
        if (frame.second.first == false) {  // If the frame is free
            if (!isFragmenting) {
                isFragmenting = true;
            }
            externalFragmentation += MemoryManager::getInstance()->getMemPerFrame();
        }
        else {
            isFragmenting = false;
        }
    }

    reportFile << "Total external fragmentation in KB: " << externalFragmentation << "\n\n";

    // ASCII printout of memory
    reportFile << "----end---- = " << MemoryManager::getInstance()->getMemorySize() << "\n\n";

    // For each allocated frame, print process limits
    //nagpriprint lang sya sa 1 file
    size_t currentAddress = MemoryManager::getInstance()->getMemorySize();
    for (auto const& frame : MemoryManager::getInstance()->getAllocationMap()) {
        if (frame.second.first) {
            reportFile << currentAddress << "\n";
            reportFile << "P" << frame.second.second << "\n";
            currentAddress -= MemoryManager::getInstance()->getMemPerProc();
            reportFile << currentAddress << "\n";
        }
    }

    reportFile << "----start---- = 0\n";
    reportFile.close();
}
