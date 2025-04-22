#include <iostream>
#include <vector>
#include "../include/Cluster.h"
#include "../include/memory.h"
#include "../include/systolic_array.h"
#include "../include/activation.h"
#include "../include/network_storage.h"
#include <nlohmann/json.hpp> // For JSON parsing (if needed)
#include <fstream> //file operations


void memoryFunction() {
    Memory mem;
    mem.initBanks(); // No parameters needed


    // Display the contents of the memory banks
    for (int i = 0; i < MemBanks; i++) {
        std::cout << "Memory Bank " << i << ":\n";
        for (int j = 0; j < BANK_ROWS; j++) {
            for (int k = 0; k < BANK_COLS; k++) {
                std::cout << mem.MemoryBanks[i].Data[j][k] << " ";
            }
            std::cout << std::endl;
        }
        std::cout << "---------------------------\n";
    }
}

int main() {

    int testOption;
   
    // What do you want to test
    std::cout << "Enter a test option (1-3):\n";
    std::cout << "1: Load and display inference data (JSON)\n";
    std::cout << "2: Test Memory Function\n";
    std::cout << "3: Run Full Test Systolic Array Function\n";
    std::cout << "Option: ";
    std::cin >> testOption;
    std::cout << "--------------------------" << std::endl;

    // Declare filepath outside the switch-case to avoid bypassing initialization
    // std::string filepath;
    //std::vector<std::vector<float>> inferenceData= loadInferenceData(filepath); // Initialize with empty data

    // Use switch-case to handle different options
    switch (testOption) {
        case 1:
        {
           
            //std::string filepath = "output/tpu_inference_data.json";
            std::string filepath = "../output/tpu_inference_data.json";

            std::vector<std::vector<float>> inferenceData = loadInferenceData(filepath);

            if (inferenceData.empty()) {
                std::cerr << "Error: No data loaded from the JSON file.\n";
                return 1;
            }

            std::cout << "Loaded " << inferenceData.size() << " rows.\n";
            std::cout << "Each row has " << inferenceData[0].size() << " columns.\n";

            for (const auto& row : inferenceData) {
                for (float val : row) {
                    std::cout << val << " ";
                }
                std::cout << "\n";
            }
            break;
        }
    case 2:
        memoryFunction();
        break;
    case 3:{
        std::string filepath = "../output/tpu_inference_data.json";
        std::vector<std::vector<float>> inferenceData = loadInferenceData(filepath);

        if (inferenceData.empty()) {
            std::cerr << "Error: Could not load inference data.\n";
            return 1;
        }

        // Flatten the 2D inference data into a 1D vector
        std::vector<float> flattenedData;
        for (const auto& row : inferenceData) {
            flattenedData.insert(flattenedData.end(), row.begin(), row.end());
        }

        // Calculate total memory capacity
        int totalCapacity = MemBanks * BANK_ROWS * BANK_COLS;

        if (flattenedData.size() > totalCapacity) {
            std::cerr << "Error: Input data size (" << flattenedData.size()
                      << ") exceeds memory capacity (" << totalCapacity << ").\n";
            return 1;
        }

         Cluster<float> cluster(4); // Create a cluster with 4x4 TPus
            // Set the parameters for the TPUs
        
        int activationChoice;
        std::cout << "Select Activation Function:\n";
        std::cout << "1: ReLU\n";
        std::cout << "2: Sigmoid\n";
        std::cout << "3: Tanh\n";
        std::cout << "4: GELU\n";
        std::cout << "Option: ";
        std::cin >> activationChoice;
        std::cout << "--------------------------" << std::endl;

        // Proceed with existing parameter setting and simulation run*/
        cluster.setParametersForTPUs();
        NetworkStorage storage("../output", true);
        storage.push(cluster, inferenceData);  // Push JSON data into TPU memory

            //put memory data in here
         // Set the activation function in the cluster
        cluster.setActivationFunction(activationChoice);  // You must implement this method in Cluster.

        //NetworkStorage storage("/home/user/data", true); 
        cluster.runAllTPUs();            // Run systolic array
        cluster.showbanks();             // Show memory outputs
      
        break;
    }
    default:
        std::cout << "Invalid option selected!" << std::endl;
        break;
}

return 0;
}
