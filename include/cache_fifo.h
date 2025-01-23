// File: include/cache_fifo.h

#ifndef CACHE_FIFO_H
#define CACHE_FIFO_H

#include "cache.h"
#include <queue>

class cache_fifo : public Cache {
public:
    // Updated constructor to include 'int verbose'
    cache_fifo(int block_size, int cache_size, int n_ways, perf_counter* perf, int verbose);
    
    // Override virtual functions
    void init_(const std::string& input_fname) override;
    void cycle() override;

private:
    int verbose_level; // Verbosity level

    // Additional private members if necessary
};

#endif // CACHE_FIFO_H
