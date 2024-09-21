#include "globals.h"

// Initialize channels without verbosity
Channel<read_request> read_request_channel(4);
Channel<read_ack> read_ack_channel(4);
