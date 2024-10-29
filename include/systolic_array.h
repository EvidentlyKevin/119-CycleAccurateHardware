// File: include/systolic_array.h

#ifndef SYSTOLIC_ARRAY_H
#define SYSTOLIC_ARRAY_H

#include <vector>
#include <memory>
#include <stdexcept>
#include <iostream> // Only include if needed for debug output
#include "mac_unit.h"
#include "channelM.h" // Assuming this is where channelM is defined

template<typename T>
class Systolic_Array {
public:
    // Constructor
    Systolic_Array(int size = DEFAULT_SIZE) : SIZE(size) {
        // Initialize the array of MACUnits using smart pointers
        array.resize(SIZE);
        for (int i = 0; i < SIZE; ++i) {
            array[i].reserve(SIZE); // Reserve space to avoid reallocations
            for (int j = 0; j < SIZE; ++j) {
                array[i].push_back(std::make_unique<MACUnit<T>>(i, j));
            }

            // Connect the MAC units to their neighbors
            for (int i = 0; i < SIZE; ++i) {
                for (int j = 0; j < SIZE; ++j) {
                    // Connect leftIn to the rightOut of the left neighbor
                    if (j > 0) {
                        array[i][j]->setLeftIn(&(array[i][j - 1]->getRightOut()));
                    }

                    // Connect upIn to the downOut of the upper neighbor
                    if (i > 0) {
                        array[i][j]->setUpIn(&(array[i - 1][j]->getDownOut()));
                    }
                }
            }
        }
    }


    // Set weights from a 2D vector
    void setWeights(const std::vector<std::vector<T>>& weights);

    // Set input activations from a vector
    void setInputActivations(const std::vector<T>& activations);

    // Set input activations from channels
    void setInputActivationsFromChannels(std::vector<channelM<T>>& channels, int current_cycle = 0, bool debug = false);

    // Access a specific MAC unit
    std::unique_ptr<MACUnit<T>>& getMACUnit(int row, int col);

    // Run one cycle
    void cycle();

    // Get the outputs from the last column of MAC units
    std::vector<T> getOutputs() const;

    // Access operator for MAC units
    std::unique_ptr<MACUnit<T>>& operator()(int row, int col);

    // Constants
    static const int DEFAULT_SIZE = 8;

private:
    int SIZE;
    std::vector<std::vector<std::unique_ptr<MACUnit<T>>>> array;
};

#include "systolic_array.tpp" // Include implementation

#endif // SYSTOLIC_ARRAY_H
