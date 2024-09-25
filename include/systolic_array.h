#ifndef SYSTOLIC_ARRAY_H
#define SYSTOLIC_ARRAY_H

#include <array> // For std::array

// Forward declaration of MACUnit
template<typename T>
class MACUnit;

template<typename T>
class Systolic_Array {
public:
    static const int SIZE = 8; // Define the size of the array
    std::array<std::array<MACUnit<T>*, SIZE>, SIZE> array; // Use pointers to MACUnit

    // Constructor to initialize MAC units dynamically
    initialize() {
        for (int i = 0; i < SIZE; ++i) {
            for (int j = 0; j < SIZE; ++j) {
                array[i][j] = new MACUnit<T>(i, j);  // Dynamically allocate MACUnit
            }
        }
    }

    // Destructor to clean up dynamically allocated memory
    ~Systolic_Array() {
        for (int i = 0; i < SIZE; ++i) {
            for (int j = 0; j < SIZE; ++j) {
                delete array[i][j];  // Clean up MACUnit
            }
        }
    }

    // Access operator
    std::array<MACUnit<T>*, SIZE>& operator[](std::size_t index) {
        return array[index];
    }

    // Const access operator
    const std::array<MACUnit<T>*, SIZE>& operator[](std::size_t index) const {
        return array[index];
    }
};

#endif
