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
    std::cout << "3: Test Systolic Array Function\n";
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
            Cluster<int> cluster(1); // Create a cluster with 2 TPUs
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

        // Set the activation function in the cluster
        cluster.setActivationFunction(activationChoice);  // You must implement this method in Cluster.

        // Proceed with existing parameter setting and simulation run*/
        cluster.setParametersForTPUs();
            //put memory data in here 

        NetworkStorage storage("/home/user/data", true); 
        storage.push();
        storage.fetch();
        storage.parse();


        // cluster.showbanks(); // if needed
        cluster.runAllTPUs();
        break;
    }
    default:
        std::cout << "Invalid option selected!" << std::endl;
        break;
}

return 0;
}
