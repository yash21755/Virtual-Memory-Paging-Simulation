#include "simulator.h"
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <iostream>

VirtualMemorySimulator::VirtualMemorySimulator(int pageSize, int memorySize, const std::string& algo)
    : pageSize(pageSize), memorySize(memorySize), algorithm(algo), pageFaults(0), tlbHits(0) {
    numFrames = memorySize / pageSize;
}

void VirtualMemorySimulator::runSimulation(const std::string& traceFile) {
    std::ifstream infile(traceFile);
    std::string line;
    while (std::getline(infile, line)) {
        if (line.empty() || line[0] == '#') continue;
        int address = std::stoi(line, nullptr, 16);
        int pageNumber = address / pageSize;
        accessMemory(pageNumber);
    }

    std::cout << "Total page faults: " << pageFaults << std::endl;
    std::cout << "Total TLB hits: " << tlbHits << std::endl;
}

void VirtualMemorySimulator::accessMemory(int pageNumber) {
    // TLB hit
    auto it = std::find(tlbList.begin(), tlbList.end(), pageNumber);
    if (it != tlbList.end()) {
        tlbHits++;
        tlbList.erase(it);
        tlbList.push_back(pageNumber);
    }
    // Page table hit (but not in TLB)
    else if (pageTable.find(pageNumber) != pageTable.end()) {
        updateTLB(pageNumber);
    }
    // Page fault
    else {
        pageFaults++;
        if (memory.size() >= static_cast<size_t>(numFrames)) {
            int evicted = memory.front();
            memory.erase(memory.begin());
            pageTable.erase(evicted);
        }
        memory.push_back(pageNumber);
        pageTable[pageNumber] = true;
        updateTLB(pageNumber);
    }
}

void VirtualMemorySimulator::updateTLB(int pageNumber) {
    // Remove if already in TLB
    auto it = std::find(tlbList.begin(), tlbList.end(), pageNumber);
    if (it != tlbList.end()) {
        tlbList.erase(it);
    }
    // If TLB is full, remove least recently used (front)
    if (tlbList.size() >= 4) {
        tlbList.pop_front();
    }
    tlbList.push_back(pageNumber);
}
