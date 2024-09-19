#ifndef PERF_COUNTER_H
#define PERF_COUNTER_H

#include <vector>

class perf_counter {
public:
    perf_counter();

    void increment_access();
    void increment_hit();
    void increment_miss();
    void add_stall_cycles(int cycles);
    void print_summary() const;

private:
    std::vector<int> perf_vector;
};

#endif // PERF_COUNTER_H
