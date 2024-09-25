#ifndef MAC_UNIT_H
#define MAC_UNIT_H

//#include "../include/channel.h"

template<typename T>
class channel;

template<typename T>
class Systolic_Array;

// MAC (Multiply-Accumulate) Unit class
template<typename T>
class MACUnit { 
public:

    MACUnit(int i, int j) : accumulator(0), rowID(i), colID(j), rightOut(128), downOut(128) {}
    int w = 1;
    channel<T> rightOut;
    channel<T> downOut;
    // Performs the MAC operation

    void cycle(Systolic_Array<T>& systolic_array) {
        if(rowID > 0 && colID > 0){
            T a = systolic_array[rowID-1][colID]->downOut.channel_pop();
            T b = systolic_array[rowID][colID-1]->rightOut.channel_pop();
            accumulator = (a*w) + b;
            rightOut.channel_push(accumulator);
            downOut.channel_push(a);
        }
        else if(rowID == 0 && colID == 0){
            T a = 1;
            T b = 1;
            accumulator = (a*w) + b;
            rightOut.channel_push(accumulator);
            downOut.channel_push(a);
            
        }
        else if(rowID == 0 && colID > 0){
            T a = 1;
            T b = systolic_array[0][colID-1]->rightOut.channel_pop();
            accumulator = (a*w) + b;
            rightOut.channel_push(accumulator);
            downOut.channel_push(a);
        }
        else if(colID == 0 && rowID > 0){   
            T a = systolic_array[rowID-1][0]->downOut.channel_pop();
            T b = 1;
            accumulator = (a*w) + b;
            rightOut.channel_push(accumulator);
            downOut.channel_push(a);
        }
    }

    T read_accumulator() const {
        return accumulator;
    }

    void clear_accumulator() {
        accumulator = 0;
    }

private:
    T accumulator; // Stores the accumulated result
    T rowID = 0;
    T colID = 0;
    
};

#endif
