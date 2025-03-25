/*
Network storage class handles storing, parsing, and retrieving data.

*/

#include "../include/network_storage.h"
#include <nlohmann/json.hpp> // For JSON parsing (if needed)
#include <fstream> //file operations
#include <sstream> // for string stream
#include <vector> // For std::vector
#include <string> // for string manipulations
#include <cstdlib> // For std::exit
#include <iostream> // for output
#include <filesystem> // for directory and file handling
 
namespace fs = std::filesystem;
using json = nlohmann::json; // For JSON parsing (if needed)
//Constructor
NetworkStorage::NetworkStorage(std::string path, bool debug) : storage(path) {
    if (debug) {
        std::cout << "NetworkStorage initialized with path: " << storage << std::endl;
    }
}
std::string network_path = "output"; // Update this path and fix it 

// Function to load inference data from a JSON file
std::vector<std::vector<float>>loadInferenceData(const std::string& filepath){
    std::ifstream file (filepath);
    std::vector<std::vector<float>> data; // 2D vector to hold the data
   // return data;

    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filepath << std::endl;
        return data; // Return empty data if file cannot be opened

    }

    json j;
    file >> j; // Parse the JSON file
    file.close(); // Close the file

    for (const auto& row : j) { // Iterate over the JSON array
        std::vector<float> rowData; // Vector to hold each row of data
        for (const auto& value : row) { // Iterate over each value in the row
            rowData.push_back(value.get<float>()); // Convert to float and add to the row
        }
        data.push_back(rowData); // Add the row to the 2D vector
    }


return data;


}

void NetworkStorage::parse() {
    // Implementation of the parse method
    
    if (!fs::exists(storage)) { // Check if the storage path exists
        std::cerr << "Error: Network storage path does not exist!" << std::endl;
        return;
    }
    for (const auto& entry : fs::directory_iterator(storage)) { // Iterate over files in the storage directory
        if (entry.path().extension() == ".json") { // don't need .bin can remove later 
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
        fs::create_directories(network_path); // Create directory if it doesn't exist
        std::cerr << "Error: Network storage path does not exist!" << std::endl;
        return;
    }

    for (const auto& file : fs::directory_iterator(storage)) {
        if (file.path().extension() == ".csv" || file.path().extension() == ".bin" || file.path().extension() == ".json") { 
            std::string destination = network_path + "/" + file.path().filename().string();
            
            // for loop to get into cluster and iterate thru every tpu 
            // the pushed data should be doing this 

            


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
        if (file.path().extension() == ".json") { // Only fetch required files
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
//optional: memory remapping 
//void NetworkStorage::loadBankMem() {
//

void NetworkStorage::fetchDebug() {
    // Implementation of the setParameters method
}

//void NetworkStorage::showConfig() const {
    // Implementation of the showConfig method


