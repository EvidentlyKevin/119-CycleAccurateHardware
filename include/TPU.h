#ifndef TPU_H
#define TPU_H

#include <iostream>
#include <vector>
#include "systolic_array.h"
#include "memory.h"
#include "activation.h" // Include the activation header file



template<typename T>
class TPU {


public:

TPU(int row, int col);
void run(Memory& mem);
void setparameters();
void display(Memory& mem);


    // Add a setter for the activation function selection
    // For example: 1 = ReLU, 2 = Sigmoid, 3 = Tanh, 4 = GELU
    void setActivationFunction(int activation);
    void sendData(const std::vector<T>& inputData, Memory& mem); // Send data to the TPU memory banks;
    // Send data to the TPU memory banks
    //mem.pushData(data, rowID, colID);



private:

int rowID;
int colID;
int SIZE;
int ROWS;
int COLS;
int num_cycles;
int activationFunction;
};

#include "TPU.tpp"
#endif // MEMORY_H
