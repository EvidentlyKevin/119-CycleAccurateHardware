// File: include/systolic_array.tpp

#include "systolic_array.h"

// Constructor definition removed to avoid redefinition error

template<typename T>
void Systolic_Array<T>::setWeights(const std::vector<std::vector<T>>& weights) {
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            array[i][j]->setWeight(weights[i][j]);
        }
    }
}

template<typename T>
void Systolic_Array<T>::setInputActivations(const std::vector<T>& activations) {
    for (int j = 0; j < SIZE; ++j) {
        array[0][j]->setInputActivation(activations[j]);
    }
}

template<typename T>
void Systolic_Array<T>::setInputActivationsFromChannels(std::vector<channelM<T>>& channels, int current_cycle, bool debug) {
    for (int j = 0; j < SIZE; ++j) {
        T activation;
        if (channels[j].pop(activation)) {
            array[0][j]->setInputActivation(activation);
            if (debug) {
                std::cout << "Cycle " << current_cycle << ": MAC[0][" << j << "] received activation " << activation << "\n";
            }
        }
    }
}

template<typename T>
std::unique_ptr<MACUnit<T>>& Systolic_Array<T>::getMACUnit(int row, int col) {
    if (row < 0 || row >= SIZE || col < 0 || col >= SIZE) {
        throw std::out_of_range("Invalid MAC unit coordinates");
    }
    return array[row][col];
}

template<typename T>
void Systolic_Array<T>::cycle() {
    for (int i = SIZE - 1; i >= 0; --i) {
        for (int j = SIZE - 1; j >= 0; --j) {
            array[i][j]->cycle();
        }
    }
}

template<typename T>
std::vector<T> Systolic_Array<T>::getOutputs() const {
    std::vector<T> outputs(SIZE);
    for (int i = 0; i < SIZE; ++i) {
        outputs[i] = array[i][SIZE - 1]->readAccumulator();
    }
    return outputs;
}

template<typename T>
std::unique_ptr<MACUnit<T>>& Systolic_Array<T>::operator()(int row, int col) {
    return array[row][col];
}
