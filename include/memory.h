#ifndef MEMORY_H
#define MEMORY_H
#include "globals.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <array>

using namespace std;



struct MemBank {
    static const int BANK_ROWS = 8;
    static const int BANK_COLS = 8;
    unsigned int Data[BANK_ROWS][BANK_COLS];
};


class Memory {
    public: 
    int BANK_ROWS = 8;
    int BANK_COLS = 8;


    static const int MemBanks = 15; // Define the number of memory banks
    MemBank MemoryBanks[MemBanks];
    //unsigned int bankTraverse(); 
    void initBanks(int numBanks, int BANK_ROWS, int BANK_COLS);

};



#endif // MEMORY_H
