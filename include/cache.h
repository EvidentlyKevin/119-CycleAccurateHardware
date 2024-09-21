#ifndef CACHE_H
#define CACHE_H

#include <unordered_map>
#include <queue>
#include <string>
#include <cmath>
#include <list>

#include "channel.h"
#include "request_structs.h"
#include "perf_counter.h"

class Cache {
public:
    Cache(size_t block_size, size_t cache_size, int n_ways, perf_counter* perf);
    virtual ~Cache() = default;
    virtual void init_(const std::string& input_fname) = 0;
    virtual void cycle() = 0;
protected:
    size_t block_size, cache_size;
    int n_ways;
    int stall_rem;
    int miss_penalty;

    int block_offset_bits, index_bits, tag_bits, set_index_mask;

    // Common data structures for cache operation
    std::unordered_map<int, int> storage_map;    // Simulated main memory
    std::unordered_map<int, int> cache_map;      // Cache storage
    std::unordered_map<int, int> free_lines_map; // Tracks free lines per set

    // For FIFO and LRU replacement policies
    std::unordered_map<int, std::queue<int>> set_fifo_map; // For FIFO
    std::unordered_map<int, std::list<int>> set_lru_map;   // For LRU

    perf_counter* perf; // Pointer to performance counter

    int get_bits(int val) {
        return static_cast<int>(std::log2(val));
    }

    int get_set_index(int addr) {
        return (addr >> block_offset_bits) & set_index_mask;
    }

    int get_tag(int addr) {
        return addr >> (block_offset_bits + index_bits);
    }
};

#endif // CACHE_H
