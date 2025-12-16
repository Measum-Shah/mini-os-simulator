# MiniOS Desktop Simulator

## Project Overview

The MiniOS Desktop Simulator is a comprehensive educational tool designed to visualize and interact with fundamental operating system concepts. Built using C++ and the Qt framework, it provides a graphical user interface (GUI) to simulate key OS modules: Process Scheduling, Memory Management, Process Synchronization, and a basic File System.

This project is ideal for students and developers seeking a hands-on understanding of how operating systems manage resources and concurrency.

## Features

The application is divided into four main modules, accessible via the sidebar navigation:

### 1. Process Scheduling
Simulate and analyze various CPU scheduling algorithms by adding custom processes and measuring performance metrics like Waiting Time and Turnaround Time.
* **Algorithms:** First-Come, First-Served (FCFS), Shortest Job First (SJF - Non-Preemptive), Priority Scheduling (Non-Preemptive), and Round Robin (RR).

### 2. Memory Management
Visualize how memory is allocated and deallocated within a fixed-size memory space.
* **Algorithms:** First Fit, Best Fit, and Worst Fit.
* **Visualization:** A dynamic, color-coded visualization shows current free and occupied memory blocks. 

### 3. Process Synchronization
Demonstrate the classic **Producer-Consumer Problem** using POSIX Threads (`pthreads`), mutexes, and condition variables to manage access to a shared buffer. 

### 4. File System
Implement basic **CRUD (Create, Read, Update, Delete)** operations for files, featuring a simple permissions system (`r` for read, `w` for write).

## 🛠️ Installation and Setup

This project requires a C++ compiler (supporting C++11 or newer), CMake, and the Qt 5 framework.

### Prerequisites

1.  **C++ Compiler:** GCC/G++ or Clang.
2.  **CMake:** Version 3.10 or higher.
3.  **Qt 5:** Essential development libraries (especially `qt5-default` on Linux, or the Qt Installer).
4.  **Pthreads Library:** Typically included with GCC/G++.

### Build Steps

These steps assume you are starting in the project's root directory (`MiniOS-Desktop`).

```bash
# 1. Create a build directory to keep the source tree clean
mkdir build
cd build

# 2. Configure the project using CMake
# This command generates the necessary Makefiles based on the CMakeLists.txt file 
# in the parent directory (..).
cmake ..

# 3. Compile the project
# This command executes the build, compiling C++ files and running Qt's MOC/UIC tools.
make

# 4. Run the executable
./MiniOS
