// File: lib/bank_memory.cpp

#include "../include/memory.h"
#include <random>


Memory::Memory() : indices(100, std::vector<int>(100)) {
    // Additional initialization if needed
}


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

void Memory::increment(int cycle) {


    for (int i = start; i < end; i += 6) {
        int group = (i - start) / 6 + 1;  // Index for each group (this is for making accessing easier)

        
        if (cycle % 3 == 0 && cycle > i) {
            indices[group][0]++;
            indices[group][1]++;
            indices[group][2] = indices[group][1] - 1; // Update z, c, f, etc.
            
            if (indices[group][1] > N - 1) {
                indices[group][0] = 0;
                indices[group][1] = 0;
                indices[group][2] = N - 1;
            }
             std::cout << "Group " << group << ": x=" << indices[group][0]
                      << " y=" << indices[group][1]
                      << " z=" << indices[group][2] << std::endl;

          
        }
    }
}



/*void Memory::increment(int cycle) {
    

    if((cycle) % 3 == 0 && cycle > 3){      
        x++;
        y++;
        z = y - 1;
        cout << "x: " << x << " y: " << y << " z: " << z << endl;
    }

    if((cycle) % 3 == 0 && cycle > 9){
        a++;
        b++;
        c = a - 1;
        cout << "a: " << a << " b: " << b << " c: " << c << endl;
    }

    if(cycle % 3 == 0 && cycle > 15){
        d++;
        e++;
        f = d - 1;
        cout << "d: " << d << " e: " << e << " f: " << f << endl;
    }
    
    if(cycle % 3 == 0 && cycle > 21){
        g++;
        cout << "g: " << g << endl;
    }

    
    
}*/

void Memory::pushData(std::vector<channelM<int>>& channels, int cycle, bool debug) {
    // const int Push_Period = 3;
    int numChannels = channels.size();
    double pos_inf = std::numeric_limits<double>::infinity();
    double neg_inf = -std::numeric_limits<double>::infinity();

    // Vector for what data to push to each channel
    std::vector<int> data(numChannels);

    for (int i = 0; i < numChannels; ++i) {
       
        int bankIndex = i / BANK_COLS;

        int colIndex = i % BANK_COLS;


        int storeCycle = 0;
        int rowIndex = 0;

        int RowOffset = 2 + ((cycle - 3) / 6) * 2;

        int RowOffset2 = RowOffset - 1;

        data[i] = MemoryBanks[bankIndex].Data[rowIndex][colIndex];




        if((cycle - 3) % 6 == 0) { // specfic edge cases fpr data[0]
            data[0] = MemoryBanks[bankIndex].Data[rowIndex + RowOffset2][colIndex];

        }

        /*for (int j = 0; j < numChannels; ++j) {
            data[z] = MemoryBanks[bankIndex].Data[rowIndex][colIndex];
            data[y] = MemoryBanks[bankIndex].Data[rowIndex][colIndex];
        }*/

       


        /*if(cycle % Push_Period == 0 && cycle > storeCycle){
            data[z] = MemoryBanks[bankIndex].Data[rowIndex + RowOffset][colIndex];
            data[x] = MemoryBanks[bankIndex].Data[rowIndex + RowOffset2][colIndex];

            if (y > numChannels - 1) { 
                y = 0;
                z = 7;
            }
            if (x > numChannels - 1) { 
                x = 0;
            }
            
        }*/

       
        //for (int j = 0; j < numChannels; ++j) {
            //data[j] = MemoryBanks[bankIndex].Data[rowIndex][colIndex];
       // }

for (int s = start; s < end; s += 6) {
        
    if (cycle % 3 == 0 && cycle > s) {
    // g in this case is group as in the amount of groups needed for specific array sizes
    for (int g = 1; g <= (N / 2); g++){

    // importabnt to break out of the loop if the indice values are flat out zero which prevents unwanted groups from being pushed
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
        cout << "Data: " << data[i] << endl;
                channels[i].push(data[i]);
                break;

            }
                        
     
       
}

// break for preventing multiple pushes and helps with reducing visual clutter
break;

    }
        }






        /*if(cycle % 3 == 0 && cycle > 3) {
                                    
            data[z] = MemoryBanks[bankIndex].Data[rowIndex + 2][colIndex];
            
            data[y] = MemoryBanks[bankIndex].Data[rowIndex + 1][colIndex];

            if (y > BANK_ROWS - 1) { // GET RID OF MAGIC NUMBER
                    y = 0;
                    z = 7;
            }
            if (x > 7) { // USE EXPRESSION OR CONSTANT
                x = 0;
            }

        }

            
        if((cycle) % 3 == 0 && cycle > 9) {
                                    
            data[c] = MemoryBanks[bankIndex].Data[rowIndex + 4][colIndex];
            
            data[a] = MemoryBanks[bankIndex].Data[rowIndex + 3][colIndex];

            if (b > 7) { // GET RID OF MAGIC NUMBER
                b = 0;
                c = 7;
            }
            if (a > 7) { // USE EXPRESSION OR CONSTANT
                a = 0;
            }

        }

            
        if((cycle) % 3 == 0 && cycle > 15){
                                    
            data[f] = MemoryBanks[bankIndex].Data[rowIndex + 6][colIndex];
            
            data[d] = MemoryBanks[bankIndex].Data[rowIndex + 5][colIndex];

            if (e > 7) { // GET RID OF MAGIC NUMBER
                e = 0;
                f = 7;
            }
            if (d > 7) { // USE EXPRESSION OR CONSTANT
                d = 0;
            }
        }

        
        
        if (cycle % 3 == 0 && cycle > 21)
        {
            data[g] = MemoryBanks[bankIndex].Data[rowIndex + 7][colIndex];
        }*/


        

        
        
        // Push data into the channel if it's not full
        if (!channels[i].is_full()) {
            if(i == (cycle / 3) % numChannels) {
                channels[i].push(data[i]);
                 if (data[0] == 2 && cycle < 6) {
                channels[0].push(data[0]);
            }
            }
           

        
            // Check for data values to push into channels
            
            // Debugging to ensure data is pushed
            if (debug) {
            std::cout << "Cycle " << cycle << ": Pushed data " << data[i]
                        << " from Bank " << bankIndex << ", Row " << rowIndex
                        << ", Column " << colIndex << " into Channel " << i << "\n";
            }

            // Increment x, y, and z only once per cycle

        }
        
    }
}




