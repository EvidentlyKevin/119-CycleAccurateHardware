// File: include/memory.h

#ifndef MEMORY_H
#define MEMORY_H

#include "globals.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <array>
#include <vector>
#include "channelM.h"

using namespace std;

struct MemBank {
    unsigned int Data[BANK_ROWS][BANK_COLS];
};

class Memory {
public:
    static const int MemBanks = 15; // Number of memory banks
    MemBank MemoryBanks[MemBanks];

    void initBanks();
    void pushData(std::vector<channelM<int>>&, int cycle, bool debug = false);
};

#endif // MEMORY_H
