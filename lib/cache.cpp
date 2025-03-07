// File: lib/cache.cpp

#include "cache.h"
#include <cmath>

Cache::Cache(int block_size, int cache_size, int n_ways, perf_counter* perf)
    : block_size(block_size), cache_size(cache_size), n_ways(n_ways), stall_rem(0), perf(perf) {
    miss_penalty = 4; // Default miss penalty
}
