#ifndef REQUEST_STRUCTS_H
#define REQUEST_STRUCTS_H

#include <cstdint>

struct read_request {
    int32_t address;
    int32_t id;
};

struct read_ack {
    bool read_success;
    int32_t address;
    int32_t data;
};

#endif // REQUEST_STRUCTS_H
