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
    static const int MemBanks = 4; // Number of memory banks
    MemBank MemoryBanks[MemBanks];

    void initBanks();
    void pushData(std::vector<channelM<int>>&, int cycle, bool debug = false);
    void increment(int cycle);
private:
    int x = 0;
    int y = 0;
    int z = 0;
    int a = 0;
    int b = 0;
    int c = 0;
    int d = 0;
    int e = 0;
    int f = 0;
    int g = 0;
};

#endif // MEMORY_H
