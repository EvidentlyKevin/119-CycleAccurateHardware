#ifndef CACHE_FIFO_H
#define CACHE_FIFO_H

#include "cache.h"

class cache_fifo : public cache {
public:
    cache_fifo(size_t block_size, size_t cache_size, int n_ways);
    void init_(const std::string& input_fname) override;
    void cycle() override;

private:
    std::unordered_map<int, int> storage_map, cache_map, free_lines_map;
    std::unordered_map<int, std::queue<int>> set_fifo_map;
    int stall_rem;
};

#endif // CACHE_FIFO_H
