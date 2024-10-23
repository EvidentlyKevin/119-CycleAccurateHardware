#include <iostream>
#include <vector>
#include "../include/memory.h"
#include "../include/systolic_array.h"

void memoryFunction() {
    Memory mem;
    mem.initBanks(); // No parameters needed

    // Display the contents of the memory banks
    for (int i = 0; i < mem.MemBanks; i++) {
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

void systolicArrayFunction() {
    // Define the size of the systolic array
    const int SIZE = 8;

    // Create an instance of the systolic array
    Systolic_Array<int> systolicArray(SIZE);

    // Initialize weights and activations
    std::vector<std::vector<int>> weights(SIZE, std::vector<int>(SIZE));
    std::vector<int> activations(SIZE);

    // Example initialization
    for (int i = 0; i < SIZE; ++i) {
        activations[i] = i + 1; // Activations: 1, 2, ..., 8
        for (int j = 0; j < SIZE; ++j) {
            weights[i][j] = (i + 1) * (j + 1); // Weights: multiplication table
        }
    }

    // Set weights in the systolic array
    systolicArray.setWeights(weights);

    // Set input activations
    systolicArray.setInputActivations(activations);

    // Run the systolic array for sufficient cycles
    int num_cycles = 2 * SIZE; // Adjust as needed
    for (int cycle = 0; cycle < num_cycles; ++cycle) {
        systolicArray.cycle();
    }

    // Get the outputs
    std::vector<int> outputs = systolicArray.getOutputs();

    // Print the outputs
    std::cout << "Systolic Array Outputs:\n";
    for (const auto& val : outputs) {
        std::cout << val << " ";
    }
    std::cout << std::endl;
}

int main() {
    int testOption;

    // Ask the user to input a test option
    std::cout << "Enter a test option (1-2):\n";
    std::cout << "1: Test Memory Function\n";
    std::cout << "2: Test Systolic Array Function\n";
    std::cout << "Option: ";
    std::cin >> testOption;

    // Use switch-case to handle different options
    switch (testOption) {
        case 1:
            memoryFunction();
            break;
        case 2:
            systolicArrayFunction();
            break;
        default:
            std::cout << "Invalid option selected!" << std::endl;
            break;
    }

    return 0;
}
