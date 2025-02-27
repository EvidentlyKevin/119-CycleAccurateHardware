#include "TPU.h"

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
    SIZE = 8;
    std::cout << "--------------------------" << std::endl;
    std::cout << "Enter the number of rows for Bank Memory: ";
    // std::cin >> ROWS;
    ROWS = 8;
    std::cout << "--------------------------" << std::endl;
    std::cout << "Enter the number of columns for Bank Memory: ";
    // std::cin >> COLS;
    COLS = 2;
    std::cout << "--------------------------" << std::endl;
    std::cout << "Enter the number of memory banks: ";
    // std::cin >> MemBanks;
    MemBanks = 8;
    std::cout << "--------------------------" << std::endl;
    std::cout << "Enter number of cycles for simulation: ";
    // std::cin >> num_cycles;
    num_cycles = 100;
    std::cout << "Enter the activation function: " << std::endl << "1: ReLU" << std::endl << "2: Sigmoid" << std::endl << "3: Tanh" << std::endl << "4: Gelu" << std::endl;
    // std::cin >> activationFunction;
    activationFunction = 1;
    std::cout << "--------------------------" << std::endl;
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

    // Make membank init cycle offset
    int memoffset = (SIZE * SIZE) / 2;


    for (int cycle = 0; cycle < num_cycles; ++cycle) {
        mem.increment(cycle);

        // Set input activations from memory channels
        if (cycle % 3 == 0 || cycle == 0) {
            mem.pushData(memoryToSystolicChannels, cycle, memoffset, true);
            systolicArray.setInputActivationsFromChannels(memoryToSystolicChannels, cycle, false);
        }
        // Run one cycle of the systolic array
        systolicArray.cycle();

        // Debugging: Print activations read by the systolic array
        for (int j = 0; j < SIZE; ++j) {
            // int activation = systolicArray.getMACUnit(0, j)->getLastActivation();
            // std::cout << "MAC[0][" << j << "] activation: " << activation << "\n";
        }
    }

    // Get the outputs from the systolic array
    std::vector<int> outputs = systolicArray.getOutputs();

    // Print the outputs
    std::cout << "Systolic Array Outputs with Memory Input:\n";
    for (size_t i = 0; i < outputs.size(); ++i) {
        std::cout << "Output[" << i << "]: " << outputs[i] << "\n";
        //std::cout << "---------------------------\n";
        //std::cout << memoffset << std::endl;
    }
}
