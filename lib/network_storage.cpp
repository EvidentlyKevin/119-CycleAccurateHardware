/*
Network storage class handles storing, parsing, and retrieving data.

*/

#include "../include/network_storage.h"
#include <fstream> //file operations
#include <sstream> // for string stream
#include <vector> // For std::vector
#include <string> // for string manipulations
#include <cstdlib> // For std::exit
#include <iostream> // for output
#include <filesystem> // for directory and file handling
 
namespace fs = std::filesystem;

//Constructor
NetworkStorage::NetworkStorage(std::string path, bool debug) : storage(path) {
    if (debug) {
        std::cout << "NetworkStorage initialized with path: " << storage << std::endl;
    }
}
std::string network_path = "\tpu_inference_data.json"; // Update this path

void NetworkStorage::parse() {
    // Implementation of the parse method
    
    if (!fs::exists(storage)) { // Check if the storage path exists
        std::cerr << "Error: Network storage path does not exist!" << std::endl;
        return;
    }
    for (const auto& entry : fs::directory_iterator(storage)) { // Iterate over files in the storage directory
        if (entry.path().extension() == ".csv" || entry.path().extension() == ".bin"|| entry.path().extension() == ".json") { // don't need .bin can remove later 
            std::ifstream file(entry.path());
            if (!file) {
                std::cerr << "Error opening file: " << entry.path() << std::endl;
                continue;
            }
            std::cout << "Parsed file: " << entry.path().filename().string() << std::endl;
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
        if (file.path().extension() == ".csv" || file.path().extension() == ".bin" || file.path().extension() == ".json") { 
            std::string destination = network_path + "/" + file.path().filename().string();
            
            try {
                fs::rename(file.path(), destination);
                std::cout << "Successfully moved file: " << file.path().filename().string() << std::endl;
            } catch (const fs::filesystem_error& e) {
                std::cerr << "Error moving file: " << file.path().filename().string() << " - " << e.what() << std::endl;
            }
        }
    }
}

std::string NetworkStorage::fetch() {
    std::cout << "Fetching data files from network storage...\n"; // Debugging (can remove later)

    if (!fs::exists(network_path)) {
        std::cerr << "Error: Network storage path does not exist!" << std::endl;
        return "";
    }

    // Implementation of the fetch method
    for (const auto& file : fs::directory_iterator(network_path)) {
        if (file.path().extension() == ".csv" || file.path().extension() == ".json") { // Only fetch required files
            std::string destination = storage + "/" + file.path().filename().string(); // Destination path

            // Copy file using filesystem 
            try {
                fs::copy_file(file.path(), destination, fs::copy_options::overwrite_existing);
                std::cout << "Successfully fetched: " << file.path().filename().string() << std::endl;
            } catch (const fs::filesystem_error& e) {
                std::cerr << "Error fetching file: " << file.path().filename().string() << " - " << e.what() << std::endl;
            }
        }
    }
    return "Fetch complete";
}

void NetworkStorage::setParameters() {
    // Implementation of the setParameters method
}

void NetworkStorage::showConfig() const {
    // Implementation of the showConfig method
}
