#ifndef MEMORY_H
#define MEMORY_H

#include <vector>
#include <string>

struct MemoryBlock {
    int start;
    int size;
    bool isFree;
    std::string processName; // Optional: assigned process
};

class MemoryManager {
public:
    // FIX: Added the missing enum for the UI to use
    enum AllocationStrategy {
        FIRST_FIT,
        BEST_FIT,
        WORST_FIT
    };

    MemoryManager(int totalMemory);

    void allocateFirstFit(int size, const std::string &processName);
    void allocateBestFit(int size, const std::string &processName);
    void allocateWorstFit(int size, const std::string &processName);

    void deallocate(const std::string &processName);

    std::vector<MemoryBlock> getMemoryBlocks() const;

private:
    int totalMemory;
    std::vector<MemoryBlock> blocks;
    void mergeFreeBlocks();
};

#endif // MEMORY_H