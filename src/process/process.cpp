#include "process.h"
#include <algorithm>
#include <iostream>

ProcessScheduler::ProcessScheduler() {}

void ProcessScheduler::addProcess(int pid, int arrival, int burst, int priority)
{
    Process p;
    p.pid = pid;
    p.arrivalTime = arrival;
    p.burstTime = burst;
    p.remainingTime = burst;
    p.priority = priority;
    p.waitingTime = 0;
    p.turnaroundTime = 0;
    processes.push_back(p);
}

// First-Come-First-Serve
void ProcessScheduler::FCFS()
{
    std::sort(processes.begin(), processes.end(),
              [](const Process &a, const Process &b) {
                  return a.arrivalTime < b.arrivalTime;
              });

    calculateWaitingAndTurnaround();
}

// Shortest Job First
void ProcessScheduler::SJF()
{
    // Non-preemptive SJF
    std::sort(processes.begin(), processes.end(),
              [](const Process &a, const Process &b) {
                  if(a.arrivalTime == b.arrivalTime)
                      return a.burstTime < b.burstTime;
                  return a.arrivalTime < b.arrivalTime;
              });

    calculateWaitingAndTurnaround();
}

// Priority Scheduling
void ProcessScheduler::PriorityScheduling()
{
    std::sort(processes.begin(), processes.end(),
              [](const Process &a, const Process &b) {
                  if(a.arrivalTime == b.arrivalTime)
                      return a.priority > b.priority; // Higher priority first
                  return a.arrivalTime < b.arrivalTime;
              });

    calculateWaitingAndTurnaround();
}

// Round Robin Scheduling
void ProcessScheduler::RoundRobin(int timeQuantum)
{
    int time = 0;
    bool done;
    do {
        done = true;
        for (auto &p : processes)
        {
            if (p.remainingTime > 0)
            {
                done = false;
                if (p.remainingTime > timeQuantum)
                {
                    time += timeQuantum;
                    p.remainingTime -= timeQuantum;
                }
                else
                {
                    time += p.remainingTime;
                    p.waitingTime = time - p.arrivalTime - p.burstTime;
                    p.remainingTime = 0;
                    p.turnaroundTime = time - p.arrivalTime;
                }
            }
        }
    } while (!done);
}

std::vector<Process> ProcessScheduler::getProcesses() const
{
    return processes;
}

// Helper: calculate waiting and turnaround for FCFS/SJF/Priority
void ProcessScheduler::calculateWaitingAndTurnaround()
{
    int currentTime = 0;
    for (auto &p : processes)
    {
        if(currentTime < p.arrivalTime)
            currentTime = p.arrivalTime;

        p.waitingTime = currentTime - p.arrivalTime;
        currentTime += p.burstTime;
        p.turnaroundTime = p.waitingTime + p.burstTime;
    }
}
