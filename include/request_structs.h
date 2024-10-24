// File: include/request_structs.h

#ifndef REQUEST_STRUCTS_H
#define REQUEST_STRUCTS_H

#include <cstdint>

struct read_request {
    int32_t address;
};

struct read_ack {
    int32_t address;
    int32_t data;
};

#endif // REQUEST_STRUCTS_H
