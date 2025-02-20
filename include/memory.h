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

int BANK_ROWS = 8;
int BANK_COLS = 8;
int N;
int MemBanks = 5; // Number of memory banks


using namespace std;

struct MemBank {
    
     std::vector<std::vector<int>> Data;

    MemBank(int rows, int cols) : Data(rows, std::vector<int>(cols)) {}

};

class Memory {
public:
   
    std::vector<MemBank> MemoryBanks;


    Memory();

    void initBanks();
    void pushData(std::vector<channelM<int>>&, int cycle, bool debug = false);
    void increment(int cycle);
    void initBanksFromLeft(std::vector<int>& TPU_left);


private:
    std::vector<std::vector<int>> indices;
    int group;
    int start = 3;
    int end = 3 * (N);   // N would be 8 for us**
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
