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

class Scheduler {
public:
    enum ScheduleAlgo
    {
        FCFS
    };

	static void initialize(ScheduleAlgo scheduleAlgo, int numCores, int tickDuration);

    static Scheduler* getInstance();

    void run();
    void addNewProcess(std::shared_ptr<Process> process);
    void reAddProcess(std::shared_ptr<Process> process);
    std::shared_ptr<Process> getFirstProcess();
    void removeFirstProcess();
    void startScheduling();
    void printProcesses();

    std::vector<std::shared_ptr<Process>>& getAllProcess();

    // debugging delete later
    int getSize();
    int numCores();

private:
    Scheduler(ScheduleAlgo scheduleAlgo, int numCores, int tickDuration);
    ~Scheduler();

    // Disable copying and assignment
    Scheduler(const Scheduler&) = delete;
    Scheduler& operator=(const Scheduler&) = delete;

    ScheduleAlgo scheduleAlgo;

    std::vector<Core*> cores;

    int tickDuration;

    static Scheduler* sharedInstance;
    std::thread workerThread;

    std::vector<std::shared_ptr<Process>> readyQueue;
    std::vector<std::shared_ptr<Process>> allProcesses;
    std::vector<std::thread> threads;

    static std::mutex mtx;

    void firstComeFirstServe();

    void sortReadyQueue();

    bool isRunning;
    void runProcess(std::shared_ptr<Process> process);
    std::string getCurrentTime(); // Get the current time
    int getCoreId(); // Determine the core being used
};
