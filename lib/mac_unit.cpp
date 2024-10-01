// File: lib/mac_unit.cpp

#include "mac_unit.h"

// Constructor
template<typename T>
MACUnit<T>::MACUnit(int i, int j)
    : accumulator(0), rowID(i), colID(j), rightOut(128), downOut(128), w(1) {}

// Perform the MAC operation
template<typename T>
void MACUnit<T>::cycle(Systolic_Array<T>& systolic_array) {
    if(rowID > 0 && colID > 0){
        T a = systolic_array[rowID-1][colID]->downOut.channel_pop();
        T b = systolic_array[rowID][colID-1]->rightOut.channel_pop();
        accumulator = (a * w) + b;
        rightOut.channel_push(accumulator);
        downOut.channel_push(a);
    }
    else if(rowID == 0 && colID == 0){
        T a = 1;
        T b = 1;
        accumulator = (a * w) + b;
        rightOut.channel_push(accumulator);
        downOut.channel_push(a);
    }
    else if(rowID == 0 && colID > 0){
        T a = 1;
        T b = systolic_array[0][colID-1]->rightOut.channel_pop();
        accumulator = (a * w) + b;
        rightOut.channel_push(accumulator);
        downOut.channel_push(a);
    }
    else if(colID == 0 && rowID > 0){   
        T a = systolic_array[rowID-1][0]->downOut.channel_pop();
        T b = 1;
        accumulator = (a * w) + b;
        rightOut.channel_push(accumulator);
        downOut.channel_push(a);
    }
}

// Read the accumulator
template<typename T>
T MACUnit<T>::read_accumulator() const {
    return accumulator;
}

// Clear the accumulator
template<typename T>
void MACUnit<T>::clear_accumulator() {
    accumulator = 0;
}

// Explicit template instantiation for int and float
template class MACUnit<int>;
template class MACUnit<float>;
