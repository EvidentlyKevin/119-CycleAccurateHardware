#include "perf_counter.h"
#include <iostream>

perf_counter::perf_counter() : perf_vector(4, 0) {}

void perf_counter::increment_access() {
    perf_vector[0]++;
}

void perf_counter::increment_hit() {
    perf_vector[2]++;
}

void perf_counter::increment_miss() {
    perf_vector[1]++;
}

void perf_counter::add_stall_cycles(int cycles) {
    perf_vector[3] += cycles;
}

void perf_counter::print_summary() const {
    std::cout << "Memory accesses: " << perf_vector[0] << std::endl;
    std::cout << "Hits: " << perf_vector[2] << std::endl;
    std::cout << "Misses: " << perf_vector[1] << std::endl;
    std::cout << "Memory stall cycles: " << perf_vector[3] << std::endl;
}
