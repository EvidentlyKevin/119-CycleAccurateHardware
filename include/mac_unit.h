#ifndef MAC_UNIT_H
#define MAC_UNIT_H

#include "../include/channel.h"
#include "../include/systolic_array.h"


// MAC (Multiply-Accumulate) Unit class
template<typename T>
class MACUnit {
public:
    MACUnit(int i, int j) : accumulator(0), rowID(i), colID(j), rightOut(4), downOut(4) {}

    channel<T> rightOut;
    channel<T> downOut;

    // Performs a multiply-and-accumulate operation
    // void cycle(T a, T b) {
    //     accumulator += a * b;
    // }
void kickstart(){
    if(rowID == 0 && colID == 0){
        T a = Systolic_Array[0][0].channel_pop();
        T b = Systolic_Array[0][0].channel_pop();}
    else if(rowID == 0){

}
}
    void cycle() {
        T a = Systolic_Array[rowID][colID-1].channel_pop();
        T b = Systolic_Array[rowID-1][colID].channel_pop();
        accumulator += a * b;
        rightOut.channel_push(accumulator);
        downOut.channel_push(accumulator);
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
};





#endif
