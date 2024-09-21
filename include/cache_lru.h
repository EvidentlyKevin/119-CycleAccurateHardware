// File: include/cache_lru.h

#ifndef CACHE_LRU_H
#define CACHE_LRU_H

#include "cache.h"
#include <list>

class cache_lru : public Cache {
public:
    // Updated constructor to include 'int verbose'
    cache_lru(size_t block_size, size_t cache_size, int n_ways, perf_counter* perf, int verbose);
    
    // Override virtual functions
    void init_(const std::string& input_fname) override;
    void cycle() override;

private:
    void move_to_back_lru(int addr);
    int verbose_level; // Verbosity level

    // Additional private members if necessary
};

#endif // CACHE_LRU_H
