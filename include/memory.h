#ifndef MEMORY_H
#define MEMORY_H
#include "globals.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <array>

using namespace std;

struct MemBank {
    unsigned int Data[BANK_ROWS][BANK_COLS];
};

class Memory {
    public: 
        int MemBanks = 1024; // Define the number of memory banks
        MemBank MemoryBanks[MemBanks];
        unsigned int bankTraverse(); 
        int initBanks(int numBanks, int BANK_ROWS, int BANK_COLS);
};

#endif // MEMORY_H
