using namespace std;
#include "TPU.h"



template<typename T>
TPU<T>::TPU(int row, int col)
    : rowID(row), colID(col), SIZE(0), ROWS(0), COLS(0), dequeuedValue(8){}

template<typename T>
void TPU<T>::setLeftPort(Port<T>* leftPort) {
    portL = leftPort; // Set the left port
}

template<typename T>
Port<T>& TPU<T>::getRPort() {
    return portR; // Return the right port
}


template<typename T>
void TPU<T>::setparameters() {
   
if(rowID == 0 && colID == 0) {
    cout << "Enter the size of the systolic array: ";
    cin >> SIZE;
    portR.enqueue(SIZE); // Send SIZE to the right port
    }
    else{
        if (!portL->isEmpty()) {
            SIZE = portL->dequeue(); // Receive SIZE from the left port
            portR.enqueue(SIZE); // Send SIZE to the right port
            std::cout << "SIZE: " << SIZE << std::endl;
        }
        
    cout << "SIZE: " << SIZE << endl;
    }

   switch(SIZE){
case 8:
    num_cycles = 65;
    ROWS = 8;
    COLS = 8;
    MemBanks = 1;
    break;
case 16:
    num_cycles = 140;
    ROWS = 16;
    COLS = 16;
    MemBanks = 1;
    break;
case 32:
    num_cycles = 284;
    ROWS = 32;
    COLS = 32;
    MemBanks = 1;
    break;
case 64:
    num_cycles = 574;
    ROWS = 64;
    COLS = 64;
    MemBanks = 1;
    break;
case 128:
    num_cycles = 766;
    ROWS = 128;
    COLS = 128;
    MemBanks = 1;
    break;
case 256:
    num_cycles = 1534;
    ROWS = 256;
    COLS = 256;
    MemBanks = 1;
    break;
case 512:
    num_cycles = 3070;
    ROWS = 512;
    COLS = 512;
    MemBanks = 1;
    break;
case 1024:
    num_cycles = 6142;
    ROWS = 1024;
    COLS = 1024;
    MemBanks = 1;
    break;
case 2048:
    num_cycles = 12286;
    ROWS = 2048;
    COLS = 2048;
    MemBanks = 1;
    break;
case 4096:
    num_cycles = 24574;
    ROWS = 4096;
    COLS = 4096;
    MemBanks = 1;
    break;
case 8192:
    num_cycles = 49150;
    ROWS = 8192;
    COLS = 8192;
    MemBanks = 1;
    break;
case 16384:
    num_cycles = 98302;
    ROWS = 16384;
    COLS = 16384;
    MemBanks = 1;
    break;
case 32768:
    num_cycles = 196606;
    ROWS = 32768;
    COLS = 32768;
    MemBanks = 1;
    break;
case 65536:
    num_cycles = 393214;
    ROWS = 65536;
    COLS = 65536;
    MemBanks = 1;
    break;
case 131072:
    num_cycles = 786430;
    ROWS = 131072;
    COLS = 131072;
    MemBanks = 1;
    break;
case 262144:
    num_cycles = 1572862;
    ROWS = 262144;
    COLS = 262144;
    MemBanks = 1;
    break;
default:
    cout << "Not Practical Size" << endl;
    break;
   }
}



template<typename T>
void TPU<T>::run() {

    N = SIZE;
    cout << "N: " << N << endl;
    BANK_COLS = COLS;
    cout << "BANK_COLS: " << BANK_COLS << endl;
    BANK_ROWS = ROWS;
    cout << "BANK_ROWS: " << BANK_ROWS << endl;

    Memory mem;

if(rowID == 0 && colID == 0) {
    mem.initBanks();
    }
    else if(portL != nullptr) {
        cout << "Getting data from left port" << endl;
        dequeuedValue = portL->dequeueAll(); // Dequeue all elements from the queue
        cout << "Getting data from left port" << endl;
        // Display the contents of the memory bank
        mem.initBanksFromLeft(dequeuedValue); // Initialize memory banks from the left side
     }

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
 

    Systolic_Array<int> systolicArray(SIZE);

        const int CHANNEL_CAPACITY = 4;

    
    
    // Create an instance of the systolic array

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
        //systolicArray.setInputActivationsFromChannels(memoryToSystolicChannels, true);

        // Run one cycle of the systolic array
        systolicArray.cycle();



        // Debugging: Print activations read by the systolic array
        //std::cout << "Cycle " << cycle << " - Activations read by the systolic array:\n";
        for (int j = 0; j < SIZE; ++j) {
            int activation = systolicArray.getMACUnit(0, j)->getLastActivation();
           // std::cout << "MAC[0][" << j << "] activation: " << activation << "\n";
        }
    }

    // Get the outputs from the systolic array
    std::vector<int> outputs = systolicArray.getOutputs();

    // Print the outputs
    std::cout << "Systolic Array Outputs with Memory Input:\n";
    for (int i = outputs.size()-1; i >= 0; --i) {
    portR.enqueue(outputs[i]);
    std::cout << "Output[" << i << "]: " << outputs[i] << "\n";
    }
    


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

/*std::cout << "Systolic Array Activated Outputs with Memory Input:\n";
for (size_t i = 0; i < activatedOutputs.size(); ++i) {
    std::cout << "Output[" << i << "]: " << activatedOutputs[i] << "\n";
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

