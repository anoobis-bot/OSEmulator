#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <vector>
#include <thread>
#include <memory>
#include <atomic>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include "Process.h"
#include "Core.h"

class Scheduler {
public:
    enum ScheduleAlgo
    {
        FCFS
    };

	static Scheduler& getInstance(ScheduleAlgo scheduleAlgo, int numCores, int tickDuration);

    void run();
    void addProcess(std::shared_ptr<Process> process);
    void startScheduling();
    void printProcesses();

private:
    Scheduler(ScheduleAlgo scheduleAlgo, int numCores, int tickDuration);
    ~Scheduler();

    // Disable copying and assignment
    Scheduler(const Scheduler&) = delete;
    Scheduler& operator=(const Scheduler&) = delete;

    ScheduleAlgo scheduleAlgo;

    std::vector<Core> cores;

    static Scheduler* sharedInstance;
    std::thread workerThread;

    std::vector<std::shared_ptr<Process>> readyQueue;
    std::vector<std::thread> threads;
    bool isRunning;
    void runProcess(std::shared_ptr<Process> process);
    std::string getCurrentTime(); // Get the current time
    int getCoreId(); // Determine the core being used
};

#endif // SCHEDULER_H
