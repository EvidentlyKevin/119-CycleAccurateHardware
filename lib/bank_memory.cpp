// File: lib/bank_memory.cpp

#include "../include/memory.h"
#include <random>


Memory::Memory() : MemoryBanks(MemBanks, MemBank(BANK_ROWS, BANK_COLS)), indices(100, std::vector<int>(100, 0)) {
    // Additional initialization if needed
}



void Memory::initBanks() {
    Memory mem;
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

void Memory::increment(int cycle) {

if((cycle - 3) % 3 == 0) { // specfic edge cases fpr data[0]

        z++;

        if(z > N) {
            z = 1;
        }

            


        }


    for (int i = start; i < end; i += 6) {
        group = (i - start) / 6 + 1;  // Index for each group (this is for making accessing easier)

        
        if (cycle % 3 == 0 && cycle > i) {
            indices[group][0]++;
            indices[group][1]++;
            indices[group][2] = indices[group][1] - 1; // Update z, c, f, etc.
            
            if (indices[group][1] > N - 1) {
                indices[group][0] = 0;
                indices[group][1] = 0;
                indices[group][2] = N - 1;
            }
             

          
        }
    }
}




void Memory::pushData(std::vector<channelM<int>>& channels, int cycle, bool debug) {
    // const int Push_Period = 3;
    size_t numChannels = channels.size();
    double pos_inf = std::numeric_limits<double>::infinity();
    double neg_inf = -std::numeric_limits<double>::infinity();

    // Vector for what data to push to each channel
    std::vector<int> data(numChannels);

    for (int i = 0; i < numChannels; ++i) {
       
        int bankIndex = i / BANK_COLS;

        int colIndex = i % BANK_COLS;


        // int storeCycle = 0;
        int rowIndex = 0;

        int RowOffset;

   

        data[i] = MemoryBanks[bankIndex].Data[rowIndex][colIndex];




        if((cycle - 3) % 6 == 0) { // specfic edge cases fpr data[0]
  

 
            data[0] = MemoryBanks[bankIndex].Data[rowIndex + (z - 1)][colIndex];
            


        }

       

for (int s = start; s < end; s += 6) {
        
    if (cycle % 3 == 0 && cycle > s) {
    // g in this case is group as in the amount of groups needed for specific array sizes
    for (g = 1; g <= (N / 2); g++){

    // important to break out of the loop if the indice values are flat out zero which prevents unwanted groups from being pushed
    if (indices[g][2] == 0 && indices[g][1] == 0 && indices[g][1] == 0) {
     break;
     
    }
    // if g is not equal to N/2 then push the data into the channels important for edge case
   if (g != N/2) {
     data[indices[g][2]] = MemoryBanks[bankIndex].Data[rowIndex + (g * 2)][colIndex];
     data[indices[g][1]] = MemoryBanks[bankIndex].Data[rowIndex + (g * 2) - 1][colIndex];
   }
    // if g is equal to N/2 then push the data into the channels which is the final group aka edge case
    if(g == N/2) { 
        data[indices[g][1]] = MemoryBanks[bankIndex].Data[rowIndex + ((g * 2) - 1)][colIndex];
   }

    // important to break here so it doesn't push multiple times into same channel
     if (data[i] >= neg_inf && data[i] <= pos_inf && data[i] != 1 && data[i] != 0) {
                channels[i].push(data[i]);
                break;

            }
                        
     
       
}

// break for preventing multiple pushes and helps with reducing visual clutter
break;

    }
        }
        // Push data into the channel if it's not full
        if (!channels[i].is_full()) {
            if(i == (cycle / 3) % numChannels) {
                channels[i].push(data[i]);
                 if (cycle == 3 ) {
                channels[0].push(data[0]);
            }
            }
           

        
            // Check for data values to push into channels
            
            // Debugging to ensure data is pushed
            if (false) {
                
            std::cout << "Cycle " << cycle << ": Pushed data " << data[i]
                        << " from Bank " << bankIndex << ", Row " << rowIndex
                        << ", Column " << colIndex << " into Channel " << i << "\n";
            }

            // Increment x, y, and z only once per cycle

        }
        
    }
}




