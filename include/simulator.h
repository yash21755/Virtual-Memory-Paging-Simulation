#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <list>
#include <string>
#include <unordered_map>
#include <vector>

class VirtualMemorySimulator {
public:
    VirtualMemorySimulator(int pageSize, int memorySize, const std::string& algorithm);
    void runSimulation(const std::string& traceFile);

private:
    int pageSize;
    int memorySize;
    int numFrames;
    std::string algorithm;
    std::unordered_map<int, bool> pageTable;
    std::unordered_map<int, bool> tlb;
    std::vector<int> memory;
    std::list<int> tlbList;
    int pageFaults;
    int tlbHits;

    void accessMemory(int pageNumber);
    void updateTLB(int pageNumber);
};

#endif
