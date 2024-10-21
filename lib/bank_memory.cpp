#include "../include/globals.h"
#include "../include/memory.h"
#include <array> 
#include <random> 
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <vector>  

void Memory::initBanks(int numBanks, int BANK_ROWS, int BANK_COLS){
    // call random num gen and use output to initialize banks
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 255);

    for(int i = 0; i < numBanks; i++){
        for(int j = 0; j < BANK_ROWS; j++){
            for(int k = 0; k< BANK_COLS; k++){
                MemoryBanks[i].Data[j][k] = dis(gen);
            }
        }
    }
}


// Wrong
/*int bankTraverse(){
    for(int i = 0; i< BANK_ROWS; i++){
        for(int j = 0; j < numBanks; j++){
            for(int k = 0; k < BANK_COLS; k++){
                int8_t vals = bank.Data[i][j][k] << endl;
            }
        }
    }
}
*/


