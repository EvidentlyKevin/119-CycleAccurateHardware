// File: lib/systolic_array.cpp

#include "systolic_array.h"

// Constructor
template<typename T>
Systolic_Array<T>::Systolic_Array() {
    initialize();
}

// Destructor
template<typename T>
Systolic_Array<T>::~Systolic_Array() {
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            delete array[i][j];  // Clean up MACUnit
        }
    }
}

// Initialize the MAC units
template<typename T>
void Systolic_Array<T>::initialize() {
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            array[i][j] = new MACUnit<T>(i, j);  // Dynamically allocate MACUnit
        }
    }
}

// Access operator
template<typename T>
std::array<MACUnit<T>*, Systolic_Array<T>::SIZE>& Systolic_Array<T>::operator[](std::size_t index) {
    return array[index];
}

// Const access operator
template<typename T>
const std::array<MACUnit<T>*, Systolic_Array<T>::SIZE>& Systolic_Array<T>::operator[](std::size_t index) const {
    return array[index];
}

// Explicit template instantiation for int and float
template class Systolic_Array<int>;
template class Systolic_Array<float>;
