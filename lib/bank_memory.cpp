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

    // Vector for what data to push to each channel
    std::vector<int> data(numChannels);

    for (int i = 0; i < numChannels; ++i) {
        int bankIndex = i / BANK_COLS;

        // for (int j = 0; j < numBanks; ++j) {
            int colIndex = i % BANK_COLS;
            int rowIndex = (cycle / 3) / (numBanks * BANK_COLS);

            // Ensure indices are within bounds
            if (colIndex < BANK_COLS && rowIndex < BANK_ROWS) {
                data[i] = MemoryBanks[bankIndex].Data[rowIndex][colIndex];

                // Push data into the channel if it's not full
                if (!channels[i].is_full()) {
                    if (i == (cycle / 3) % numChannels) {
                        channels[i].push(data[i]);
                    }

                    // Debugging to ensure data is pushed
                    if (debug) {
                        std::cout << "Cycle " << cycle << ": Pushed data " << data[i]
                                  << " from Bank " << bankIndex << ", Row " << rowIndex
                                  << ", Column " << colIndex << " into Channel " << i << "\n";
                    }
                }
            }
        //}
    }
}
