//activation class
#pragma once
class Activation 
{
public:
    double relu(double);
    double sigmoid(double);
    double tanh(double);
    double gelu(double);
private:
    void reset();
};
