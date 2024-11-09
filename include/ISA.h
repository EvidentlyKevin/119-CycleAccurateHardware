#include "globals.h"
#include <bitset>
#include "systolic_array.h"
#include "channelM.h"
#include "load_store_unit.h"
#include "mac_unit.h"
#include "memory.h"
#include "perf_counter.h"
//#include "../lib/ISA.cpp"


template<typename T>

struct Instruction{
    bitset<3> opcode;
    bitset<1> addrMode;
    bitset<5> deviceID;
    bitset<32> sourceAddr;
    bitset<32> destAddr;
    bitset<12> ImmSize;
};

class isaTest{
    public:
    isaTest() {}

};


