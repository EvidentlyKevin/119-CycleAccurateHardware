// File: include/systolic_array.tpp

#include "systolic_array.h"

template<typename T>
Systolic_Array<T>::Systolic_Array(int size) : SIZE(size) {
    array.resize(SIZE);
    for (int i = 0; i < SIZE; ++i) {
        array[i].reserve(SIZE);
        for (int j = 0; j < SIZE; ++j) {
            array[i].push_back(std::make_unique<MACUnit<T>>(i, j));
        }
    }

    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            if (j > 0) {
                array[i][j]->setLeftIn(&(array[i][j - 1]->getRightOut()));
            }
            if (i > 0) {
                array[i][j]->setUpIn(&(array[i - 1][j]->getDownOut()));
            }
        }
    }
}

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
void Systolic_Array<T>::cycle(int act) {
    for (int i = SIZE - 1; i >= 0; --i) {
        for (int j = SIZE - 1; j >= 0; --j) {
            // Only apply the activation function in the final (output) column.
            int act_to_use = (j == SIZE - 1) ? act : 0;
            array[i][j]->cycle(act_to_use);
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