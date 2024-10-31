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

    // Initialization
    for (int i = 0; i < SIZE; ++i) {
        activations[i] = i + 1; // Activations: 1, 2, 3, ..., SIZE
        for (int j = 0; j < SIZE; ++j) {
            weights[i][j] = (i + 1) * (j + 1); // Weights: multiplication table
            weights[i][j] = 1; // Simple weights
        }
    }

    // Set weights in the systolic array
    systolicArray.setWeights(weights);

    // Set input activations
    systolicArray.setInputActivations(activations);

    // Run the systolic array for sufficient cycles
    int num_cycles = 100 * SIZE; // Adjust as needed
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

void systolicArrayFunctionWithMemory() {
    // Define the size of the systolic array
    const int SIZE = 8;

    // Define CHANNEL_CAPACITY
    const size_t CHANNEL_CAPACITY = 4;

    // Create an instance of the memory
    Memory mem;
    mem.initBanks();

    // Create an instance of the systolic array
    Systolic_Array<int> systolicArray(SIZE);

    // Set weights for the systolic array
    std::vector<std::vector<int>> weights(SIZE, std::vector<int>(SIZE));
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            // REVIEW HOW WEIGHTS ARE SET
            weights[i][j] = 1; // Simple weights
            // weights[i][j] = (i + 1) * (j + 1); // Complex weights
        }
    }
    systolicArray.setWeights(weights);

    std::vector<channelM<int>> memoryToSystolicChannels;
    // Reserve is expensive, use emplace_back instead
    // emplace_back constructs the object in place, no need to copy
    // meaning that the object is constructed directly in the vector
    //memoryToSystolicChannels.reserve(SIZE);
    for (int i = 0; i < SIZE; ++i) {
        memoryToSystolicChannels.emplace_back(CHANNEL_CAPACITY);
    }

    // NUMBER OF CYCLES FOR SIMULATION, REVIEW THIS
    // Must be sufficient cycles for data to propagate
    // int num_cycles = 2 * SIZE + 1;
    int num_cycles = 44; // Adjust as needed

    // Simulation loop
    // Make this a function in a utility file?
    for (int cycle = 0; cycle < num_cycles; ++cycle) {
        // Memory pushes data into channels
        mem.pushData(memoryToSystolicChannels, cycle, true);

        // Set input activations from memory channels
        systolicArray.setInputActivationsFromChannels(memoryToSystolicChannels, true);

        // Run one cycle of the systolic array
        systolicArray.cycle();

        // Debugging: Print activations read by the systolic array
        std::cout << "Cycle " << cycle << " - Activations read by the systolic array:\n";
        for (int j = 0; j < SIZE; ++j) {
            int activation = systolicArray.getMACUnit(0, j)->getLastActivation();
            std::cout << "MAC[0][" << j << "] activation: " << activation << "\n";
        }
        std::cout << "---------------------------------------------\n";
    }

    // Get the outputs from the systolic array
    std::vector<int> outputs = systolicArray.getOutputs();

    // Print the outputs
    std::cout << "Systolic Array Outputs with Memory Input:\n";
    for (size_t i = 0; i < outputs.size(); ++i) {
        std::cout << "Output[" << i << "]: " << outputs[i] << "\n";
    }
}

int main() {
    int testOption;

    // What do you want to test
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
            systolicArrayFunctionWithMemory();
            break;
        default:
            std::cout << "Invalid option selected!" << std::endl;
            break;
    }

    return 0;
}
