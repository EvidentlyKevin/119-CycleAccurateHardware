#ifndef CLUSTER_H
#define CLUSTER_H

#include <iostream>
#include <vector>
#include <memory>
#include "TPU.h"
#include "memory.h"

template<typename T>
class Cluster {
public:
    //Memory persistentMem;
    Cluster(int size = DEFAULT_SIZE);
    void runAllTPUs();
    void setParametersForTPUs();
    void showbanks();
    void setActivationFunction(int activation);
    int getActivationFunction() const;
    //Memory& getMemory() { return persistentMem; }
    void sendDataToTPUs(const std::vector<std::vector<T>>& data);
    int getSize() const;
    void setTPUData(int i, int j, const std::vector<T>& data);
    void loadFromJson(const std::string& filepath);
    static const int DEFAULT_SIZE = 8;

private:
    int SIZE;
    int activationFunction;
    Memory persistentMem;
    std::vector<std::vector<std::unique_ptr<TPU<T>>>> array;
};

#include "Cluster.tpp"
#endif // CLUSTER_H
