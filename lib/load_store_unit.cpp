#include "load_store_unit.h"
#include <iostream>
#include <fstream>
#include <sstream>

LoadStoreUnit::LoadStoreUnit(size_t ls_queue_size) : ls_queue_size(ls_queue_size) {}

void LoadStoreUnit::init_(const std::string& input_fname) {
    std::ifstream input_file(input_fname);
    std::string line;
    while (std::getline(input_file, line)) {
        std::istringstream iss(line);
        int first;
        if (iss >> first) {
            request_queue.push(first);
        }
    }
    input_file.close();
}

int LoadStoreUnit::cycle() {
    if (request_queue.empty()) {
        return -1;
    }

    if (load_queue.size() >= ls_queue_size) {
        std::cout << "Load queue full!" << std::endl;
    } else {
        load_queue.push(request_queue.front());
        request_queue.pop();
    }

    if (!read_ack_channel.channel_empty()) {
        read_ack_channel.channel_pop();
    }

    if (read_request_channel.channel_full()) {
        std::cout << "Can't push to channel" << std::endl;
        return 1;
    }

    read_request req1 = {load_queue.front(), 0};
    read_request_channel.channel_push(req1);
    load_queue.pop();
    return 0;
}
