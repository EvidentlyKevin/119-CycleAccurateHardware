#ifndef ARRAYWRITE_H
#define ARRAYWRITE_H

#include <vector>
#include <string>

struct Layer {
    std::string name;         // Name of the layer.
    std::vector<int> dims;    // Dimensions of the layer (e.g., for a 2D array: {rows, cols}; for 1D: {length})
    std::vector<float> data;  // Flat vector holding the layer’s data.
};

class SystolicArray {
public:
    SystolicArray() = default;
    ~SystolicArray() = default;

    // Reads the file and creates layers with their dimensions preserved.
    bool w_fetch(const std::string& filename = "../weights/testDump.text");

    // Reads the file and fills each layer's data vector.
    bool w_fill(const std::string& filename = "../weights/testDump.text");

    // (Optional) Print the array contents for debugging.
    void print() const;

    // Provides access to the layers.
    const std::vector<Layer>& getLayers() const;

private:
    std::vector<Layer> layers;

    // Helper function to parse a bracketed list into a vector of floats.
    std::vector<float> parseBracketedList(const std::string& str);
};

#endif // ARRAYWRITE_H
