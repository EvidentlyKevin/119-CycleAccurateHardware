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

channel<read_request> read_request_channel(4), read_sink_channel(4);
channel<read_ack> read_ack_channel(4), sink_ack_channel(4);

// perf_vector
// 0: number of instructions/memory ops/requests processed
// 1: number of misses
// 2: number of hits
vector<int> perf_vector(3, 0); 

// Function to push requests into the read request channel
void push_requests() {
    for (int i = 0; i < 5; ++i) {
        read_request req = {i * 10, i};  // Sample data
        if (!read_request_channel.channel_full()) {
            read_request_channel.channel_push(req);
            cout << "Pushed read_request: {address: " << req.address 
                 << ", id: " << req.id << "}" << endl;
            perf_vector[0]++;  // Increment processed requests count
        } else {
            cout << "Request channel is full!" << endl;
        }
    }
}

// Function to pop requests and process them
void process_requests() {
    while (!read_request_channel.channel_empty()) {
        read_request req = read_request_channel.channel_pop();
        cout << "Processing read_request: {address: " << req.address 
             << ", id: " << req.id << "}" << endl;

        // Simulate processing and generating read_ack
        read_ack ack = {true, req.address, req.id};
        read_ack_channel.channel_push(ack);

        // For demonstration, consider every alternate request a "miss"
        if (req.id % 2 == 0) {
            perf_vector[1]++;  // Count as a "miss"
        } else {
            perf_vector[2]++;  // Count as a "hit"
        }
    }
}

// Function to pop acks
void process_acks() {
    while (!read_ack_channel.channel_empty()) {
        read_ack ack = read_ack_channel.channel_pop();
        cout << "Ack received: {success: " << ack.read_success 
             << ", address: " << ack.address << ", id: " << ack.id << "}" << endl;
    }
}



template<typename T>
class MACUnit {
public:
    
   MACUnit() : accumulator(0){}

   void cycle(T a, T b){
    accumulator += a*b;
   }

T read_accumulator() const{
    return accumulator;
}

void clear_accumulator(){
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

    // Display performance vector results
    cout << "Processed Requests: " << perf_vector[0] << endl;
    cout << "Misses: " << perf_vector[1] << endl;
    cout << "Hits: " << perf_vector[2] << endl;

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
