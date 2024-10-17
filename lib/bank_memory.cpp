#include "../include/globals.h"
#include "../include/memory.h"
#include <array> 
#include <random> 
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <vector>

using namespace std;



  int numBanks = 15;

  

    void Memory::initBanks(int numBanks, int BANK_ROWS, int BANK_COLS){
// call random num gen and use output to initialize banks 
        for(int i = 0; i < numBanks; i++){
            for(int j = 0; j < BANK_ROWS; j++){
                for(int k = 0; k< BANK_COLS; k++){
                    MemoryBanks[i].Data[j][k] = rand() % 256;
                }
            }
        }
    }
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


