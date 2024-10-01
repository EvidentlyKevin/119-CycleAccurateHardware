// File: include/mac_unit.h

#ifndef MAC_UNIT_H
#define MAC_UNIT_H

#include "channel.h"          // Include Channel class
#include "systolic_array.h"   // Include Systolic_Array class

// Forward declarations
template<typename T>
class Systolic_Array;

// MAC (Multiply-Accumulate) Unit class
template<typename T>
class MACUnit { 
public:
    // Constructor
    MACUnit(int i, int j);

    // Perform the MAC operation
    void cycle(Systolic_Array<T>& systolic_array);

    // Accessor methods
    T read_accumulator() const;
    void clear_accumulator();

    // Public members
    int w; // Weight for multiplication
    channel<T> rightOut; // Output channel to the right
    channel<T> downOut;  // Output channel downward

private:
    T accumulator; // Stores the accumulated result
    int rowID;     // Row identifier in the array
    int colID;     // Column identifier in the array
};

#endif // MAC_UNIT_H
