#pragma once

#include <string>
#include <iostream>
#include <stdexcept>
#include <memory>

class NetworkStorage {
public:
    // Constructor
    NetworkStorage(std::string path = "./storage", bool debug = false);

    // Methods
    void parse();
    std::string fetch();
    void push();
    void setParameters();
    void showConfig() const;
    void loadBankMem();
private:
    std::string storage;
};
