// lib/bank_memory.cpp
#include "../include/memory.h"
#include <random>

void Memory::initBanks() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 255);

    for (int i = 0; i < MemBanks; i++) {
        for (int j = 0; j < BANK_ROWS; j++) {
            for (int k = 0; k < BANK_COLS; k++) {
                MemoryBanks[i].Data[j][k] = dis(gen);
            }
        }
    }
}
