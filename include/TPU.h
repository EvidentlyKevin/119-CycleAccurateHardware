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

Memory mem;

TPU(int row, int col);
void run();
void setparameters();
void display();


    // Add a setter for the activation function selection
    // For example: 1 = ReLU, 2 = Sigmoid, 3 = Tanh, 4 = GELU
void setActivationFunction(int activation);


int rowID;
int colID;
int SIZE;
int ROWS;
int COLS;
int num_cycles;

private:

int activationFunction = 1; // Default activation function is ReLU
};

#include "TPU.tpp"
#endif // MEMORY_H
