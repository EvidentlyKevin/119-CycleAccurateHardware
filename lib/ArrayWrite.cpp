#include "ArrayWrite.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <cctype>
#include <regex>

// A helper function to trim whitespace from both ends of a string (index-based).
static inline std::string trim(const std::string &str) {
    if (str.empty()) {
        return str;
    }
    size_t first = 0;
    size_t last = str.size() - 1;
    while (first < str.size() && std::isspace(static_cast<unsigned char>(str[first]))) {
        first++;
    }
    while (last > first && std::isspace(static_cast<unsigned char>(str[last]))) {
        last--;
    }
    if (first > last) {
        return "";
    }
    return str.substr(first, last - first + 1);
}

// Uses regex to extract all numeric tokens from a bracketed list.
std::vector<float> SystolicArray::parseBracketedList(const std::string &str) {
    std::vector<float> result;
    std::regex number_regex("[-+]?[0-9]*\\.?[0-9]+");
    auto numbers_begin = std::sregex_iterator(str.begin(), str.end(), number_regex);
    auto numbers_end = std::sregex_iterator();
    for (std::sregex_iterator i = numbers_begin; i != numbers_end; ++i) {
        std::smatch match = *i;
        try {
            float val = std::stof(match.str());
            result.push_back(val);
        } catch (const std::exception&) {
            std::cerr << "Error parsing float from token: " << match.str() << std::endl;
        }
    }
    return result;
}

// Helper: compute product of elements in an int vector.
static int product(const std::vector<int>& dims) {
    int prod = 1;
    for (int d : dims) {
        prod *= d;
    }
    return prod;
}

// w_fetch: Reads the file and, for each non-blank line, creates a Layer.
// The dimension token is expected to be a bracketed list (e.g. "[1.0, 512.0]").
// The dimensions are preserved exactly (converted to ints) and the total number of data elements
// is computed as the product of these dimensions.
bool SystolicArray::w_fetch(const std::string& filename) {
    std::ifstream infile(filename);
    if (!infile.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return false;
    }
    
    std::string line;
    layers.clear();
    
    while (std::getline(infile, line)) {
        line = trim(line);
        if (line.empty())
            continue;  // Skip blank lines

        std::stringstream ss(line);
        std::string nameToken, dimToken, dataToken;
        
        // Expect three tab-separated tokens.
        if (!std::getline(ss, nameToken, '\t'))
            continue;
        if (!std::getline(ss, dimToken, '\t'))
            continue;
        if (!std::getline(ss, dataToken, '\t'))
            continue;
        
        Layer layer;
        layer.name = trim(nameToken);
        
        // Parse the dimension token.
        std::vector<float> dimsFloat = parseBracketedList(dimToken);
        if (dimsFloat.empty()) {
            std::cerr << "Error: No dimension info for layer " << layer.name << std::endl;
            continue;
        }
        // Convert float dimensions to int without any modification.
        for (float f : dimsFloat) {
            layer.dims.push_back(static_cast<int>(f));
        }
        
        // Compute total size from dims.
        int totalSize = product(layer.dims);
        
        // Preallocate the flat data vector.
        layer.data.resize(totalSize, 0.0f);
        
        layers.push_back(layer);
    }
    
    infile.close();
    return true;
}

// w_fill: Reads the file again and fills each layer's data vector
// using the third token (data token) from each non-blank line.
// It reshapes the parsed flat list of values into the layer's flat vector (which already has the expected size).
bool SystolicArray::w_fill(const std::string& filename) {
    std::ifstream infile(filename);
    if (!infile.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return false;
    }
    
    std::string line;
    size_t layerIndex = 0;
    
    while (std::getline(infile, line)) {
        line = trim(line);
        if (line.empty())
            continue;
        
        std::stringstream ss(line);
        std::string nameToken, dimToken, dataToken;
        if (!std::getline(ss, nameToken, '\t'))
            continue;
        if (!std::getline(ss, dimToken, '\t'))
            continue;
        if (!std::getline(ss, dataToken, '\t'))
            continue;
        
        if (layerIndex >= layers.size()) {
            std::cerr << "Error: More layers in file than expected." << std::endl;
            break;
        }
        
        // Parse the data token into a flat vector of floats.
        std::vector<float> dataValues = parseBracketedList(dataToken);
        int expectedSize = product(layers[layerIndex].dims);
        if (dataValues.size() != static_cast<size_t>(expectedSize)) {
            std::cerr << "Warning: Data size mismatch for layer " 
                      << layers[layerIndex].name
                      << ". Expected " << expectedSize 
                      << " but got " << dataValues.size() << std::endl;
        }
        
        // Fill as many elements as possible.
        size_t count = std::min(dataValues.size(), static_cast<size_t>(expectedSize));
        for (size_t i = 0; i < count; i++) {
            layers[layerIndex].data[i] = dataValues[i];
        }
        layerIndex++;
    }
    
    infile.close();
    return true;
}

// For debugging: prints out each layer's name, dimensions, and data.
// For 1D or 2D arrays, it prints them in a human-friendly format; otherwise, it prints the dims and flat data.
void SystolicArray::print() const {
    for (const auto &layer : layers) {
        std::cout << "Layer: " << layer.name << "\nDimensions: [";
        for (size_t i = 0; i < layer.dims.size(); i++) {
            std::cout << layer.dims[i] << (i < layer.dims.size() - 1 ? ", " : "");
        }
        std::cout << "]\nData:" << std::endl;
        
        if (layer.dims.size() == 1) {
            // Print as a 1D array.
            for (int i = 0; i < layer.dims[0]; i++) {
                std::cout << layer.data[i] << " ";
            }
            std::cout << std::endl;
        } else if (layer.dims.size() == 2) {
            // Print as a 2D array (row-major).
            int rows = layer.dims[0];
            int cols = layer.dims[1];
            for (int r = 0; r < rows; r++) {
                for (int c = 0; c < cols; c++) {
                    std::cout << layer.data[r * cols + c] << " ";
                }
                std::cout << std::endl;
            }
        } else {
            // For higher dimensions, just print flat.
            for (float val : layer.data) {
                std::cout << val << " ";
            }
            std::cout << std::endl;
        }
        
        std::cout << "\n------------------------\n";
    }
}

const std::vector<Layer>& SystolicArray::getLayers() const {
    return layers;
}
