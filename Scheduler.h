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

class Scheduler {
public:
    Scheduler();
    ~Scheduler();

    void addProcess(std::shared_ptr<Process> process);
    void startScheduling();
    void printProcesses();

private:
    std::vector<std::shared_ptr<Process>> processes;
    std::vector<std::thread> threads;
    bool isRunning;
    void runProcess(std::shared_ptr<Process> process);
    std::string getCurrentTime(); // Get the current time
    int getCoreId(); // Determine the core being used
};

#endif // SCHEDULER_H
