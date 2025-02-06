#include "../include/activation.h"
#include <iostream>
#include <cmath>



//activation functions
    // Rectified Linear Unit (ReLU): f(x) = max(0, x)
    float Activation::relu(float x) {
        if (x < 0) {
            return 0;
        }
        return x;
    }
    // Sigmoid: f(x) = 1 / (1 + e^(-x))
    float Activation::sigmoid(float x) {
        return 1 / (1 + exp(-x));
    }
    // Hyperbolic Tangent: f(x) = (e^x - e^(-x)) / (e^x + e^(-x))
    float Activation::tanh(float x) {
        return std::tanh(x);
    }

    float Activation::gelu(float x) {
        return 0.5f * x * (1.0f + std::tanh(std::sqrt(2.0f / 3.14159265358f) * (x + 0.044715f * std::pow(x, 3.0f))));
    }

    void reset() {
        std::cout << "Resetting activation function" << std::endl;
        
    }

