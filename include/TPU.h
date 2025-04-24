#ifndef TPU_H
#define TPU_H

#include <iostream>
#include <vector>
#include "systolic_array.h"
#include "memory.h"
#include "Port.h"

template<typename T>
class Cluster; // Forward declaration
#include "activation.h" // Include the activation header file



template<typename T>
class TPU {


public:

    Memory mem;

    TPU(int row, int col);
    void run();
    void setparameters();
    void setLeftPort(Port<T>* leftPort);
    void display();
    void setActivationFunction(int activation);
    void sendData(const std::vector<T>& inputData) ;




    Port<T> portR;
    Port<T>* portL = nullptr; // Pointer to the left port
    Port<T>& getRPort();

    int rowID;
    int colID;
    std::vector<T> dequeuedValue;
    int SIZE;
    int ROWS;
    int COLS;
    int num_cycles;

private:
    int activationFunction = 1; // Default activation function is ReLU


};

#include "TPU.tpp"
#endif // MEMORY_H
