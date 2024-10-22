#ifndef MAC_UNIT_H
#define MAC_UNIT_H

#include "channelM.h"

template<typename T>
class Systolic_Array; // Forward declaration

template<typename T>
class MACUnit {
public:
    // Constructor
    MACUnit(int i, int j)
        : rowID(i), colID(j), accumulator(0), a(0), b(0), w(0), clk(0),
          rightOut(CHANNEL_CAPACITY), downOut(CHANNEL_CAPACITY), inputA(CHANNEL_CAPACITY_INPUT) {}

    // *** Set the weight for the MAC unit ***
    void setWeight(T weight) {
        w = weight;
    }

    // Set the input activation (only for the first row)
    void setInputActivation(T activation) {
        if (rowID == 0) {
            inputA.push(activation);
        }
    }

    // Perform one cycle of computation
    void cycle(Systolic_Array<T>& systolic_array);

    // Read the accumulator value
    T readAccumulator() const {
        return accumulator;
    }

    // Clear the accumulator
    void clearAccumulator() {
        accumulator = 0;
    }

    // Constants
    static const int MAX_CLK_STATE = 2;
    static const size_t CHANNEL_CAPACITY = 128;
    static const size_t CHANNEL_CAPACITY_INPUT = 10;

private:
    // Member variables in the order they are initialized
    int rowID;
    int colID;
    T accumulator;
    T a;
    T b;
    T w;
    int clk;

public:
    // Channels are public to allow access from Systolic_Array
    channelM<T> rightOut;
    channelM<T> downOut;
    channelM<T> inputA; // Only used for the first row

private:
    // Helper functions
    bool fetchInputs(Systolic_Array<T>& systolic_array);
    void computeMAC();
    void sendOutputs();
};

#include "mac_unit.tpp"

#endif // MAC_UNIT_H
