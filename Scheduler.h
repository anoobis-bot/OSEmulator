#include <vector>
#include <thread>
#include <memory>
#include <atomic>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <mutex>
#include <sstream>
#include "Process.h"
#include "Core.h"
#include <algorithm>

#include "ScheduleAlgo.h"

class Scheduler {
public:

	static void initialize(ScheduleAlgo scheduleAlgo, unsigned int quantumCycleMax,int numCores, unsigned int tickDelay);

    static Scheduler* getInstance();

    void run();
    void addNewProcess(std::shared_ptr<Process> process);
    void reAddProcess(std::shared_ptr<Process> process);
    std::shared_ptr<Process> getFirstProcess();
    void removeFirstProcess();
    void printProcesses();

    std::vector<std::shared_ptr<Process>>& getAllProcess();

    // debugging delete later
    int getSize();
    int numCores();

    // for the config.txt, delete later
    void setNumCores(int num);
    void setSchedulerType(const std::string& type);
    void setQuantumCycles(int quantum);
    void setBatchProcessFreq(int freq);
    void setMinInstructions(int minIns);
    void setMaxInstructions(int maxIns);
    void setDelayPerExec(int delay);
    void schedulerTest();
    void schedulerStop();
    void schedulerTestLoop();
    void createProcess(int processID);
    int getAvailableCores();
    float getCPUUtilization();
    int getNumberOfCoresUsed();


private:
    Scheduler(ScheduleAlgo scheduleAlgo, unsigned int quantumCycleMax,int numCores, unsigned int tickDelay);
    ~Scheduler();

    // Disable copying and assignment
    Scheduler(const Scheduler&) = delete;
    Scheduler& operator=(const Scheduler&) = delete;

    ScheduleAlgo scheduleAlgo;

    unsigned int quantumCycleMax;

    int batchProcessFreq;      // Frequency of batch processing
    int minInstructions;       // Minimum instructions for a process
    int maxInstructions;       // Maximum instructions for a process
    int delayPerExec;          // Delay per execution cycle
    bool schedulerTestFlag = false;

    std::vector<Core*> cores;

    static Scheduler* sharedInstance;
    std::thread workerThread;

    std::vector<std::shared_ptr<Process>> readyQueue;
    std::vector<std::shared_ptr<Process>> allProcesses;
    std::vector<std::thread> threads;

    std::thread testThread;
    int processCounter = 0;

    static std::mutex mtx;

    void firstComeFirstServe();
    void roundRobin();

    void sortReadyQueue();

    bool isRunning;
    void runProcess(std::shared_ptr<Process> process);
    std::string getCurrentTime(); // Get the current time
    int getCoreId(); // Determine the core being used
};
