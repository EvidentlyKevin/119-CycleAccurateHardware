// File: lib/load_store_unit.cpp

#include "load_store_unit.h"
#include "globals.h"
#include <iostream>
#include <fstream>
#include <sstream>

LoadStoreUnit::LoadStoreUnit(size_t ls_queue_size, int verbose)
    : ls_queue_size(ls_queue_size), verbose_level(verbose) {}

void LoadStoreUnit::init_(const std::string& input_fname) {
    std::ifstream input_file(input_fname);
    if (!input_file.is_open()) {
        std::cerr << "Error: Cannot open requests file " << input_fname << "\n";
        return;
    }
    std::string line;
    while (std::getline(input_file, line)) {
        std::istringstream iss(line);
        int address;
        if (iss >> address) {
            request_queue.push(address);
            if (verbose_level > 1) {
                std::cout << "LoadStoreUnit: Queued address " << address << "\n";
            }
        }
    }
    input_file.close();

    if (verbose_level > 1) {
        std::cout << "LoadStoreUnit Initialized with " << request_queue.size() << " requests.\n";
    }
}

int LoadStoreUnit::cycle() {
    if (request_queue.empty() && load_queue.empty()) {
        if (verbose_level > 1) {
            std::cout << "Cycle: LoadStoreUnit is idle.\n";
        }
        return -1; // No more requests
    }

    // Move requests from request_queue to load_queue if possible
    while (!request_queue.empty() && load_queue.size() < ls_queue_size) {
        load_queue.push(request_queue.front());
        if (verbose_level > 1) {
            std::cout << "Cycle: LoadStoreUnit queued address " << request_queue.front() << "\n";
        }
        request_queue.pop();
    }

    // Process read acknowledgments
    read_ack ack;
    while (read_ack_channel.channel_pop(ack)) {
        // Process the acknowledgment, e.g., print the data
        std::cout << "LSU received data for address " << ack.address << ": " << ack.data << std::endl;
    }

    // Send read requests
    if (!load_queue.empty() && !read_request_channel.channel_full()) {
        int address = load_queue.front();
        read_request req = {address};
        if (read_request_channel.channel_push(req)) {
            if (verbose_level > 0) {
                std::cout << "Cycle: LoadStoreUnit sent read request for address " << address << "\n";
            }
            load_queue.pop();
        } else {
            if (verbose_level > 0) {
                std::cerr << "LoadStoreUnit: Read request channel is full. Cannot push request for address " << address << "\n";
            }
            // Optionally, implement retry logic or handle the full channel
        }
    }

    return 0;
}
