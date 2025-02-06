//activation class
#pragma once
class Activation 
{
public:
    float relu(float);
    float sigmoid(float);
    float tanh(float);
    float gelu(float);
private:
    void reset();
};
