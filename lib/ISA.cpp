#include <iostream>
#include <fstream>
#include <array>
#include <bitset>
#include <string>
#include "../include/memory.h"
#include "../include/ISA.h"
#include "../include/systolic_array.h"
//#include "../include/systolic_array.h"
//#include "../include/systolic_array.tpp"
using namespace std; 

//7.14b neural nets/systolic array figure and explanation in textbook 

class instrFunctions {
    private:
        int num_cycles = 44;
        const int SIZE = 8;
        const size_t CHANNEL_CAPACITY = 4;
        Systolic_Array<int> systolicArray;
        std::vector<int> activations; // Assuming activations is a vector of integers
        Memory mem;
        std::vector<std::vector<int>> weights;
        std::vector<channelM<int>> memoryToSystolicChannels;
    public:
        instrFunctions() : systolicArray(SIZE), weights(SIZE, std::vector<int>(SIZE)) {}

        void read_host_memory(bitset<32> sourceAddr, bitset<32> destAddr, bitset<12> ImmSize) {
             mem.initBanks();
            // Display the contents of the memory banks
        for (int i = 0; i < mem.MemBanks; i++) {
        std::cout << "Memory Bank " << i << ":\n";
        for (int j = 0; j < BANK_ROWS; j++) {
            for (int k = 0; k < BANK_COLS; k++) {
                std::cout << mem.MemoryBanks[i].Data[j][k] << " ";
            }
            std::cout << std::endl;
        }
        std::cout << "---------------------------\n";
    }

            // Implementation of reading from host memory
            // reading memory via pushing the data?
        }

        void read_weights(bitset<32> sourceAddr, bitset<32> destAddr, bitset<12> ImmSize) {
            for (int i = 0; i < 8; ++i) {
            for (int j = 0; j < 8; ++j) {
            weights[i][j] = 1; // Simple weights
            cout << weights[i][j] << " ";
            }
            cout << "\n";
    }
        systolicArray.setWeights(weights);
            // Implementation of reading weights
            // call set/read weights function of systolic array functions
        }

        void matrix_multiply(bitset<32> sourceAddr, bitset<32> destAddr, bitset<12> ImmSize) {
            // Implementation of matrix multiplication
        for (int cycle = 0; cycle < num_cycles; ++cycle) {
        // Memory pushes data into channels
        mem.pushData(memoryToSystolicChannels, cycle, true);


        // Set input activations from memory channels
        systolicArray.setInputActivationsFromChannels(memoryToSystolicChannels, true);


        // Run one cycle of the systolic array
        systolicArray.cycle();
        std::cout << "---------------------------------------------\n";
    }
    std::vector<int> outputs = systolicArray.getOutputs();

    // Print the outputs
    std::cout << "Systolic Array Outputs with Memory Input:\n";
    for (size_t i = 0; i < outputs.size(); ++i) {
        std::cout << "Output[" << i << "]: " << outputs[i] << "\n";
    }
        }

        void activate(bitset<32> sourceAddr, bitset<32> destAddr, bitset<12> ImmSize) {                
                //why? included needed systolic header and tpp file
        }

        void write_host_memory(bitset<32> sourceAddr, bitset<32> destAddr, bitset<12> ImmSize) {

        for (int i = 0; i < SIZE; ++i) {

        memoryToSystolicChannels.emplace_back(CHANNEL_CAPACITY);
    }
            // Implementation of writing to host memory
            // no clue tbf 
        }

        void nop() {
            cout << "No Operation!" << endl;
            // No operation
        }
        };


class OpCalls {
bitset<84> exampleInstruction; // 85-bit bitset
    private:
        int pc = 0; // program counter
        instrFunctions instrFuncs; // Create an instance of instrFunctions
        Instruction<int> parseInstr(bitset<84> memInstrRawInstr) {
            Instruction<int> instr;

            // Extract opcode (bits 84 to 81, 3 bits)
            for (int i = 0; i < 3; i++) {
            instr.opcode[i] = memInstrRawInstr[83 - i];
            }

            // Extract addrMode (bit 80, 1 bit)
            instr.addrMode.set(0, memInstrRawInstr[80]);

            // Extract deviceID (bits 79 to 75, 5 bits)
            for (int i = 0; i < 5; i++) {
            instr.deviceID[i] = memInstrRawInstr[79 - i];
            }

            // Extract sourceAddr (bits 74 to 43, 32 bits)
            for (int i = 0; i < 32; i++) {
            instr.sourceAddr[i] = memInstrRawInstr[75 - i + 1];
            }

            // Extract destAddr (bits 42 to 11, 32 bits)
            for (int i = 0; i < 32; i++) {
            instr.destAddr[i] = memInstrRawInstr[43 - i + 1];
            }

            // Extract ImmSize (bits 10 to 0, 12 bits)
            for (int i = 0; i < 11; i++) {
            instr.ImmSize[i] = memInstrRawInstr[11 - i + 1];
            }

            return instr;
        }

        void opcodeSelect(const Instruction<int>& instr){    
            bitset<3> opcode = instr.opcode;
            bitset<32> sourceAddr = instr.sourceAddr;
            bitset<32> destAddr = instr.destAddr;
            bitset<12> ImmSize = instr.ImmSize;

            if (opcode == bitset<3>("000")) {
                cout << "Read Host Memory" << endl;
                instrFuncs.read_host_memory(sourceAddr, destAddr, ImmSize);
            } else if (opcode == bitset<3>("001")) {
                cout << "Read Weights" << endl;     
                instrFuncs.read_weights(sourceAddr, destAddr, ImmSize);
            } else if (opcode == bitset<3>("010")) {
                cout << "Matrix Multiply" << endl;  
                instrFuncs.matrix_multiply(sourceAddr, destAddr, ImmSize);
            } else if (opcode == bitset<3>("011")) {
                cout << "Activate" << endl;
                instrFuncs.activate(sourceAddr, destAddr, ImmSize);
            } else if (opcode == bitset<3>("100")) {
                cout << "Write Host Memory" << endl;
                instrFuncs.write_host_memory(sourceAddr, destAddr, ImmSize);
            } else {
                instrFuncs.nop();
            }


            
        }



    public:
        bitset<84> currInstr[1024];

        bitset<84> fetch() {
            // Fetch instruction from instruction memory
            bitset<84> raw_instruction(currInstr[pc]);
            pc++;
            return raw_instruction;
        }

        Instruction<int> decode(bitset<84>& raw_instruction) {
            // Decode the raw instruction into structured format
            return parseInstr(raw_instruction);
        }

        void execute(const Instruction<int>& instr) {
            // Execute the decoded instruction
            if (instr.addrMode == 0) {
                cout << "Host to Device transfer" << endl;
            } else {
                cout << "Device to Host transfer" << endl;
            }
            cout << "Device ID: " << bitset<5>(instr.deviceID) << endl;

            opcodeSelect(instr);

            cout << "Operation: " << bitset<3>(instr.opcode) << endl;

            cout << "Source Address: " << bitset<32>(instr.sourceAddr) << endl;

            cout << "Destination Address: " << bitset<32>(instr.destAddr) << endl;

            cout << "Immediate Size: " << bitset<12>(instr.ImmSize) << endl;    
        }

        // Method to load an instruction into memory
        void loadInstruction(bitset<84> instruction, int address) {
            for (address=0; address < 1024; address++) {
                 currInstr[address] = instruction;
            }
        }

        void setMemInstrRawInstr(bitset<84> instruction) {
            for(int i = 0; i< 1024; i++){
            bitset<84> memInstrRawInstr = currInstr[i];
            }
        }

        // Method to run the instruction pipeline
        void runPipeline() {
            bitset<84> raw_inst = fetch();
            Instruction<int> decoded_inst = decode(raw_inst);
            execute(decoded_inst);
        }

       /* template<std::size_t N>
        void reverse(std::bitset<N> &b) {
            for(std::size_t i = 0; i < N/2; ++i) {
                bool t = b[i];
                b[i] = b[N-i-1];
                b[N-i-1] = t;
            }
        } */
};