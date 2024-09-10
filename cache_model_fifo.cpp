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


//////////////////////////////

class load_store_unit{
public:
    load_store_unit(size_t ls_queue_size): ls_queue_size(ls_queue_size){}

    // initialize load requests
    void init_(string input_fname){
        ifstream input_file(input_fname);

        string line;
        while(getline(input_file, line)){
            istringstream iss(line);
            int first;
            cout << first << endl;
            if (iss >> first){
                request_queue.push(first);
            }
        }
        input_file.close();
    }

    // cycle behavior
    int cycle(){
        cout << request_queue.front() << endl;
        if (request_queue.empty()){
            return -1;     
        }

        if (load_queue.size() >= ls_queue_size){
            cout << "Load queue full!" << endl;
        }
        else{
            load_queue.push(request_queue.front());
            request_queue.pop();
        }

        if (!read_ack_channel.channel_empty()){
            read_ack_channel.channel_pop();
        }
        
        if (read_request_channel.channel_full()){
            cout << "can't push to channel" << endl;
            return 1;
        }

        read_request req1 = {load_queue.front(), 0};
        read_request_channel.channel_push(req1);
        cout << load_queue.front() << endl;
        
        // cout << "Buffer is not full..." << endl;
        load_queue.pop();
        cout << "Load popped" << endl;

        return 0;
    }

private:
    size_t ls_queue_size;
    queue<int> load_queue, request_queue;
    queue<pair<int, int>> store_queue, data_queue;

};


//////////////////////////////
// template<typename T>
class cache{
public:
    const int miss_penalty = 4;
    
    // size parameters are in words
    cache(size_t block_size, size_t cache_size, int n_ways) : block_size(block_size), cache_size(cache_size), n_ways(n_ways), stall_rem(stall_rem){}

    int block_offset_bits, index_bits, tag_bits, set_index_mask;

    void init_(string input_fname){

        int n_sets = cache_size/(n_ways*block_size);

        block_offset_bits = get_bits(block_size*4);
        index_bits = get_bits(n_sets);
        tag_bits = 32 - (block_offset_bits + index_bits);

        set_index_mask = (1 << index_bits) - 1;
        for (int i = 0; i < n_sets; i++){
            free_lines_map[i] = n_ways;
            set_fifo_map[i] = queue<int>();
        }

        stall_rem = 0;

        ifstream input_file(input_fname);

        string line;
        while(getline(input_file, line)){
            istringstream iss(line);
            int first, second;
            if (iss >> first >> second){
                storage_map[first] = second;
            }
        }

        input_file.close();

        // cout << stall_rem << endl;
        
        // cout << storage_map.size() << endl;
        // cout << data_queue.size() << endl;
    }


    int get_bits(int val){
        return log2(val);
    }

    bool is_full(){
        return cache_map.size() >= cache_size;
    }

    void cycle(){
        if (read_request_channel.channel_empty()) {
            cout << "Cache starting up..." << endl;
            return;
        }

        if (stall_rem > 0){
            stall_rem--;
            cout << "Memory stalled for " << stall_rem << " more cycles..." << endl;
            return;
        }

        read_request req1 = read_request_channel.channel_pop();

        if (cache_map.find(req1.address) != cache_map.end()){ // hit
            cout << "HIT! @ " << req1.address << " Val: "<< cache_map[req1.address] << endl;
            read_ack ack1 = {true, req1.address, req1.id};
            read_ack_channel.channel_push(ack1);
            perf_vector[0]++;
            perf_vector[2]++;
        }
        else{ // miss
            int set_index = (req1.address >> block_offset_bits) & set_index_mask;
            cout << "MISS @ " << req1.address << " Set Index: " << set_index << endl;
            stall_rem += miss_penalty;

            // line eviction
            if (free_lines_map[set_index] == 0){
                int words_evicted = 0;
                int addr = set_fifo_map[set_index].front();
                while (words_evicted < block_size){
                    cache_map.erase(addr);
                    addr += 4;
                    words_evicted++;
                }
                set_fifo_map[set_index].pop();
                free_lines_map[set_index]++;
            }

            if (free_lines_map[set_index] > 0){
                int words_fetched = 0;
                int addr = req1.address;
                while (words_fetched < block_size){
                    cache_map[addr] = storage_map[addr];
                    addr += 4;
                    words_fetched++;
                }
                free_lines_map[set_index]--;
                set_fifo_map[set_index].push(addr);
            }
            

            read_ack ack1 = {false, req1.address, req1.id};
            read_ack_channel.channel_push(ack1);

            perf_vector[0]++;
            perf_vector[1]++;
        }

        // cout << stall_rem << endl;
    }

private:
    size_t block_size, cache_size;
    int n_ways;
    int stall_rem = 0;
    queue<pair<int, int>> data_queue;
    unordered_map<int, int> storage_map, cache_map, free_lines_map;
    unordered_map<int, queue<int>> set_fifo_map;
    // int block_address_mask, block_offset_mask;
};


int main(){
    // channel<int> read_cache(5);
    cout << "Starting simulation..." << endl;
    
    cache l1(16, 4096, 4);
    l1.init_("tests/storage-1716782613-1");
    cout << "Storage initialized... " << endl;

    load_store_unit lsu1(8);
    lsu1.init_("tests/requests-1716782613-1");
    cout << "Requests initialized... " << endl;

    cout << "Starting cycles...\n" << endl;

    int n_cycles = 8192;
    int count = 1;

    while (n_cycles >= 0){
       cout << "Cycle#: " << count << endl;
       l1.cycle();
       lsu1.cycle();
       n_cycles--;
       count++;
    //    cout << endl;
    }

    cout << "\n\n\n" << "Summary\n#########" << endl;
    cout << "Memory accesses: " << perf_vector[0] << endl;
    cout << "Misses: " << perf_vector[1] << endl;
    cout << "Hits: " << perf_vector[2] << endl;
    cout << "Hit rate: " << (float)perf_vector[2]/(float)perf_vector[0] << endl;
    cout << "CPI: " << (float)count/(float)perf_vector[0] << endl;
    // cout << read_request_channel.channel_empty() << endl;
    // lsu1.cycle();
    // cout << read_request_channel.channel_empty() << endl;
    // lsu1.cycle();
    // cout << read_request_channel.channel_size() << endl;
    // lsu1.cycle();
    // cout << read_request_channel.channel_size() << endl;
    // lsu1.cycle();
    // cout << read_request_channel.channel_size() << endl;
    // lsu1.cycle();
    // cout << read_request_channel.channel_size() << endl;
    // lsu1.cycle();
    // cout << read_request_channel.channel_size() << endl;
    return 0;
}