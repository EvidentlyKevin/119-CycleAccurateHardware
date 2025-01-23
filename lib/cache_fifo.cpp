// File: lib/cache_fifo.cpp

#include "cache_fifo.h"
#include "globals.h"
#include <iostream>
#include <fstream>
#include <sstream>

cache_fifo::cache_fifo(int block_size, int cache_size, int n_ways, perf_counter* perf, int verbose)
    : Cache(block_size, cache_size, n_ways, perf), verbose_level(verbose) {}

void cache_fifo::init_(const std::string& input_fname) {
    int n_sets = cache_size / (n_ways * block_size * 4); // Assuming block_size in words (4 bytes per word)

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
    if (!input_file.is_open()) {
        std::cerr << "Error: Cannot open storage file " << input_fname << "\n";
        return;
    }
    std::string line;
    while (std::getline(input_file, line)) {
        std::istringstream iss(line);
        int address, data;
        if (iss >> address >> data) {
            storage_map[address] = data;
        }
    }
    input_file.close();

    if (verbose_level > 1) {
        std::cout << "Cache FIFO Initialized with " << n_sets << " sets.\n";
    }
}

void cache_fifo::cycle() {
    if (read_request_channel.channel_empty()) {
        if (verbose_level > 1) {
            std::cout << "Cycle: Cache is idle.\n";
        }
        return;
    }

    if (stall_rem > 0) {
        stall_rem--;
        perf->add_stall_cycles(1);
        if (verbose_level > 1) {
            std::cout << "Cycle: Memory stalled for " << stall_rem << " more cycles.\n";
        }
        return;
    }

    read_request req;
    if (!read_request_channel.channel_pop(req)) {
        if (verbose_level > 1) {
            std::cout << "Cycle: No read request to process.\n";
        }
        return; // No request to process
    }

    perf->increment_access();

    if (cache_map.find(req.address) != cache_map.end()) { // Hit
        read_ack ack = {req.address, cache_map[req.address]};
        if (!read_ack_channel.channel_push(ack)) {
            // Handle full channel if necessary, e.g., retry in next cycle
            std::cerr << "Read Ack Channel is full. Cannot push ack for address " << req.address << "\n";
        }
        perf->increment_hit();
        if (verbose_level > 0) {
            std::cout << "Cycle: HIT at address " << req.address << " with data " << cache_map[req.address] << "\n";
        }
    } else { // Miss
        int set_index = get_set_index(req.address);
        stall_rem += miss_penalty - 1; // We already decremented stall_rem once
        perf->add_stall_cycles(miss_penalty);
        perf->increment_miss();

        if (verbose_level > 0) {
            std::cout << "Cycle: MISS at address " << req.address << ". Loading block starting at address "
                      << (req.address / (block_size * 4)) * (block_size * 4) << "\n";
        }

        // Line eviction
        if (free_lines_map[set_index] == 0) {
            int evict_block_addr = set_fifo_map[set_index].front();
            set_fifo_map[set_index].pop();
            free_lines_map[set_index]++;

            for (int i = 0; i < block_size * 4; i += 4) {
                cache_map.erase(evict_block_addr + i);
            }

            if (verbose_level > 1) {
                std::cout << "Cycle: Evicted block starting at address " << evict_block_addr << "\n";
            }
        }

        // Load new block into cache
        if (free_lines_map[set_index] > 0) {
            int block_start_addr = (req.address / (block_size * 4)) * (block_size * 4);
            for (int i = 0; i < block_size * 4; i += 4) {
                int addr = block_start_addr + i;
                auto it = storage_map.find(addr);
                if (it != storage_map.end()) {
                    cache_map[addr] = it->second;
                } else {
                    cache_map[addr] = 0; // Assign default value
                    if (verbose_level > 0) {
                        std::cerr << "Warning: Address " << addr << " not found in storage. Assigned default value 0.\n";
                    }
                }
            }
            free_lines_map[set_index]--;
            set_fifo_map[set_index].push(block_start_addr);

            if (verbose_level > 1) {
                std::cout << "Cycle: Loaded block starting at address " << block_start_addr << "\n";
            }
        }

        read_ack ack = {req.address, cache_map[req.address]};
        if (!read_ack_channel.channel_push(ack)) {
            // Handle full channel if necessary
            std::cerr << "Read Ack Channel is full. Cannot push ack for address " << req.address << "\n";
        }
    }
}
