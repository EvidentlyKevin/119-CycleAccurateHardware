#include "TPU.h"
#include "activation.h" // Include the activation header file

template<typename T>
TPU<T>::TPU(int row, int col)
    : rowID(row), colID(col), SIZE(0), ROWS(0), COLS(0) {}

template<typename T>
void TPU<T>::setparameters() {
    // Define the size of the systolic array
    /* note:
       You have to adjust the memory bank parameters and N so it doesn't crash.
       Bank Parameters should be as follows:
         - MemBanks >= SIZE,
         - Rows = SIZE,
         - Cols = SIZE / 4.
       N should be equal to SIZE.
       You can find these parameters in the memory.h and globals.h files.
       Further investigation or explanation is needed to understand the relationship between
       the memory banks and the systolic array size.
    */
    std::cout << "note: Bank Parameters should be as follows MemBanks >= SIZE, Rows = SIZE, Cols = SIZE / 4" << std::endl;
    std::cout << "8x8: 65 cycles" << std::endl;
    std::cout << "16x16: 138 cycles" << std::endl;
    std::cout << "32x32: 283 cycles" << std::endl;
   
    std::cout << "Enter the size of the systolic array: ";
    // std::cin >> SIZE;
    SIZE = 8; // For testing purposes, set SIZE to 8
    std::cout << "--------------------------" << std::endl;

    std::cout << "Enter the number of rows for Bank Memory: ";
   // std::cin >> ROWS;
    ROWS = SIZE; // For testing purposes, set ROWS to SIZE
    std::cout << "--------------------------" << std::endl;
    std::cout << "Enter the number of columns for Bank Memory: ";
   // std::cin >> COLS;
    COLS = SIZE / 4; // For testing purposes, set COLS to SIZE / 4
    std::cout << "--------------------------" << std::endl;
    std::cout << "Enter the number of memory banks: ";
 //   std::cin >> MemBanks;
    MemBanks = 4; // For testing purposes, set MemBanks to SIZE
    std::cout << "--------------------------" << std::endl;
    std::cout << "Enter number of cycles for simulation: ";
   // std::cin >> num_cycles;
   num_cycles = 100; // For testing purposes, set num_cycles to 10
    
}

template<typename T>
void TPU<T>::display() {
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


    // //Memory mem;
    // mem.initBanks(); // No parameters needed

    // // Display the contents of the memory banks
    // for (int i = 0; i < MemBanks; i++) {
    //     std::cout << "Memory Bank " << i << ":\n";
    //     for (int j = 0; j < BANK_ROWS; j++) {
    //         for (int k = 0; k < BANK_COLS; k++) {
    //             std::cout << mem.MemoryBanks[i].Data[j][k] << " ";
    //         }
    //         std::cout << std::endl;
    //     }
    //     std::cout << "---------------------------\n";
    // }
}

template<typename T>
void TPU<T>::run() {
    N = SIZE;
    std::cout << "N: " << N << std::endl;
    BANK_COLS = COLS;
    std::cout << "BANK_COLS: " << BANK_COLS << std::endl;
    BANK_ROWS = ROWS;
    std::cout << "BANK_ROWS: " << BANK_ROWS << std::endl;

    Memory mem;
    mem.initBanks();

    Systolic_Array<int> systolicArray(SIZE);
    const int CHANNEL_CAPACITY = 4;

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
    // Reserve is expensive; using emplace_back to construct channels in place.
    for (int i = 0; i < SIZE; ++i) {
        memoryToSystolicChannels.emplace_back(CHANNEL_CAPACITY);
    }

    // Simulation loop
    for (int cycle = 0; cycle < num_cycles; ++cycle) {
        mem.increment(cycle);

        // Set input activations from memory channels
        if (cycle % 3 == 0 || cycle == 0) {
            mem.pushData(memoryToSystolicChannels, cycle, true);
            systolicArray.setInputActivationsFromChannels(memoryToSystolicChannels, cycle, false);
        }
        // Run one cycle of the systolic array
        systolicArray.cycle();

        // Debugging: Print activations read by the systolic array
        for (int j = 0; j < SIZE; ++j) {
             int activation = systolicArray.getMACUnit(0, j)->getLastActivation();
             std::cout << "MAC[0][" << j << "] activation: " << activation << "\n";
        }
    }

    // Get the outputs from the systolic array
std::vector<int> outputs = systolicArray.getOutputs();

// --- NEW CODE: Apply activation function to the outputs ---
// Create an Activation object
Activation act;

// Create a vector to store the activated outputs
std::vector<double> activatedOutputs;

// Convert outputs to double, apply the chosen activation function,
// and store the result in activatedOutputs.
for (size_t i = 0; i < outputs.size(); ++i) {
    double value = static_cast<double>(outputs[i]);
    switch (activationFunction) {  // activationFunction is a member of TPU
        case 1:
            value = act.relu(value);
            break;
        case 2:
            value = act.sigmoid(value);
            break;
        case 3:
            value = act.tanh(value);
            break;
        case 4:
            value = act.gelu(value);
            break;
        default:
            value = act.relu(value);  // Default to ReLU if unknown
            break;
    }
    activatedOutputs.push_back(value);
}
// Print the activated outputs
std::cout << "Systolic Array Activated Outputs with Memory Input:\n";
for (size_t i = 0; i < activatedOutputs.size(); ++i) {
    std::cout << "Output[" << i << "]: " << activatedOutputs[i] << "\n";
}

// Print the outputs
 /*std::cout << "Systolic Array Outputs with Memory Input:\n";
 for (size_t i = 0; i < outputs.size(); ++i) {
    std::cout << "Output[" << i << "]: " << outputs[i] << "\n";
 }*/
}

template<typename T>
void TPU<T>::sendData(const std::vector<T>& inputData) {

    N = SIZE;
    BANK_COLS = COLS;
    BANK_ROWS = ROWS;

    Memory mem;
    mem.initBanks();

    int bank = 0;
    int row = 0;
    int col = 0;

    for (size_t i = 0; i < inputData.size(); ++i) {
        // Safety check
        if (bank >= MemBanks || row >= BANK_ROWS || col >= BANK_COLS) {
            std::cerr << "Out of memory bounds! Aborting data transfer.\n";
            break;
        }
        mem.MemoryBanks[bank].Data[row][col] = static_cast<int>(inputData[i]);

       // mem.MemoryBanks[bank].Data[row][col] = inputData[i];

        ++col;
        if (col >= BANK_COLS) {
            col = 0;
            ++row;

            if (row >= BANK_ROWS) {
                row = 0;
                ++bank;
            }
        }
    }

    std::cout << "Input data size: " << inputData.size() << std::endl;
    std::cout << "TPU[" << rowID << "][" << colID << "] received data:\n";

    for (const auto& val : inputData) {
        std::cout << val << " ";
    }
    std::cout << std::endl;
}

// template<typename T>
// void TPU<T>::sendData(const std::vector<T>& inputData) {

//     Memory mem; // Send data to the TPU memory banks
//     mem.initBanks(); // Initialize memory banks

//     int row = 0;
//     int col = 0;

//     for (size_t i = 0; i < inputData.size(); ++i) {
//         // Calculate the row and column indices for the memory banks
//         if(row >= BANK_ROWS) break; // Prevent out-of-bounds access
           
//         mem.MemoryBanks[0].Data[row][col] = inputData[i]; // Store data in memory bank


//         ++col; // Move to the next column
//         if (col >= BANK_COLS) { // If the end of the row is reached, move to the next row
//             col = 0;
//             ++row;
//         }
//     }
// std::cout << "input data size->"<< inputData.size()<< std::endl;

// std::cout << "TPU["<< rowID <<"]["<< colID << "] received data:\n";
// // Print the input data
// for (const auto& val : inputData) {
//     std::cout << val << " ";
// }
// std::cout << std::endl;



    // // Display the contents of the memory banks
    // for (int i = 0; i < MemBanks; i++) {
    //     std::cout << "Memory Bank " << i << ":\n";
    //     for (int j = 0; j < BANK_ROWS; j++) {
    //         for (int k = 0; k < BANK_COLS; k++) {
    //             std::cout << mem.MemoryBanks[i].Data[j][k] << " ";
    //         }
    //         std::cout << std::endl;
    //     }
    //     std::cout << "---------------------------\n";
    // }

    // int row = 0;
    // int col = 0;

    // for (size_t i = 0; i < inputData.size(); ++i) {
    //     if (row >= BANK_ROWS) break;  // Don't overflow memory

    //     this->mem.MemoryBanks[0].Data[row][col] = inputData[i];  //  Store in proper memory

    //     ++col;
    //     if (col >= BANK_COLS) {
    //         col = 0;
    //         ++row;
    //     }
    // }

    // std::cout << "TPU[" << rowID << "][" << colID << "] received data:\n";
    // for (int i = 0; i < BANK_ROWS; ++i) {
    //     for (int j = 0; j < BANK_COLS; ++j) {
    //         std::cout << this->mem.MemoryBanks[0].Data[i][j] << " ";
    //     }
    //     std::cout << "\n";
    // }
    // std::cout << "---------------------------\n";

