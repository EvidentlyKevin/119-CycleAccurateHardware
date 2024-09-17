#include <iostream>
#include <fstream>
#include <sstream>
#include <deque>
#include <queue>
#include <cmath>
#include <unordered_map>
#include <vector>

using namespace std;

template<typename T>
class channel{
public:
    channel(size_t capacity): capacity(capacity), is_full(is_full){}

    void channel_push(T& element){
        if (buffer.size() >= capacity){
            cout << "Buffer full..." << endl;
            return;
        }
        buffer.push_back(element);

        if (buffer.size() == capacity){
            is_full = true;
        }
        else{
            is_full = false;
        }
        return;
    }

    T channel_pop(){
        if (buffer.size() == 0){
            // cout << "Buffer empty..." << endl;
            return T();
        }
        T ret_val = buffer.front();
        buffer.pop_front();
        is_full = false;
        return ret_val;
    }

    bool channel_full(){
        return is_full;
    }

    bool channel_empty(){
        return buffer.empty();
    }

    size_t channel_size(){
        return buffer.size();
    }

private:
    deque<T> buffer;
    size_t capacity;
    bool is_full;
};

struct read_request{
    int32_t address;
    int32_t id;
};

struct read_ack{
    bool read_success;
    int32_t address;
    int32_t id;
};

channel<int32_t> number_channel(4);  // Holds numbers to be added
channel<int32_t> result_channel(4);  // Holds results of additions

// perf_vector
// 0: number of instructions/memory ops/requests processed
// 1: number of misses
// 2: number of hits
vector<int> perf_vector(3, 0); 

void push_numbers() {
    for (int i = 1; i <= 5; ++i) {
        if (!number_channel.channel_full()) {
            number_channel.channel_push(i);  // Pushes numbers 1, 2, 3, 4, 5 into the channel
            cout << "Pushed number: " << i << endl;
        } else {
            cout << "Number channel is full!" << endl;
        }
    }
}





template<typename T>
class MACUnit {
public:
    
   MACUnit() : accumulator(0) {}

   void cycle(T a, T b) {
    accumulator += a*b;
   }

T read_accumulator() const {
    return accumulator;
}

void clear_accumulator() {
    accumulator = 0;
}

private:
    T accumulator;  // Stores the accumulated result
};

int main() {
    MACUnit<double> mac_unit;


// Push some requests
    push_requests();
    
    // Process the requests and push acknowledgements
    process_requests();
    
    // Process the acknowledgements
    process_acks();

    

    mac_unit.cycle(2.5, 4.0);  // 2.5 * 4.0 = 10.0
    mac_unit.cycle(1.5, 2.0);  // 1.5 * 2.0 = 3.0
    mac_unit.cycle(2.5, 4.0);  // 2.5 * 4.0 = 10.0
    mac_unit.cycle(1.5, 2.0);  // 1.5 * 2.0 = 3.0
    mac_unit.cycle(2.5, 4.0);  // 2.5 * 4.0 = 10.0
    mac_unit.cycle(1.5, 2.0);  // 1.5 * 2.0 = 3.0
    mac_unit.cycle(2.5, 4.0);  // 2.5 * 4.0 = 10.0
    mac_unit.cycle(1.5, 2.0);  // 1.5 * 2.0 = 3.0
    mac_unit.cycle(2.5, 4.0);  // 2.5 * 4.0 = 10.0

    std::cout << "Accumulated value: " << mac_unit.read_accumulator() << std::endl;

    return 0;
}
