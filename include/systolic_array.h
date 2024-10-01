#ifndef SYSTOLIC_ARRAY_H
#define SYSTOLIC_ARRAY_H

#include <array> // For std::array
#include <random> // For std::random_device, std::mt19937, std::uniform_int_distribution
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <vector>
template<typename T>
class channel;

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

    // Constructor to initialize MAC units dynamically
     Construct(const std::string& filename) { // DC
     
     loadWeightsFromFile(filename); // DC

     for (int i = 0; i < SIZE; ++i) {
         for (int j = 0; j < SIZE; ++j) {
             array[i][j] = new MACUnit<T>(i, j);  // Dynamically allocate MACUnit
             array[i][j]->w = weights[i][j]; // DC
         }
     }
 }

    ConstructA(const std::string& filename) { // DC
     
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
        int rowCount = 0; // To track row count in the weights array

        while (std::getline(file, line)) {
            // Check for batch separator (optional)
            if (line == "#") {
                break; // Stop reading if a separator is encountered
            }

            // Read numbers into the current row of the weights array
            std::istringstream iss(line);
            for (int col = 0; col < 8; ++col) {
                if (!(iss >> A[rowCount][col])) {
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
        if (rowCount != 1) {
            throw std::runtime_error("Weights array is not complete. Expected 1 rows but got " + std::to_string(rowCount));
        }

        file.close(); // Close the file
    }


    // Function to get weight at specific position (i, j)
    T GetWeight(int i, int j) const {
        if (i < 0 || i >= 8 || j < 0 || j >= 8) {
            throw std::out_of_range("Index out of range");
        }
        return weights[i][j];
    }







    // Destructor to clean up dynamically allocated memory
    Destruct() {
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

#endif
