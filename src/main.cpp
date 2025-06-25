#include <iostream>
#include "simulator.h"

int main() {
    VirtualMemorySimulator sim(4, 16, "LRU");
    sim.runSimulation("data/trace.txt");
    return 0;
}
