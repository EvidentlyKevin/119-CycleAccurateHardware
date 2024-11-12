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

            // Max address index for pipelining logic
            int maxAddrIndex = (cycle / 3) % (BANK_COLS * numBanks);

            int rowOffset = 0;
            int delta = 0;

            // Adjust rowIndex to pipeline data efficiently
            if (i < maxAddrIndex) {
                delta = maxAddrIndex - i;

                // Compute rowOffset based on delta and numBanks
                // This allows rowIndex to be incremented by 1 or 2
                rowOffset = ((delta) / numBanks) + 1;

                // Ensure we don't exceed the maximum number of rows
                if (rowIndex + rowOffset >= BANK_ROWS) {
                    rowOffset = BANK_ROWS - 1 - rowIndex;
                }
            }

            int actualRowIndex = rowIndex + rowOffset;

            // Ensure indices are within bounds
            if (colIndex < BANK_COLS && actualRowIndex < BANK_ROWS) {
                // Get the data from the memory bank
                if (actualRowIndex != rowIndex) {
                    // actualRowIndex = rowIndex;
                    data[delta] = MemoryBanks[bankIndex].Data[actualRowIndex][colIndex];
                }
                data[i] = MemoryBanks[bankIndex].Data[rowIndex][colIndex];

                // Push data into the channel if it's not full
                if (!channels[i].is_full()) {
                    channels[i].push(data[i]);
                    if (maxAddrIndex > 0) {
                        channels[delta].push(data[delta]);
                    }

                    // Debugging to ensure data is pushed
                    if (debug) {
                        std::cout << "Cycle " << cycle << ": Pushed data " << data[i]
                                  << " from Bank " << bankIndex << ", Row " << actualRowIndex
                                  << ", Column " << colIndex << " into Channel " << i << "\n";
                    }
                }
            }
        //}
    }
}
