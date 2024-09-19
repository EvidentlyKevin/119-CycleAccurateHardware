#ifndef CACHE_H
#define CACHE_H

#include <unordered_map>
#include <queue>
#include <string>
#include "channel.h"

class Cache {
public:
    Cache(size_t block_size, size_t cache_size, int n_ways);

    void init_(const std::string& input_fname);
    void cycle();
    int get_bits(int val);

private:
    size_t block_size, cache_size;
    int n_ways;
    int stall_rem = 0;
    int block_offset_bits, index_bits, tag_bits, set_index_mask;
    std::unordered_map<int, int> storage_map, cache_map, free_lines_map;
    std::unordered_map<int, std::queue<int>> set_fifo_map;
};

#endif // CACHE_H
