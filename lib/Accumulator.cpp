#include "../include/Accumulator.h"
#include <iostream>
#include <algorithm>
#include <numeric>
#include <cmath>  // For std::ceil() and std::floor()

void Accumulator::load(const SystolicArray& arrayWriter) {
    processedMatrices.clear(); // Reset before processing

    const std::vector<Layer>& layers = arrayWriter.getLayers();

    for (const auto& layer : layers) {
        // Skip bias layers
        if (layer.name.find(".bias") != std::string::npos) {
            continue;
        }

        std::vector<float> resultVector(128, 0.0f); // Output is always 128 values

        // Case: [X, 128] (Includes [128,128]) - Compute column-wise averages
        if (layer.dims.size() == 2 && layer.dims[1] == 128) {
            int rows = layer.dims[0];
            int cols = layer.dims[1];

            for (int i = 0; i < 128; i++) {
                float sum = 0.0f;
                for (int j = 0; j < rows; j++) {
                    sum += layer.data[j * cols + i]; // Sum along the column
                }
                resultVector[i] = sum / static_cast<float>(rows); // Column-wise average
            }
        } 
        // Case: [X] (1D vector) - Compute segment-wise averages
        else if (layer.dims.size() == 1) {
            int X = layer.dims[0];
            int n = static_cast<int>(std::ceil(static_cast<float>(X) / 128.0f)); // Round UP for default behavior

            // If rounding up would cause overflow, round down instead
            if (n * 128 > X) {
                n = static_cast<int>(std::floor(static_cast<float>(X) / 128.0f));
            }

            for (int i = 0; i < 128; i++) {
                int startIdx = i * n;
                int endIdx = std::min(startIdx + n, X); // Prevent out-of-bounds

                // If we rounded down and there are extra values at the end, drop them
                if (endIdx == X && i == 127 && X % 128 != 0) {
                    break;
                }

                float sum = std::accumulate(layer.data.begin() + startIdx, layer.data.begin() + endIdx, 0.0f);
                int count = endIdx - startIdx;
                resultVector[i] = (count > 0) ? sum / static_cast<float>(count) : 0.0f;
            }
        } 
        else {
            std::cerr << "Skipping layer " << layer.name << " due to unknown dimensions.\n";
            continue;
        }

        // Store result
        processedMatrices[layer.name] = resultVector;
    }
}

std::unordered_map<std::string, std::vector<float>> Accumulator::getProcessedMatrices() const {
    return processedMatrices;
}

void Accumulator::reset() {
    processedMatrices.clear();
}
