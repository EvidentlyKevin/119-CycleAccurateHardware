// File: include/mac_unit.h

#ifndef MAC_UNIT_H
#define MAC_UNIT_H

#include "channelM.h"

template<typename T>
class Systolic_Array; // Forward declaration

template<typename T>
class MACUnit {
public:
    // Constructor
    MACUnit(int row, int col);

    void setUpIn(channelM<T>* upChannel);

    void setLeftIn(channelM<T>* leftChannel);

    channelM<T>& getRightOut();

    channelM<T>& getDownOut();

    // Set the weight for the MAC unit
    void setWeight(T weight);

    // Set the input activation (only for the first row)
    void setInputActivation(T activation);

    // Perform one cycle of computation
    //void cycle(Systolic_Array<T>& systolic_array);
    void cycle();

    // Read the accumulator value
    T readAccumulator() const;

    // Debugging method to get the last activation
    T getLastActivation() const;

private:
    // DO NOT MOVE INITIALIZATIONS PLEASE
    int rowID;
    int colID;
    T accumulator;
    T a; // Activation input
    T b; // Partial sum input
    T w; // Weight
    int clk;

    // Channels for data movement
    channelM<T> rightOut;
    channelM<T> downOut;
    channelM<T> inputA;
    // Input channels from neighboring MAC units
    channelM<T>* leftIn = nullptr; // Left input channel
    channelM<T>* upIn = nullptr;   // Up input channel

    // Helper functions
    // bool fetchInputs(Systolic_Array<T>& systolic_array, bool debug = false);
    bool fetchInputs(bool debug = false);
    void computeMAC(bool debug = false);
    void sendOutputs(bool debug = false);

    // Constants
    static const int MAX_CLK_STATE = 2;
    static const size_t CHANNEL_CAPACITY = 128;
    static const size_t CHANNEL_CAPACITY_INPUT = 10;
};

#include "mac_unit.tpp"

#endif // MAC_UNIT_H
