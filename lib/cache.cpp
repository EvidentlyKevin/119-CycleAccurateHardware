#include "cache.h"
#include <iostream>
#include <cmath>
#include <fstream>
#include <sstream>

Cache::Cache(size_t block_size, size_t cache_size, int n_ways)
    : block_size(block_size), cache_size(cache_size), n_ways(n_ways) {}

void Cache::init_(const std::string& input_fname) {
    int n_sets = cache_size / (n_ways * block_size);
    block_offset_bits = get_bits(block_size * 4);
    index_bits = get_bits(n_sets);
    tag_bits = 32 - (block_offset_bits + index_bits);
    set_index_mask = (1 << index_bits) - 1;

    for (int i = 0; i < n_sets; i++) {
        free_lines_map[i] = n_ways;
        set_fifo_map[i] = std::queue<int>();
    }

    stall_rem = 0;

    std::ifstream input_file(input_fname);
    std::string line;
    while (std::getline(input_file, line)) {
        std::istringstream iss(line);
        int first, second;
        if (iss >> first >> second) {
            storage_map[first] = second;
        }
    }
    input_file.close();
}

int Cache::get_bits(int val) {
    return static_cast<int>(log2(val));
}

void Cache::cycle() {
    if (read_request_channel.channel_empty()) {
        std::cout << "Cache starting up..." << std::endl;
        return;
    }

    if (stall_rem > 0) {
        stall_rem--;
        std::cout << "Memory stalled for " << stall_rem << " more cycles..." << std::endl;
        return;
    }

    read_request req1 = read_request_channel.channel_pop();
    if (cache_map.find(req1.address) != cache_map.end()) { // Hit
        std::cout << "HIT! @ " << req1.address << std::endl;
        read_ack ack1 = {true, req1.address, req1.id};
        read_ack_channel.channel_push(ack1);
    } else { // Miss
        std::cout << "MISS @ " << req1.address << std::endl;
        stall_rem += 4; // Miss penalty
    }
}
