// File: include/systolic_array.h

#ifndef SYSTOLIC_ARRAY_H
#define SYSTOLIC_ARRAY_H

#include <array>      // For std::array
#include "mac_unit.h" // Include MACUnit header

// Forward declaration of MACUnit
template<typename T>
class MACUnit;

// Systolic Array class
template<typename T>
class Systolic_Array {
public:
    static const int SIZE = 8; // Define the size of the array
    std::array<std::array<MACUnit<T>*, SIZE>, SIZE> array; // 2D array of MACUnit pointers

    // Constructor
    Systolic_Array();

    // Destructor
    ~Systolic_Array();

    // Access operators
    std::array<MACUnit<T>*, SIZE>& operator[](std::size_t index);
    const std::array<MACUnit<T>*, SIZE>& operator[](std::size_t index) const;

private:
    // Initializes the MAC units in the array
    void initialize();
};

#endif // SYSTOLIC_ARRAY_H
