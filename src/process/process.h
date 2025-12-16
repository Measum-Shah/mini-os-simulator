#ifndef PROCESS_H
#define PROCESS_H

#include <vector>
#include <string>

// Process structure
struct Process {
    int pid;            // Process ID
    int arrivalTime;    // Arrival time
    int burstTime;      // Burst time
    int priority;       // Priority (optional)
    int remainingTime;  // For Round Robin
    int waitingTime;    // Calculated
    int turnaroundTime; // Calculated
};

// Scheduler class
class ProcessScheduler {
public:
    ProcessScheduler();
    void addProcess(int pid, int arrival, int burst, int priority = 0);

    // Scheduling algorithms
    void FCFS();
    void SJF();
    void PriorityScheduling();
    void RoundRobin(int timeQuantum);

    // Get processes after scheduling
    std::vector<Process> getProcesses() const;

private:
    std::vector<Process> processes;
    void calculateWaitingAndTurnaround();
};

#endif // PROCESS_H
