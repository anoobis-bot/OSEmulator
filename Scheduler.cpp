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
	    if (core->isAvailable() && !this->readyQueue.empty())
	    {
            if (core -> hasAttachedProcess())
            {
                // returning the process to the ready queue
                this->addProcess(core->getAttachedProcess());
                if (core->getAttachedProcess()->getState() != Process::FINISHED)
                {
                    core->getAttachedProcess()->readyState();
                }
            }
            core->attachProcess(this->getFirstProcess());
            this->getFirstProcess()->setCoreID(core->getCoreID());
            this->getFirstProcess()->runningState();
            this->removeFirstProcess();
	    }
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

//Scheduler::~Scheduler() {
//    isRunning = false;
//    for (auto& thread : threads) {
//        if (thread.joinable()) {
//            thread.join();
//        }
//    }
//}

void Scheduler::addProcess(std::shared_ptr<Process> process) {
    std::lock_guard<std::mutex> lock(mtx); // Lock the mutex before modifying the vector
    this->readyQueue.push_back(process);
    this->allProcesses.push_back(process);
}

std::shared_ptr<Process> Scheduler::getFirstProcess()
{
    std::lock_guard<std::mutex> lock(mtx); // Lock the mutex before accessing the vector
	return this->readyQueue.front();
}

void Scheduler::removeFirstProcess()
{
    std::lock_guard<std::mutex> lock(mtx); // Lock the mutex before accessing the vector
    readyQueue.erase(readyQueue.begin());
}

void Scheduler::sortReadyQueue()
{
    std::lock_guard<std::mutex> lock(mtx); // Lock the mutex before modifying the vector
    std::sort(this->readyQueue.begin(), this->readyQueue.end(), [](std::shared_ptr<Process> a, std::shared_ptr<Process> b) {
        return a->getCreationTime() < b->getCreationTime();
    });
}

std::vector<std::shared_ptr<Process>>& Scheduler::getAllProcess()
{
    return this->allProcesses;
}


//void Scheduler::startScheduling() {
//    for (auto& process : processes) {
//        threads.emplace_back(&Scheduler::runProcess, this, process);
//    }
//}

//void Scheduler::runProcess(std::shared_ptr<Process> process) {
//    std::ofstream outputFile("screen_" + process->getName() + ".txt", std::ios::app);
//
//    for (int i = 0; i < 100; ++i) {
//        std::string timestamp = getCurrentTime();
//        int coreId = getCoreId();
//
//        outputFile << "(" << timestamp << ") Core " << coreId << ": \"Hello world from " << process->getName() << "!\"\n";
//        process->incrementCommandsExecuted();   // Not Complete
//
//        std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Simulate processing time
//    }
//
//    outputFile.close();
//    process->finish(); // Not Complete
//}

//std::string Scheduler::getCurrentTime() {
//    auto now = std::chrono::system_clock::now();
//    auto now_c = std::chrono::system_clock::to_time_t(now);
//    std::tm* now_tm = std::localtime(&now_c);
//
//    std::ostringstream oss;
//    oss << std::put_time(now_tm, "%m/%d/%Y %I:%M:%S %p");
//    return oss.str();
//}

//int Scheduler::getCoreId() {
//    static int coreId = 0; // Cycle through cores for simplicity
//    return coreId++ % 4;   // Assuming 4 cores (0, 1, 2, 3)
//}

//void Scheduler::printProcesses() {
//    std::cout << "----------------------------------------------" << std::endl;
//    std::cout << "Running processes:" << std::endl;
//
//    for (const auto& process : processes) {
//        std::string state = process->getState(); // Not Complete
//        std::cout << process->getName() << "\t(" << getCurrentTime() << ")\t\tCore: " << getCoreId() << "\t\t" << process->getCommandsExecuted() << "/100" << std::endl;
//    }
//
//    std::cout << "\nFinished processes:" << std::endl;
//    for (const auto& process : processes) {
//        if (process->getState() == "finished") {    // Not Complete
//            std::cout << process->getName() << "\t(" << getCurrentTime() << ")\t\tFinished\t100/100" << std::endl;
//        }
//    }
//    std::cout << "----------------------------------------------" << std::endl;
//}
