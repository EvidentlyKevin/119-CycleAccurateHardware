#ifndef CACHE_LRU_H
#define CACHE_LRU_H

#include "cache.h"

class cache_lru : public cache {
public:
    cache_lru(size_t block_size, size_t cache_size, int n_ways);
    void init_(const std::string& input_fname) override;
    void cycle() override;

private:
    void move_to_back_lru(int addr);

    std::unordered_map<int, int> storage_map, cache_map, free_lines_map;
    std::unordered_map<int, std::queue<int>> set_fifo_map;
    int stall_rem;
};

#endif // CACHE_LRU_H
