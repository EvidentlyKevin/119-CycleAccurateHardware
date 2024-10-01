#ifndef MAC_UNIT_H
#define MAC_UNIT_H

//#include "../include/channel.h"
#include "systolic_array.h"
#include <chrono>  // For time duration
#include <Windows.h>


template<typename T>
class channel;

template<typename T>
class Systolic_Array;

// MAC (Multiply-Accumulate) Unit class
template<typename T>
class MACUnit { 
public:

    MACUnit(int i, int j) : accumulator(0), rowID(i), colID(j), rightOut(128), downOut(128), inputA(10) {}
    
    int clk = 0;
    int clk_Period = 1;
    channel<T> inputA;
    channel<T> rightOut;
    channel<T> downOut;
    T a;
    T b;
    T w;
    // Performs the MAC operation

    void cycle(Systolic_Array<T>& systolic_array) {
    
if(clk > 2){
    clk = 0;    
}
             if(clk == 0){
            if(rowID > 0 && colID > 0){
        if(!systolic_array[rowID][colID-1]->rightOut.channel_empty() && !systolic_array[rowID-1][colID]->downOut.channel_empty()){


             a = systolic_array[rowID-1][colID]->downOut.channel_pop();
             b = systolic_array[rowID][colID-1]->rightOut.channel_pop();
                     clk++;

             //w = systolic_array.GetWeight(rowID, colID);
}
        }
        else if(rowID == 0 && colID == 0){
            if (!inputA.channel_empty()){
             a = inputA.channel_pop();
             b = 0;
                     clk++;
            }
             //w = systolic_array.GetWeight(rowID, colID);


        }
        else if(rowID == 0 && colID > 0){  
            if(!inputA .channel_empty()){
             a = inputA.channel_pop();
             b = systolic_array[rowID][colID-1]->rightOut.channel_pop();;
                     clk++;
            }
            // w = systolic_array.GetWeight(rowID, colID);
          

        }
        else if(colID == 0 && rowID > 0){   
            if(!systolic_array[rowID-1][0]->downOut.channel_empty()){
             a = systolic_array[rowID-1][0]->downOut.channel_pop();
             b = 0;
                     clk++;
            }
             //w = systolic_array.GetWeight(rowID, colID);
        }
        }
        
      else if(clk == 1){

            accumulator = (a*w) + b;
            clk++;
        }

       else if(clk == 2){
            rightOut.channel_push(accumulator);
            downOut.channel_push(a);
            clk++;  
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
     // Add the Weight field
    
};

#endif
