#ifndef LOAD_STORE_UNIT_H
#define LOAD_STORE_UNIT_H

#include <queue>
#include <string>
#include "channel.h"

class LoadStoreUnit {
public:
    LoadStoreUnit(size_t ls_queue_size);
    void init_(const std::string& input_fname);
    int cycle();

private:
    size_t ls_queue_size;
    std::queue<int> load_queue, request_queue;
};

#endif // LOAD_STORE_UNIT_H
