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

}

void cache_lru::move_to_back_lru(int addr) {

}
