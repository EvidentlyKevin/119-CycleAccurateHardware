// File: lib/bank_memory.cpp

#include "../include/memory.h"
#include <random>
#include <iostream>
#include <limits>
#include <fstream> // For file operations
#include <nlohmann/json.hpp> // For JSON parsing

using json = nlohmann::json; // For JSON parsing (if needed)

Memory::Memory() 
    : MemoryBanks(MemBanks, MemBank(BANK_ROWS, BANK_COLS)),
      indices(100, std::vector<int>(100, 0)) ,
// Additional initialization if needed
        z(1), start(3), end(100), group(0) {  // Initialize member variables
            // Initialize memory banks
            initBanks();
        }
    
void Memory::initBanks() {

    if (MemBanks <= 0 || BANK_ROWS <= 0 || BANK_COLS <= 0) {
        std::cerr << "Error: Invalid memory dimensions.\n";
        return;
    }

    std::cout << "Initializing MemoryBanks: MemBanks = " << MemBanks
              << ", BANK_ROWS = " << BANK_ROWS
              << ", BANK_COLS = " << BANK_COLS << std::endl;

    // Removed creation of a local Memory instance.
    // std::random_device rd;
    // std::mt19937 gen(rd());
    // std::uniform_int_distribution<> dis(0, 255);
     
//     for (int i = 0; i < MemBanks; i++) {
//         for (int j = 0; j < BANK_ROWS; j++) {
//             for (int k = 0; k < BANK_COLS; k++) {


//                 // Set to random non-zero values to avoid the zero problem
//                 MemoryBanks[i].Data[j][k] = dis(gen);
                
//                 // Debug output for the first few banks
//                 // if (i < 3 && j < 3) {
//                 //     std::cout << "Bank[" << i << "][" << j << "][" << k << "] = " 
//                 //               << MemoryBanks[i].Data[j][k] << std::endl;
//                 // }
//             }
//         }
//     }
//     std::cout << "Memory banks initialized successfully.\n";
// }

    for (int i = 0; i < MemBanks; i++) {
        for (int j = 0; j < BANK_ROWS; j++) {
            for (int k = 0; k < BANK_COLS; k++) {
                MemoryBanks[i].Data[j][k] = 0;
            }
        }
    }
    std::cout << "Memory banks initialized successfully.\n";
}

void Memory::increment(int cycle) {
    if ((cycle - 3) % 3 == 0) { // specific edge cases for data[0]
        z++;
        if (z > N) {
            z = 1;
        }
    }

    for (int i = start; i < end; i += 6) {
        // Compute the group index for the current value of i.
        group = (i - start) / 6 + 1;

        if (cycle % 3 == 0 && cycle > i) {
            indices[group][0]++;
            indices[group][1]++;
            indices[group][2] = indices[group][1] - 1; // Update indices

            if (indices[group][1] > N - 1) {
                indices[group][0] = 0;
                indices[group][1] = 0;
                indices[group][2] = N - 1;
            }
        }
    }
}

void Memory::pushData(std::vector<channelM<int>> &channels, int cycle, bool debug) {
    size_t numChannels = channels.size();
    double pos_inf = std::numeric_limits<double>::infinity();
    double neg_inf = -std::numeric_limits<double>::infinity();

    // Vector to store the data values to push to each channel.
    std::vector<int> data(numChannels);

    for (size_t i = 0; i < numChannels; ++i) {
        int bankIndex = static_cast<int>(i) / BANK_COLS;
        int colIndex = static_cast<int>(i) % BANK_COLS;
        int rowIndex = 0;

        data[i] = MemoryBanks[bankIndex].Data[rowIndex][colIndex];

        if ((cycle - 3) % 6 == 0) { // specific edge cases for data[0]
            // Make sure that the row index adjustment is within bounds.
            data[0] = MemoryBanks[bankIndex].Data[rowIndex + (z - 1)][colIndex];
        }

        for (int s = start; s < end; s += 6) {
            if (cycle % 3 == 0 && cycle > s) {
                // Loop over groups using a local variable.
                for (int local_g = 1; local_g <= (N / 2); local_g++) {
                    // Break if all three indices in this group are zero.
                    if (indices[local_g][2] == 0 &&
                        indices[local_g][1] == 0 &&
                        indices[local_g][0] == 0) {
                        break;
                    }
                    // If not the final group, update data from two different rows.
                    if (local_g != N / 2) {
                        data[indices[local_g][2]] = MemoryBanks[bankIndex].Data[rowIndex + (local_g * 2)][colIndex];
                        data[indices[local_g][1]] = MemoryBanks[bankIndex].Data[rowIndex + (local_g * 2) - 1][colIndex];
                    }
                    // For the final group, update the data from a specific row.
                    if (local_g == N / 2) { 
                        data[indices[local_g][1]] = MemoryBanks[bankIndex].Data[rowIndex + ((local_g * 2) - 1)][colIndex];
                    }
                    
                    // Break to avoid pushing multiple times into the same channel.
                    if (data[i] >= neg_inf && data[i] <= pos_inf &&
                        data[i] != 1 && data[i] != 0) {
                        channels[i].push(data[i]);
                        break;
                    }
                }
                // Break to prevent further processing and multiple pushes.
                break;
            }
        }

        // Push data into the channel if it is not already full.
        if (!channels[i].is_full()) {
            if (static_cast<int>(i) == (cycle / 3) % static_cast<int>(numChannels)) {
                channels[i].push(data[i]);
                if (cycle == 3) {
                    channels[0].push(data[0]);
                }
            }
            // If debugging is enabled, print details about the push.
            if (debug) {
                std::cout << "Cycle " << cycle << ": Pushed data " << data[i]
                          << " from Bank " << bankIndex << ", Row " << rowIndex
                          << ", Column " << colIndex << " into Channel " << i << "\n";
            }
        }
    }
}

void Memory::loadFromJson(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open JSON file: " << filename << std::endl;
        return;
    }
    json j;
    file >> j;
    file.close();

    MemoryBanks.clear();

    // Assuming j is an array of banks (each bank is a 2D array of ints)
    for (const auto& bankJson : j) {
        std::vector<std::vector<int>> bankData;
        for (const auto& rowJson : bankJson) {
            std::vector<int> row = rowJson.get<std::vector<int>>();
            bankData.push_back(row);
        }
        // Create a MemBank from the loaded data
        MemBank bank(static_cast<int>(bankData.size()), bankData.empty() ? 0 : static_cast<int>(bankData[0].size()));
        bank.Data = bankData;
        MemoryBanks.push_back(bank);
    }
    std::cout << "Memory banks loaded from JSON successfully." << std::endl;
}