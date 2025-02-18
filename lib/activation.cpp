#include "../include/activation.h"
#include <iostream>
#include <cmath>


//activation functions
// Rectified Linear Unit (ReLU): f(x) = max(0, x)
double Activation::relu(double x) {
    if (x < 0) {
        return 0;
    }
    return x;
}
// Sigmoid: f(x) = 1 / (1 + e^(-x))
double Activation::sigmoid(double x) {
    return 1 / (1 + exp(-x));
}
// Hyperbolic Tangent: f(x) = (e^x - e^(-x)) / (e^x + e^(-x))
double Activation::tanh(double x) {
    return std::tanh(x);
}

double Activation::gelu(double x) {
    return 0.5f * x * (1.0f + std::tanh(std::sqrt(2.0f / 3.14159265358f) * (x + 0.044715f * std::pow(x, 3.0f))));
}

void Activation::reset() {
    std::cout << "Resetting activation function" << std::endl;
}

