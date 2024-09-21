#include "perf_counter.h"

perf_counter::perf_counter() : total_accesses(0), hits(0), misses(0), stall_cycles(0) {}

void perf_counter::increment_access() {
    total_accesses++;
}

void perf_counter::increment_hit() {
    hits++;
    total_accesses++;
}

void perf_counter::increment_miss() {
    misses++;
    total_accesses++;
}

void perf_counter::add_stall_cycles(int cycles) {
    stall_cycles += cycles;
}

void perf_counter::print_summary() const {
    std::cout << "Memory accesses: " << total_accesses << std::endl;
    std::cout << "Hits: " << hits << std::endl;
    std::cout << "Misses: " << misses << std::endl;
    std::cout << "Memory stall cycles: " << stall_cycles << std::endl;
}
