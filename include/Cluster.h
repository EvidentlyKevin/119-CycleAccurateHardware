#ifndef CLUSTER_H
#define CLUSTER_H

#include <iostream>
#include <vector>
#include <stdexcept>
#include "TPU.h"

template<typename T>
class Cluster {
public:

// Constructor
    Cluster(int size = DEFAULT_SIZE);
    void runAllTPUs();
    void setParametersForTPUs(); 
    void showbanks(); 
    // Methods to set and get the activation function choice
    void setActivationFunction(int activation);
    int getActivationFunction() const;

    static const int DEFAULT_SIZE = 8;








private:
int SIZE;
int activationFunction = 1; // Default activation function is ReLU
std::vector<std::vector<std::unique_ptr<TPU<T>>>> array;


};

#include "Cluster.tpp"

#endif // CLUSTER_H
