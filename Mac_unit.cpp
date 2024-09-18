#include <iostream>
#include <deque>
#include <vector>

using namespace std;

// Generic channel class to simulate a buffer with a fixed capacity.
template<typename T>
class channel {
public:
    // Constructor that initializes the buffer with a given capacity
    channel(size_t capacity) : capacity(capacity), is_full(false) {}

    // Method to push an element into the channel if it's not full
    void channel_push(T& element) {
        if (buffer.size() >= capacity) {
            cout << "Buffer full..." << endl;
            return;
        }
        buffer.push_back(element);  // Push element to the buffer

        // Update the full status of the buffer
        is_full = buffer.size() == capacity;
    }

    // Method to pop and return the front element from the channel
    T channel_pop() {
        if (buffer.empty()) {
            cout << "Buffer empty..." << endl;
            return T();  // Return a default-constructed object if the buffer is empty
        }

        T ret_val = buffer.front();  // Get the front element
        buffer.pop_front();  // Remove the front element
        is_full = false;  // Buffer can't be full after a pop

        return ret_val;
    }

    // Check if the buffer is full
    bool channel_full() {
        return is_full;
    }

    // Check if the buffer is empty
    bool channel_empty() {
        return buffer.empty();
    }

    // Return the current size of the buffer
    size_t channel_size() {
        return buffer.size();
    }

private:
    deque<T> buffer;  // Buffer to hold the elements
    size_t capacity;  // Maximum capacity of the buffer
    bool is_full;  // Boolean flag indicating whether the buffer is full
};

// Structures for read request and acknowledgment (Unused in this specific code)
struct read_request {
    int32_t address;
    int32_t id;
};

struct read_ack {
    bool read_success;
    int32_t address;
    int32_t id;
};

// Channels to hold numbers for the MAC unit operations
channel<int32_t> number_channelA(100);  // Holds numbers for MAC unit A
channel<int32_t> number_channelB(100);  // Holds numbers for MAC unit B

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

// MAC (Multiply-Accumulate) Unit class
template<typename T>
class MACUnit {
public:
    MACUnit() : accumulator(0) {}

    // Performs a multiply-and-accumulate operation
    void cycle(T a, T b) {
        accumulator += a * b;
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
    T accumulator;  // Stores the accumulated result
};

// Function to simulate the MAC operation cycle between number_channelA and number_channelB
void Mac_Cycle() {
    MACUnit<double> mac_unit;  // Instantiate a MAC unit

    // Process both channels as long as they're not empty
    while (!number_channelA.channel_empty() && !number_channelB.channel_empty()) {
        int32_t numA = number_channelA.channel_pop();  // Pop from channel A
        int32_t numB = number_channelB.channel_pop();  // Pop from channel B

        mac_unit.cycle(numA, numB);  // Perform MAC operation

        // Display the result of the current MAC operation and accumulated value
        cout << "--------------------------------" << endl;
        cout << "MAC Operation: " << numA << " * " << numB << " = " << numA * numB << endl;
        cout << "Accumulated value: " << mac_unit.read_accumulator() << endl;
        cout << "--------------------------------" << endl;
    }
}

int main() {
    // Push numbers into both channels
    push_numberA(30);
    cout << "Succesfully pushed numbers into channel A" << endl;
    cout << "--------------------------------" << endl;
    push_numberB(30);
    cout << "Succesfully Pushed numbers into channel B" << endl;
    cout << "" << endl;
    cout << "now performing MAC operation" << endl;

    // Perform MAC operations
    Mac_Cycle();
    cout << "Mac Operation Completed YAY!!" << endl;
    return 0;
}
