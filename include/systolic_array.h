// File: include/systolic_array.h

#ifndef SYSTOLIC_ARRAY_H
#define SYSTOLIC_ARRAY_H

#include <vector>
#include <memory>
#include <stdexcept>
#include "mac_unit.h"

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
    void setWeights(const std::vector<std::vector<T>>& weights) {
        for (int i = 0; i < SIZE; ++i) {
            for (int j = 0; j < SIZE; ++j) {
                array[i][j]->setWeight(weights[i][j]);
            }
        }
    }

    // Set input activations from a vector
    void setInputActivations(const std::vector<T>& activations) {
        for (int j = 0; j < SIZE; ++j) {
            array[0][j]->setInputActivation(activations[j]);
        }
    }

    void setInputActivationsFromChannels(std::vector<channelM<T>>& channels,  int current_cycle = 0, bool debug = false) {
        for (int j = 0; j < SIZE; ++j) {
            T activation;
            if (channels[j].pop(activation)) {
                array[0][j]->setInputActivation(activation);

                // Debugging: Print the activation read
                if (debug) {
                    std::cout << "Cycle " << current_cycle << ": MAC[0][" << j 
                              << "] received activation " << activation << "\n";
                }
            }
        }
    }

    std::unique_ptr<MACUnit<T>>& getMACUnit(int row, int col) {
        if (row < 0 || row >= SIZE || col < 0 || col >= SIZE) {
            throw std::out_of_range("Invalid MAC unit coordinates");
        }
        return array[row][col];
    }

    // Run one cycle
    void cycle() {
        // All MAC units perform cycle simultaneously
        for (int i = SIZE - 1; i >= 0; --i) {
            for (int j = SIZE - 1; j >= 0; --j) {
                array[i][j]->cycle();
            }
        }
    }

    // Get the outputs from the last column of MAC units
    std::vector<T> getOutputs() const {
        std::vector<T> outputs(SIZE);
        for (int i = 0; i < SIZE; ++i) {
            outputs[i] = array[i][SIZE - 1]->readAccumulator();
        }
        return outputs;
    }

    // Access operator to allow MAC units to access their neighbors
    std::unique_ptr<MACUnit<T>>& operator()(int row, int col) {
        return array[row][col];
    }

    // Constants
    static const int DEFAULT_SIZE = 8;

private:
    int SIZE;
    std::vector<std::vector<std::unique_ptr<MACUnit<T>>>> array;
};

#endif // SYSTOLIC_ARRAY_H
