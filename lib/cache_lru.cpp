#include "cache_lru.h"
#include "request_structs.h"
#include "channel.h"
#include <iostream>
#include <cmath>
#include <fstream>
#include <sstream>

cache_lru::cache_lru(size_t block_size, size_t cache_size, int n_ways)
    : stall_rem(0) {
    this->block_size = block_size;
    this->cache_size = cache_size;
    this->n_ways = n_ways;
}

void cache_lru::init_(const std::string& input_fname) {
    int n_sets = cache_size/(n_ways*block_size); // calculate number of sets

    block_offset_bits = get_bits(block_size*4); // block offset bits
    index_bits = get_bits(n_sets); // index bits
    tag_bits = 32 - (block_offset_bits + index_bits); // tag bits

    set_index_mask = (1 << index_bits) - 1; // set index mask

    // track number of free lines in each set
    // needed for line eviction and replacement in SA caches
    for (int i = 0; i < n_sets; i++){
        free_lines_map[i] = n_ways; // n-way set associativity
        set_fifo_map[i] = queue<int>(); // initialize FIFO map for each set
    }

    stall_rem = 0;

    // open the storage input file and populate the storage map with key-value pairs
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
}

void cache_lru::cycle() {

    if (read_request_channel.channel_empty()) { // check if there are read requests
        cout << "Cache starting up..." << endl;
        return;
    }

    if (stall_rem > 0){ // check memory stalls
        stall_rem--;
        // cout << "Memory stalled for " << stall_rem << " more cycles..." << endl;
        return;
    }

    read_request req1 = read_request_channel.channel_pop(); // get read request from bus

    if (cache_map.find(req1.address) != cache_map.end()){ // hit
        // cout << "HIT! @ " << req1.address << " Val: "<< cache_map[req1.address] << endl;
        read_ack ack1 = {true, req1.address, cache_map[req1.address]}; // send read acknowledgement (future support)
        read_ack_channel.channel_push(ack1);
        perf_vector[0]++; // increment total accesses
        perf_vector[2]++; // increment hit count
        move_to_back_lru(req1.address); // update LRU
    }
    else{ // miss
        int set_index = get_set_index(req1.address);
        // cout << "MISS @ " << req1.address << " Set Index: " << set_index << endl;
        stall_rem += miss_penalty; // add miss penalty to stall count
        perf_vector[3] += miss_penalty; // update number of cycles stalled due to memory

        // comment this line out if you want only hits to count as "use" of a cache line
        move_to_back_lru(req1.address); // update LRU

        // line eviction from set
        if (free_lines_map[set_index] == 0){
            int words_evicted = 0;
            int addr = set_fifo_map[set_index].front(); // least recently used line from set
            while (words_evicted < block_size){ // all words in the cache line
                cache_map.erase(addr); // evict the address from the cache
                addr += 4;
                words_evicted++;
            }
            set_fifo_map[set_index].pop(); // remove the LRU line from queue
            free_lines_map[set_index]++; // one more line is now available in the set
        }

        // fetch line from lower levels of memory hierarchy
        // and place in cache set if free lines are available in set
        if (free_lines_map[set_index] > 0){
            int words_fetched = 0;
            int addr = req1.address;
            while (words_fetched < block_size){
                cache_map[addr] = storage_map[addr]; // fetch data from storage
                addr += 4;
                words_fetched++;
            }
            free_lines_map[set_index]--; // one less line is now available in the set
            set_fifo_map[set_index].push(addr); // most recently used line (new)
        }
        

        read_ack ack1 = {false, req1.address, req1.id};
        read_ack_channel.channel_push(ack1);

        perf_vector[0]++; // increment total accesses
        perf_vector[1]++; // increment miss count
    }
}

void cache_lru::move_to_back_lru(int addr) {
    int set_index = get_set_index(addr); // get the set index of the address
    int addr_tag = get_tag(addr); // get the tag of the address
    // cout << "LRU" << endl;
    // cout << addr_tag << endl;

    queue<int> tmp_queue; // (future support) optimize using deque
    // process the FIFO queue for the set for maintaing LRU order
    while (!set_fifo_map[set_index].empty()){
        int current_addr = set_fifo_map[set_index].front();
        int current_addr_tag = get_tag(current_addr);
        set_fifo_map[set_index].pop();

        // all addresses except the address being accessed (hit or miss)
        if (current_addr_tag != addr_tag){
            tmp_queue.push(current_addr);
        }
    }

    // the address being accessed is the MOST recently used
    // hence it is at the end of the queue
    // at the front of the queue si the address LEASE recently used
    tmp_queue.push(addr);
    set_fifo_map[set_index] = tmp_queue;
    // cout << set_fifo_map[set_index].size() << endl;
}
