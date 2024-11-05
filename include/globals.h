// File: include/globals.h

#ifndef GLOBALS_H
#define GLOBALS_H

#include "channel.h"
#include "request_structs.h"


// Declare a global verbosity level
extern int global_verbose_level;

// Declare global channels with verbosity
extern Channel<read_request> read_request_channel;
extern Channel<read_ack> read_ack_channel;

// Declare global memory banks size

static const int BANK_ROWS = 8;
static const int BANK_COLS = 8;

#endif // GLOBALS_H
