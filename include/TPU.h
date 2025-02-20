#ifndef TPU_H
#define TPU_H

#include <iostream>
#include <vector>
#include "systolic_array.h"
#include "memory.h"
#include "activation.h"



template<typename T>
class TPU {


public:

Memory mem;

TPU(int row, int col);
void run();
void setparameters();
void display();

int rowID;
int colID;
int SIZE;
int ROWS;
int COLS;
int num_cycles;
int activationFunction;

private:


};

#include "TPU.tpp"
#endif // MEMORY_H
