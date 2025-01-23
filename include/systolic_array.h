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
    Systolic_Array(int size = DEFAULT_SIZE);

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