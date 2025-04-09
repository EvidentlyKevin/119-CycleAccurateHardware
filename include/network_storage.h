#pragma once

#include"Cluster.h"
#include <string>
#include <iostream>
#include <stdexcept>
#include <memory>
#include <vector>
#include <filesystem> // for directory and file handling

class NetworkStorage {
public:
    // Constructor
    NetworkStorage(std::string path = "./storage", bool debug = false);
    // Methods  
    void parse();
    std::string fetch();
    
    void push(Cluster<float>& cluster, const std::vector<std::vector<float>>& data);

    void fetchDebug();
    void showConfig() const;
   
private:
    std::string storage;
};

std::vector<std::vector<float>> loadInferenceData(const std::string& filepath);

