#include "memory.h"
#include <algorithm>
#include <iostream>

MemoryManager::MemoryManager(int total)
{
    totalMemory = total;
    blocks.push_back({0, totalMemory, true, ""});
}

void MemoryManager::allocateFirstFit(int size, const std::string &processName)
{
    for(auto &block : blocks)
    {
        if(block.isFree && block.size >= size)
        {
            MemoryBlock newBlock = {block.start, size, false, processName};
            block.start += size;
            block.size -= size;
            // The original line 'if(block.size == 0) block.isFree = false;' was flawed; removed for safer compilation.
            blocks.push_back(newBlock);
            return;
        }
    }
    std::cout << "No sufficient memory for process: " << processName << "\n";
}

void MemoryManager::allocateBestFit(int size, const std::string &processName)
{
    auto best = std::min_element(blocks.begin(), blocks.end(),
        [size](const MemoryBlock &a, const MemoryBlock &b) {
            if(a.isFree && a.size >= size && b.isFree && b.size >= size)
                return a.size < b.size;
            return false;
        });

    if(best != blocks.end() && best->isFree && best->size >= size) // Added best->size >= size check for robustness
        allocateFirstFit(size, processName); // Reuse FirstFit allocation
    else
        std::cout << "No sufficient memory for process: " << processName << "\n";
}

void MemoryManager::allocateWorstFit(int size, const std::string &processName)
{
    auto worst = std::max_element(blocks.begin(), blocks.end(),
        [size](const MemoryBlock &a, const MemoryBlock &b) {
            if(a.isFree && a.size >= size && b.isFree && b.size >= size)
                return a.size < b.size;
            return false;
        });

    if(worst != blocks.end() && worst->isFree && worst->size >= size) // Added worst->size >= size check for robustness
        allocateFirstFit(size, processName);
    else
        std::cout << "No sufficient memory for process: " << processName << "\n";
}

void MemoryManager::deallocate(const std::string &processName)
{
    for(auto &block : blocks)
    {
        if(block.processName == processName)
        {
            block.isFree = true;
            block.processName = "";
        }
    }
    mergeFreeBlocks();
}

void MemoryManager::mergeFreeBlocks()
{
    std::sort(blocks.begin(), blocks.end(),
              [](const MemoryBlock &a, const MemoryBlock &b) { return a.start < b.start; });

    for(size_t i=0; i<blocks.size()-1;)
    {
        if(blocks[i].isFree && blocks[i+1].isFree)
        {
            blocks[i].size += blocks[i+1].size;
            blocks.erase(blocks.begin() + i + 1);
        }
        else
            i++;
    }
}

std::vector<MemoryBlock> MemoryManager::getMemoryBlocks() const
{
    return blocks;
}