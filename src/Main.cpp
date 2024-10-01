
#include <iostream>
#include <deque>
#include <vector>
#include <array>
#include "../include/channel.h"
#include "../include/systolic_array.h"
#include "../include/mac_unit.h"

using namespace std;








int main() {
Systolic_Array<double> systolic_array;
int loop = 0;
systolic_array.Construct("C:\\Users\\Henry\\C++Code\\RandomNumOutput\\0801008n0");
systolic_array.ConstructA("C:\\Users\\Henry\\C++Code\\RandomNumOutput(inputA)\\0801008n0");

int max_cycle = 256, cycle_count = 0, array_size = 8;



for (int i = 0; i < array_size; i++){
    systolic_array[0][i]->inputA.channel_push(systolic_array.A[0][i]);
}



while (cycle_count < max_cycle){
    for (int i = array_size-1; i > -1; i--) { 
        for (int j = array_size-1; j > -1; j--) {
            cout << "Clock: " << systolic_array[i][j]->clk << endl;
            cout << "" << endl;
            cout << "Performing MAC operation at position: " << i << ", " << j << endl;
            cout << "operand B: " << systolic_array[i][j]->b << endl;
            cout << "operand A: " << systolic_array[i][j]->a << endl;
            cout << "Weight: " << systolic_array[i][j]->w << endl;      
                        cout << "" << endl;
            systolic_array[i][j]->cycle(systolic_array);
                        cout << "Accumulated value: " << systolic_array[i][j]->read_accumulator() << endl;
                                    cout << "" << endl;


        }
    }
cycle_count++;

}
    return 0;

}
