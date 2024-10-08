#ifndef SYSTOLIC_ARRAY_H
#define SYSTOLIC_ARRAY_H

#include <array> 
#include <random> 
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <vector>

template<typename T>
class channelM;

// Forward declaration of MACUnit
template<typename T>
class MACUnit;

template<typename T>
class Systolic_Array {
public:
    static const int SIZE = 8; // Define the size of the array
    std::array<std::array<MACUnit<T>*, SIZE>, SIZE> array; // Use pointers to MACUnit
    std::array<std::array<int, 8>, 8> weights;
    std::array<std::array<int, 1>, 8> A;
    std::array<std::array<int, SIZE>, SIZE> bank; 


 
// Constructor to initialize MAC units dynamically
    void Construct(const std::string& filename) { // DC
    
    loadWeightsFromFile(filename); // DC

    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            array[i][j] = new MACUnit<T>(i, j);  // Dynamically allocate MACUnit
            array[i][j]->w = weights[i][j]; // DC
        }
    }
}

    void ConstructA(const std::string& filename) { // DC
     
     loadWeightsFromFileA(filename); // DC

     for (int i = 0; i < 1; ++i) {
         for (int j = 0; j < SIZE; ++j) {
            array[i][j]->a = A[i][j];       }
     }
 }


    // Function to load weights from a file
     void loadWeightsFromFile(const std::string& filename) {
        std::ifstream file(filename); // Open the file
        if (!file) {
            throw std::runtime_error("Could not open the file!");
        }

        std::string line;
        int rowCount = 0; // To track row count in the weights array

        while (std::getline(file, line)) {
            // Check for batch separator (optional)
            if (line == "#") {
                break; // Stop reading if a separator is encountered
            }

            // Read numbers into the current row of the weights array
            std::istringstream iss(line);
            for (int col = 0; col < 8; ++col) {
                if (!(iss >> weights[rowCount][col])) {
                    throw std::runtime_error("Error reading number at row " + std::to_string(rowCount + 1));
                }
                // Ignore commas
                if (iss.peek() == ',') {
                    iss.ignore();
                }
            }
            rowCount++;
        }

        // Check if we have filled the weights array completely
        if (rowCount != 8) {
            throw std::runtime_error("Weights array is not complete. Expected 8 rows but got " + std::to_string(rowCount));
        }

        file.close(); // Close the file
    }

     void loadWeightsFromFileA(const std::string& filename) {
         std::ifstream file(filename); // Open the file
         if (!file) {
             throw std::runtime_error("Could not open the file!");
         }

         std::string line;
         int count = 0; // To track the number of values read

         // Read numbers from the file into A[0] (the single row)
         while (std::getline(file, line)) {
             // Check for batch separator (optional)
             if (line == "#") {
                 break; // Stop reading if a separator is encountered
             }

             std::istringstream iss(line);
             int value;

             // Read up to 8 values into A[0]
             while (iss >> value) {
                 if (count >= SIZE) { // Check if we've filled the row
                     throw std::runtime_error("Too many values in the file. Expected " + std::to_string(SIZE) + ".");
                 }
                 A[0][count++] = value; // Store value in A[0][column]
                 // Ignore commas
                 if (iss.peek() == ',') {
                     iss.ignore();
                 }
             }
             if (count == SIZE) break; // Stop if we have filled the row
         }

         // Check if we have filled the A array completely
         if (count != SIZE) {
             throw std::runtime_error("Weights array is not complete. Expected " + std::to_string(SIZE) + " values but got " + std::to_string(count) + ".");
         }

         file.close(); // Close the file
     }

    







    // Destructor to clean up dynamically allocated memory
    void Destruct() {
        for (int i = 0; i < SIZE; ++i) {
            for (int j = 0; j < SIZE; ++j) {
                delete array[i][j];  
            }
        }
    }

    // Access operator
    std::array<MACUnit<T>*, SIZE>& operator[](std::size_t index) {
        return array[index];
    }

    // Const access operator
    const std::array<MACUnit<T>*, SIZE>& operator[](std::size_t index) const {
        return array[index];
    }
};

#endif // SYSTOLIC_ARRAY_H