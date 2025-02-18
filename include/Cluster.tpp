#include "Cluster.h"

template<typename T>
Cluster<T>::Cluster(int size) : SIZE(size) {
    array.resize(SIZE);
    for (int i = 0; i < SIZE; ++i) {
        array[i].reserve(SIZE);
        for (int j = 0; j < SIZE; ++j) {
            array[i].push_back(std::make_unique<TPU<T>>(i, j));
        }
    }
}

template<typename T>
void Cluster<T>::setParametersForTPUs() {
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            std::cout << "set parameters for TPU[" << i << "][" << j << "]" << std::endl;
            array[i][j]->setparameters();
        }
    }
}

template<typename T>
void Cluster<T>::runAllTPUs() {
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            std::cout << "output for TPU[" << i << "][" << j << "]" << std::endl;
            array[i][j]->run();
        }
    }
}

template<typename T>
void Cluster<T>::showbanks() {
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            std::cout << "show banks for TPU[" << i << "][" << j << "]" << std::endl;
            array[i][j]->display();
        }
    }
}

template<typename T>
void Cluster<T>::setActivationFunction(int activation) {
    activationFunction = activation;
}

template<typename T>
int Cluster<T>::getActivationFunction() const {
    return activationFunction;
}