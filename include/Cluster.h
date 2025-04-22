#ifndef CLUSTER_H
#define CLUSTER_H

#include <iostream>
#include <vector>
#include <stdexcept>
#include "Port.h"
#include <memory>
#include "TPU.h"

template<typename T>
class Cluster {
public:

// Constructor
    Cluster(int size = DEFAULT_SIZE);
    void runTPUS();
    void setParametersForTPUs(); 
   // void showbanks(); 
    // Methods to set and get the activation function choice
    void setActivationFunction(int activation);
    void sendDataToTPUs(const std::vector<T>& data);
    int getActivationFunction() const;
    int getSize() const;
    void setTPUData(int i, int j, const std::vector<T>& data);
    static const int DEFAULT_SIZE = 8;

private:
    int SIZE;
    int activationFunction = 1; // Default activation function is ReLU
    std::vector<std::vector<std::unique_ptr<TPU<T>>>> array;

};

#include "Cluster.tpp"

#endif // CLUSTER_H
