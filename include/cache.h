#ifndef CACHE_H
#define CACHE_H

#include <unordered_map>
#include <queue>
#include <string>
#include "channel.h"

class Cache {
public:
    Cache(size_t block_size, size_t cache_size, int n_ways);
    virtual void init_(const std::string& input_fname) = 0;
    virtual void cycle() = 0;
protected:
    size_t block_size, cache_size;
    int n_ways;
    int stall_rem = 0;
    int block_offset_bits, index_bits, tag_bits, set_index_mask;

    int get_bits(int val) {
        return static_cast<int>(log2(val));
    }
};

#endif // CACHE_H
