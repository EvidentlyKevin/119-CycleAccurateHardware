#ifndef MEMORY_H
#define MEMORY_H
#include "globals.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <array>

using namespace std;

struct MemBank {
    unsigned int BANK_ROWS = 0;
    unsigned int BANK_COLS = 0;
    unsigned int BankID;
    unsigned int Data[BANK_ROWS][BANK_COLS];
};

class Memory {
    public: 
        MemBank();
        unsigned int bankTraverse(); 
        MemBank MemoryBanks[];
        void initBanks(int numBanks);
};

#endif // MEMORY_H
