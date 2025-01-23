// File: include/globals.h

#ifndef PERF_COUNTER_H
#define PERF_COUNTER_H

#include <iostream>

class perf_counter {
public:
    perf_counter();

    void increment_access();
    void increment_hit();
    void increment_miss();
    void add_stall_cycles(int cycles);
    void print_summary() const;

private:
    int total_accesses;
    int hits;
    int misses;
    int stall_cycles;
};

#endif // PERF_COUNTER_H
