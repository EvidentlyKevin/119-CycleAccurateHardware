/*
#include "globals.h"
#include "memory.h"
#include <iostream>
#include <fstream>
#include <array>

using namespace std;

class MemoryModule {
public:
    
    int initBanks(){
// call random num gen and use output to initialize banks 

        for(int i = 0; i < BANK_ROWS; i++){
            for(int j = 0; j < numBanks; j++){
                for(int k = 0; k< BANK_COLS; k++){
                    MemoryBanks[j].Data[i][k] = rand() % 256;
                }
            }
        }
    }

    int bankTraverse(){
        for(int i = 0; i< BANK_ROWS; i++){
            for(int j = 0; j < numBanks; j++){
                for(int k = 0; k < BANK_COLS; k++){
                    int8_t vals = bank.Data[i][j][k] << endl;
                    //inputA_channel.push(vals);
                }
            }
        }
    }
}

*/