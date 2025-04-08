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
        dequeuedValue = portL->dequeueAll(); // Dequeue all elements from the queue
        // Display the contents of the memory bank
        mem.initBanksFromLeft(dequeuedValue); // Initialize memory banks from the left side
     }

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
    // Reserve is expensive, use emplace_back instead
    // emplace_back constructs the object in place, no need to copy
    // meaning that the object is constructed directly in the vector
    //memoryToSystolicChannels.reserve(SIZE);
    for (int i = 0; i < SIZE; ++i) {
        memoryToSystolicChannels.emplace_back(CHANNEL_CAPACITY);
    }

    // NUMBER OF CYCLES FOR SIMULATION
    // Simulation loop
    // Make this a function in a utility file?
    for (int cycle = 0; cycle < num_cycles; ++cycle) {

        mem.increment(cycle);


        // Memory pushes data into channels
        

       

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
    
}
