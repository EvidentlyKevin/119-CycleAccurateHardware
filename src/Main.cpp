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

    // Perform MAC operations
systolic_array.initialize();

    for (int i = 0; i < 8; ++i) {  
        for (int j = 0; j < 8; ++j) {
            cout << "Performing MAC operation at position: " << i << ", " << j << endl;
            systolic_array[i][j]->cycle(systolic_array);
           //cout << systolic_array[i][j]->rightOut.front() << endl;
    cout << "Accumulated value: " << systolic_array[i][j]->read_accumulator() << endl;
    cout << " " << endl;



        }
    }

    
    return 0;
}
