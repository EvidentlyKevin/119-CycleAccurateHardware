#ifndef GLOBALS_H
#define GLOBALS_H

#include "channel.h"
#include <vector>

extern Channel<read_request> read_request_channel;
extern Channel<read_ack> read_ack_channel;
extern std::vector<int> perf_vector;

#endif // GLOBALS_H
