#ifndef MAC_UNIT_H
#define MAC_UNIT_H

#include "../include/channel.h"

// Forward declaration of Systolic_Array to avoid circular dependency
template<typename T>
class Systolic_Array;

// MAC (Multiply-Accumulate) Unit class
template<typename T>
class MACUnit { 
public:
    MACUnit(int i, int j) : accumulator(0), rowID(i), colID(j), rightOut(128), downOut(128) {}

    // Performs the MAC operation
    void cycle(Systolic_Array<T>& systolic_array) {
        if(rowID > 0 && colID > 0){
            T a = systolic_array[rowID][colID-1].downOut.channel_pop();
            T b = systolic_array[rowID-1][colID].rightOut.channel_pop();
            accumulator += a * b;
            rightOut.channel_push(accumulator);
            downOut.channel_push(accumulator);
        }
        else if(rowID == 0 && colID == 0){
            T a = 1;
            T b = 1;
            accumulator += a * b;
            rightOut.channel_push(accumulator);
            downOut.channel_push(accumulator);
        }
        else if(rowID == 0){
            T a = 1;
            T b = systolic_array[0][colID].rightOut.channel_pop();
            accumulator += a * b;
            rightOut.channel_push(accumulator);
            downOut.channel_push(accumulator);
        }
        else if(colID == 0){
            T a = systolic_array[rowID-1][colID].downOut.channel_pop();
            T b = 1;
            accumulator += a * b;
            rightOut.channel_push(accumulator);
            downOut.channel_push(accumulator);
        }
    }

    // Returns the accumulated result
    T read_accumulator() const {
        return accumulator;
    }

    // Clears the accumulated value
    void clear_accumulator() {
        accumulator = 0;
    }

private:
    T accumulator; // Stores the accumulated result
    int rowID = 0;
    int colID = 0;
    channel<T> rightOut;
    channel<T> downOut;
};

#endif
