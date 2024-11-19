// File: lib/bank_memory.cpp

#include "../include/memory.h"
#include <random>
int z = 0;
int x = 0;
int y = 0;


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

void Memory::increment() {
    x++;
    y++;
    z = y - 1;

    cout << "x: " << x << " y: " << y << " z: " << z << endl;
}

void Memory::pushData(std::vector<channelM<int>>& channels, int cycle, bool debug) {
    int numBanks = MemBanks;
    int numChannels = channels.size();

    // Vector for what data to push to each channel
    std::vector<int> data(numChannels);

    for (int i = 0; i < numChannels; ++i) {
       
        int bankIndex = i / BANK_COLS;

        int colIndex = i % BANK_COLS;
        int rowIndex = 0;

        // Max address index for pipelining logic
        int plWindow = (cycle % (BANK_COLS * numBanks) / 3);

        int rowOffset = 0;
        int delta = 0;

        // Adjust rowIndex to pipeline data efficiently
        if (i < plWindow) {
            delta = plWindow - i;

            // Compute rowOffset based on delta and numBanks
            // This allows rowIndex to be incremented by 1 or 2
            rowOffset = ((delta) / 3) + 1;

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

            if(cycle == 2){
                data[0] = MemoryBanks[bankIndex].Data[rowIndex + 1][colIndex];
            }

            if((cycle - 2) % 3 == 0 && cycle > 2){
                                        
                data[z] = MemoryBanks[bankIndex].Data[rowIndex + 2][colIndex];
                data[x] = MemoryBanks[bankIndex].Data[rowIndex + 1][colIndex];
                
                if (y > 7) { // GET RID OF MAGIC NUMBER
                    y = 0;
                    z = 1;
                }
                if (x > 7) { // USE EXPRESSION OR CONSTANT
                    x = 0;
                }
            }
            
            // Push data into the channel if it's not full
            if (!channels[i].is_full()) {
                if(i == (cycle / 3) % numChannels) {
                channels[i].push(data[i]);
                }
                // Check for data values of 3 to push into channels
                for (int j = 0; j < numChannels; ++j) {
                    if (data[j] == 3 && !channels[j].is_full()) {
                        channels[j].push(data[j]);
                    }
                }
                // Debugging to ensure data is pushed
                
                std::cout << "Cycle " << cycle << ": Pushed data " << data[i]
                            << " from Bank " << bankIndex << ", Row " << actualRowIndex
                            << ", Column " << colIndex << " into Channel " << i << "\n";

                // Increment x, y, and z only once per cycle

            }
        }
    }
}




