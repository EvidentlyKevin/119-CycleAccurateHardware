// File: lib/bank_memory.cpp

#include "../include/memory.h"
#include <random>
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
    
}

void Memory::pushData(std::vector<channelM<int>>& channels, int cycle, bool debug) {
    const int Push_Period = 3;
    int numBanks = MemBanks;
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




            if((cycle - 3) % 6 == 0){
                storeCycle += 3;   
                data[0] = MemoryBanks[bankIndex].Data[rowIndex + RowOffset2][colIndex];
            }


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
            
           
         
           
            if(cycle % 3 == 0 && cycle > 3){
                                        
                data[z] = MemoryBanks[bankIndex].Data[rowIndex + 2][colIndex];
                
                data[x] = MemoryBanks[bankIndex].Data[rowIndex + 1][colIndex];

            if (y > BANK_ROWS - 1) { // GET RID OF MAGIC NUMBER
                    y = 0;
                    z = 7;
                }
                if (x > 7) { // USE EXPRESSION OR CONSTANT
                    x = 0;
                }

            }

            
            if((cycle) % 3 == 0 && cycle > 9){
                                        
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
            }
            


            
            
            // Push data into the channel if it's not full
            if (!channels[i].is_full()) {
                if(i == (cycle / 3) % numChannels) {
                channels[i].push(data[i]);
                }
                // Check for data values to push into channels



           if (data[i] >= neg_inf && data[i] <= pos_inf && data[i] != 1 && data[i] != 0) {
            channels[i].push(data[i]);
            }



                

                
                // Debugging to ensure data is pushed
                
                std::cout << "Cycle " << cycle << ": Pushed data " << data[i]
                            << " from Bank " << bankIndex << ", Row " << rowIndex
                            << ", Column " << colIndex << " into Channel " << i << "\n";

                // Increment x, y, and z only once per cycle

            }
        
    }
}




