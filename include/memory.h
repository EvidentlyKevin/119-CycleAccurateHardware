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

// Global variables: declare them here (definition should be in a .cpp file)
inline int BANK_ROWS;   // e.g., set BANK_ROWS = 5 in a .cpp file
inline int BANK_COLS;   // e.g., set BANK_COLS = 3 in a .cpp file
inline int N;           // e.g., set N = 8 in a .cpp file (or another appropriate value)
inline int MemBanks;    // Number of memory banks (e.g., set MemBanks = 5 in a .cpp file)

// Structure representing a memory bank
struct MemBank {
    std::vector<std::vector<int>> Data;

    MemBank(int rows, int cols)
        : Data(rows, std::vector<int>(cols))
    {}
};

class Memory {
public:
    std::vector<MemBank> MemoryBanks;

    Memory();

    void initBanks();
    void pushData(std::vector<channelM<int>> &data, int cycle, bool debug = false);
    void increment(int cycle);

private:
    std::vector<std::vector<int>> indices;
    int group;
    int start = 3;
    int end = 3 * N;   // Note: N must be defined before using Memory (see globals definition)
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
