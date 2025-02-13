#include "host_memory.h"

void hostMemory::init() {
    // Initialize memory with zeros
    for (size_t i = 0; i < 16; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            memory[i][j] = 0.0;
        }
    }
}

void hostMemory::getData(double x,int r,int c) {
    // Refresh memory (example: randomizing values)
    memory[r][c] = static_cast<double>(x);  // Example update
      
}

void hostMemory::saveToFile(const std::string& filename, int cycle) {
    std::ofstream file(filename, std::ios::app); // Open in append mode
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << " for writing." << std::endl;
        return;
    }

    file << "Cycle: " << cycle << "\n";  // Label each cycle
    for (size_t i = 0; i < 16; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            file << "Row: " << i << " Col: " << j << " Value: " << memory[i][j];
            if (j < cols - 1) file << ",";
        }
        file << "\n";
    }
    file << "--------------------------------\n"; // Separate cycles for readability

    file.close();
}

