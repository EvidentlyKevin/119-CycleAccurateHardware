#include "cache_lru.h"
#include "cache_fifo.h"
#include "load_store_unit.h"
#include "perf_counter.h"
#include <iostream>

int main(int argc, char* argv[]) {
    // Parse command-line arguments and initialize the cache, LSU, etc.
    
    cache_lru lru_cache(16, 4096, 4);
    lru_cache.init_("tests/storage.txt");

    load_store_unit lsu(8);
    lsu.init_("tests/requests.txt");

    perf_counter counter;

    int n_cycles = 16384;
    while (n_cycles--) {
        lru_cache.cycle();
        lsu.cycle();
        counter.increment_access();
        // Additional performance tracking logic...
    }

    counter.print_summary();
    return 0;
}
