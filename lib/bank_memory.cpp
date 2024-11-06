
// File: lib/bank_memory.cpp

#include "../include/memory.h"
#include <random>

void Memory::initBanks() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 255);

    for (int i = 0; i < MemBanks; i++) {
        int foo = 0;
        for (int j = 0; j < BANK_ROWS; j++) {
            ++foo;
            for (int k = 0; k < BANK_COLS; k++) {
                // MemoryBanks[i].Data[j][k] = dis(gen);
                MemoryBanks[i].Data[j][k] = foo;
                // MemoryBanks[i].Data[j][k] = 6;
            }
        }
    }
}

void Memory::pushData(std::vector<channelM<int>>& channels, int cycle, bool debug) {
    int numBanks = MemBanks;
    int numChannels = channels.size();

    for (int i = 0; i < numChannels; ++i) {
        int bankIndex = i % BANK_COLS;
        for(int j = 0; j < numBanks; ++j) {
            // Determine the bank and column index for this channel

            
            int colIndex = j % numBanks;
            int rowIndex = cycle / 3;

            // KEVIN WORK
            // Max row and col index that do not need to be pushed to the channel
            int maxAddrIndex = (cycle / 3) % (BANK_COLS * numBanks);
            // KEVIN WORK
            

            // Ensure indices are within bounds
            if (colIndex < BANK_COLS && rowIndex < BANK_ROWS) {
                // Get the data from the memory bank

                int data;

                // For accurate pipelining
                //if (i < maxAddrIndex && rowIndex < BANK_ROWS-1) {
                   // data = MemoryBanks[bankIndex].Data[rowIndex+1][colIndex];
                //} else {
                    data = MemoryBanks[bankIndex].Data[rowIndex][colIndex];
               // }

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
}