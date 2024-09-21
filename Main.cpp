#include <iostream>
#include <deque>
#include <vector>
#include <array>
#include "channel.h"
#include "systolic_array.h"
#include "mac_unit.h"
using namespace std;


// Channels to hold numbers for the MAC unit operations
channel<int32_t> number_channelA(128);  // Holds numbers for MAC unit A
channel<int32_t> number_channelB(128);  // Holds numbers for MAC unit B


// Performance vector
// [0]: Number of instructions/memory operations/requests processed
// [1]: Number of misses
// [2]: Number of hits
vector<int> perf_vector(3, 0);

// Function to push numbers into number_channelA
void push_numberA(int x) {
    cout << "Pushing numbers into channel A from 1 to " << x << endl;
    for (int i = 1; i <= x; ++i) {
        if (!number_channelA.channel_full()) {
            number_channelA.channel_push(i);  // Push numbers into channel A
            //cout << "Pushed number A: " << i << endl;
        } else {
            cout << "Number channel A is full!" << endl;
        }
    }
}

// Function to push numbers into number_channelB
void push_numberB(int y) {
    cout << "Pushing numbers into channel B from 1 to " << y << endl;
    for (int i = 1; i <= y; ++i) {
        if (!number_channelB.channel_full()) {
            number_channelB.channel_push(i);  // Push numbers into channel B
            //cout << "Pushed number B: " << i << endl;
        } else {
            cout << "Number channel B is full!" << endl;
        }
    }
}



/*Function to simulate the MAC operation cycle between number_channelA and number_channelB
void Mac_Cycle() {
    MACUnit<double> mac_unit;  // Instantiate a MAC unit

    // Process both channels as long as they're not empty
    while (!number_channelA.channel_empty() && !number_channelB.channel_empty()) {
        // int32_t numA = number_channelA.channel_pop();  // Pop from channel A
        // int32_t numB = number_channelB.channel_pop();  // Pop from channel B
        cout << "multiplying " << number_channelA.front() << " with " << number_channelB.front() << endl;
        mac_unit.cycle();  // Perform MAC operation
        cout << mac_unit.read_accumulator() << endl;

        // Display the result of the current MAC operation and accumulated value
        /*cout << "--------------------------------" << endl;
        cout << "MAC Operation: " << numA << " * " << numB << " = " << numA * numB << endl;
        cout << "\033[1;34m"<< "Accumulated value: " << mac_unit.read_accumulator() << endl;
        cout << "\033[1;37m"<<"--------------------------------" << endl;
    }
}*/ 

int main() {
    MACUnit<double> mac_unit(0, 0);
    // Push numbers into both channels
    push_numberA(100);
    cout << "Succesfully pushed numbers into channel A" << endl;
    cout << "--------------------------------" << endl;
    push_numberB(100);
    cout << "Succesfully Pushed numbers into channel B" << endl;
    cout << "" << endl;
    cout << "now performing MAC operation" << endl;

   // Perform MAC operations
    //Mac_Cycle();
    cout << mac_unit.read_accumulator() << endl;
    cout << "Mac Operation Completed YAY!!" << endl;
    return 0;
}
