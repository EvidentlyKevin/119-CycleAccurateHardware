// File: include/load_store_unit.h

#ifndef LOAD_STORE_UNIT_H
#define LOAD_STORE_UNIT_H

#include <queue>
#include <string>
#include "channel.h"
#include "request_structs.h"

class LoadStoreUnit {
public:
    // Updated constructor to include 'int verbose'
    LoadStoreUnit(size_t ls_queue_size, int verbose);
    
    void init_(const std::string& input_fname);
    int cycle();

private:
    size_t ls_queue_size;
    std::queue<int> load_queue, request_queue;
    int verbose_level; // Verbosity level
};

#endif // LOAD_STORE_UNIT_H
