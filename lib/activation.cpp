//#include "../include/activation.h"
#include <iostream>
#include <cmath>


class Activation
{   

public:
//activation functions
    // Rectified Linear Unit (ReLU): f(x) = max(0, x)
    float relu(float x) {
        if (x < 0) {
            return 0;
        }
        return x;
    }
    // Sigmoid: f(x) = 1 / (1 + e^(-x))
    float sigmoid(float x) {
        return 1 / (1 + exp(-x));
    }
    // Hyperbolic Tangent: f(x) = (e^x - e^(-x)) / (e^x + e^(-x))
    float tanh(float x) {
        return std::tanh(x);
    }
private:
    void reset() {
        std::cout << "Resetting activation function" << std::endl;
        
    }
};
