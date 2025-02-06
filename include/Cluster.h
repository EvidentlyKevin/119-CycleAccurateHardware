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

    static const int DEFAULT_SIZE = 8;










private:
int SIZE;
std::vector<std::vector<std::unique_ptr<TPU<T>>>> array;

};

#include "Cluster.tpp"

#endif // CLUSTER_H
