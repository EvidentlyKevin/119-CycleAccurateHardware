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

   cout << "note: Bank Parameters should be as follows MemBanks >= SIZE, Rows = SIZE, Cols = SIZE / 4" << endl;
    cout << "8x8: 70 cycles" << endl;
    cout << "16x16: 140 cycles" << endl;
    cout << "32x32: 284 cycles" << endl;
    cout << "64x64: 574 cycles" << endl;
    cout << "128x128: 766 cycles" << endl;
    cout << "256x256: 1534 cycles" << endl;
    cout << "512x512: 4000 cycles" << endl;
    cout << "1024x1024: 8000 cycles" << endl;
    cout << "Further testing is needed to find the number of cycles for the for other systolic array sizes" << endl;
   
   cout << "Enter the size of the systolic array: ";
   cin >> SIZE;
   cout << "--------------------------" << endl;
   cout << "Enter the number of rows for Bank Memory: ";
   cin >> ROWS;
   cout << "--------------------------" << endl;
   cout << "Enter the number of columns for Bank Memory: ";
   cin >> COLS;
   cout << "--------------------------" << endl;
   cout << "Enter the number of memory banks: ";
   cin >> MemBanks;
   cout << "--------------------------" << endl;
   cout << "Enter number of cycles for simulation: ";
   cin >> num_cycles;
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
