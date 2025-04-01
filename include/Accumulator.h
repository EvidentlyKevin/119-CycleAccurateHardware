#ifndef ACCUMULATOR_H
#define ACCUMULATOR_H

#include "ArrayWrite.h"
#include <vector>
#include <string>
#include <unordered_map>

class Accumulator {
public:
    void load(const SystolicArray& arrayWriter);
    void reset();
    
    // Retrieve processed matrices (one per layer)
    std::unordered_map<std::string, std::vector<float>> getProcessedMatrices() const;

private:
    std::unordered_map<std::string, std::vector<float>> processedMatrices; // Stores results per layer
};

#endif // ACCUMULATOR_H
