#include "../include/network_storage.h"
#include <fstream> 
#include <sstream> //
#include <vector> // For std::vector
#include <string>
#include <cstdlib> // For std::exit
#include <iostream>
#include <filesystem>
 
namespace fs = std::filesystem;

//Constructor
NetworkStorage::NetworkStorage(std::string path, bool debug) : storage(path) {
    if (debug) {
        std::cout << "NetworkStorage initialized with path: " << storage << std::endl;
    }
}
std::string network_path = "/mnt/network_weights"; // Update this path

void NetworkStorage::parse() {
    // Implementation of the parse method
    
    if (!fs::exists(network_path)) {
        std::cerr << "Error: Network storage path does not exist!" << std::endl;
        return;
    }
    for (const auto& entry : fs::directory_iterator(storage)) {
        if (entry.path().extension() == ".csv" || entry.path().extension() == ".bin") { // Only parse relevant weight files
            std::ifstream file(entry.path());
            if (!file) {
                std::cerr << "Error opening file: " << entry.path() << std::endl;
                continue;
            }
    }
}

void NetworkStorage::push() {
    // Implementation of the push method
    
    if (!fs::exists(network_path)) {
        std::cerr << "Error: Network storage path does not exist!" << std::endl;
        return;
    }

    for (const auto& file : fs::directory_iterator(storage)) {
        if (file.path().extension() == ".csv" || file.path().extension() == ".bin") { // Only push relevant weight files
            std::string cmd = "mv " + file.path().string() + " " + network_path;
            std::cout << "Executing: " << cmd << std::endl;
            int result = system(cmd.c_str());
            if (result == 0) {
                std::cout << "Successfully moved: " << file.path().filename().string() << std::endl;
            } else {
                std::cerr << "Error moving file: " << file.path().filename().string() << std::endl;
            }
        }
    }
}

}

std::string NetworkStorage::fetch() {

    std::cout << "Fetching data files from network storage...\n";

    std::string network_path = "/mnt/network_weights"; // Update this path
    if (!fs::exists(network_path)) {
        std::cerr << "Error: Network storage path does not exist!" << std::endl;
        return "";
    return "Fetch complete";
    // Implementation of the fetch method
    for (const auto& file : fs::directory_iterator(network_path)) {
        if (file.path().extension() == ".csv") { // Only fetch data files
            std::string destination = storage + "/" + file.path().filename().string();
            std::string cmd = "cp " + file.path().string() + " " + destination;
            int result = system(cmd.c_str());

            if (result == 0) {
                std::cout << "Successfully fetched: " << file.path().filename().string() << std::endl;
            } else {
                std::cerr << "Error fetching file: " << file.path().filename().string() << std::endl;
            }
        }
    }
    return "Fetch complete";
}
}
void NetworkStorage::loadBankMem(const std::string& filename) { //might not need
    std::string filepath = storage + "/" + filename;
    std::ifstream file(filepath);

    if (!file.is_open()) {
        std::cerr << "Error opening data file: " << filename << std::endl;
        return;
    }

    std::ofstream bankMemory("bank_memory.cpp", std::ios::trunc);
    if (!bankMemory.is_open()) {
        std::cerr << "Error opening bank memory file!" << std::endl;
        return;
    }

    std::string line;
    bankMemory << "// Bank Memory Data from " << filename << "\n";
    while (std::getline(file, line)) {
        bankMemory << line << "\n"; // Copy data directly
    }

    file.close();
    bankMemory.close();
    std::cout << "Bank memory populated with data from: " << filename << std::endl;
}

void NetworkStorage::setParameters() {
    // Implementation of the setParameters method
}

void NetworkStorage::showConfig() const {
    // Implementation of the showConfig method
}
