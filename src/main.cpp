#include <iostream>
#include <vector>
#include "../include/Cluster.h"
#include "../include/memory.h"
#include "../include/systolic_array.h"
#include "../include/activation.h"


void memoryFunction() {
    Memory mem;
    mem.initBanks(); // No parameters needed

    // Display the contents of the memory banks
    for (int i = 0; i < MemBanks; i++) {
        std::cout << "Memory Bank " << i << ":\n";
        for (int j = 0; j < BANK_ROWS; j++) {
            for (int k = 0; k < BANK_COLS; k++) {
                std::cout << mem.MemoryBanks[i].Data[j][k] << " ";
            }
            std::cout << std::endl;
        }
        std::cout << "---------------------------\n";
    }
}


int main() {
    int testOption;
    Cluster<int> cluster(1);

    

    // What do you want to test
    std::cout << "Enter a test option (1-2):\n";
    std::cout << "1: Test Memory Function\n";
    std::cout << "2: Test Systolic Array Function\n";
    std::cout << "Option: ";
    std::cin >> testOption;
    std::cout << "--------------------------" << std::endl;

    // Use switch-case to handle different options
    switch (testOption) {
        case 1:
        memoryFunction();
            break;
        case 2:
            cluster.setParametersForTPUs();
            //cluster.showbanks();
            cluster.runAllTPUs();

            break;
        default:
            std::cout << "Invalid option selected!" << std::endl;
            break;
    }

    return 0;
}