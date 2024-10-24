// File: lib/bank_memory.cpp

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

void Memory::pushData(std::vector<channelM<int>>& channels, int cycle, bool debug) {
    int numBanks = MemBanks;
    int numChannels = channels.size();

    for (int i = 0; i < numChannels; ++i) {
        // Determine the bank and column index for this channel
        int bankIndex = i / numBanks;       // Channel i maps to bank (i % numBanks)
        int colIndex = i % numBanks;        // Channel i maps to column (i / numBanks)
        int rowIndex = cycle % BANK_ROWS;   // Cycle through rows

        // Ensure indices are within bounds
        if (colIndex < BANK_COLS && rowIndex < BANK_ROWS && bankIndex < MemBanks) {
            // Get the data from the memory bank
            int data = MemoryBanks[bankIndex].Data[rowIndex][colIndex];

            // Push data into the channel if it's not full
            if (!channels[i].is_full()) {
                channels[i].push(data);

                // Debugging to ensure data is pushed
                if (debug) {
                    std::cout << "Cycle " << cycle << ": Pushed data " << data
                              << " from Bank " << bankIndex << ", Row " << rowIndex
                              << ", Column " << colIndex << " into Channel " << i << "\n";
                }
            }
        }
    }
}

