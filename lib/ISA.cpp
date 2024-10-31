#include <iostream>
#include <fstream>
#include <array>
#include <bitset>

using namespace std; 

struct Instruction{
    bitset<3> opcode;
    bitset<1> addrMode;
    bitset<5> deviceID;

    bitset<32> sourceAddr;
    bitset<32> destAddr;
    bitset<12> ImmSize;
};
  


class instrFunctions{
    public:
        void read_host_memory(bitset<32> sourceAddr, bitset<32> destAddr, bitset<12> ImmSize) {
            // Implementation of reading from host memory
        }

        void read_weights(bitset<32> sourceAddr, bitset<32> destAddr, bitset<12> ImmSize) {
            // Implementation of reading weights
        }

        void matrix_multiply(bitset<32> sourceAddr, bitset<32> destAddr, bitset<12> ImmSize) {
            // Implementation of matrix multiplication
        }

        void activate(bitset<32> sourceAddr, bitset<32> destAddr, bitset<12> ImmSize) {
            // Implementation of activation function
        }

        void write_host_memory(bitset<32> sourceAddr, bitset<32> destAddr, bitset<12> ImmSize) {
            // Implementation of writing to host memory
        }

        void nop() {
            // No operation
        }
};


class OpCalls {
bitset<84> exampleInstruction; // 85-bit bitset
    private:
        int pc = 0; // program counter
        instrFunctions instrFuncs; // Create an instance of instrFunctions
        Instruction parseInstr(bitset<84> memInstrRawInstr) {
            Instruction instr;

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

        void opcodeSelect(const Instruction& instr){    
            bitset<3> opcode = instr.opcode;
            bitset<32> sourceAddr = instr.sourceAddr;
            bitset<32> destAddr = instr.destAddr;
            bitset<12> ImmSize = instr.ImmSize;

            if (opcode == bitset<3>("000")) {
                instrFuncs.read_host_memory(sourceAddr, destAddr, ImmSize);
            } else if (opcode == bitset<3>("001")) {
                instrFuncs.read_weights(sourceAddr, destAddr, ImmSize);
            } else if (opcode == bitset<3>("010")) {
                instrFuncs.matrix_multiply(sourceAddr, destAddr, ImmSize);
            } else if (opcode == bitset<3>("011")) {
                instrFuncs.activate(sourceAddr, destAddr, ImmSize);
            } else if (opcode == bitset<3>("100")) {
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

        Instruction decode(bitset<84>& raw_instruction) {
            // Decode the raw instruction into structured format
            return parseInstr(raw_instruction);
        }

        void execute(const Instruction& instr) {
            // Execute the decoded instruction
            if (instr.addrMode == 0) {
                cout << "Host to Device transfer" << endl;
            } else {
                cout << "Device to Host transfer" << endl;
            }
            cout << "Device ID: " << bitset<5>(instr.deviceID) << endl;

            opcodeSelect(instr);

            cout << "Operation: " << bitset<3>(instr.opcode) << endl;
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
            Instruction decoded_inst = decode(raw_inst);
            execute(decoded_inst);
        }

        template<std::size_t N>
        void reverse(std::bitset<N> &b) {
            for(std::size_t i = 0; i < N/2; ++i) {
                bool t = b[i];
                b[i] = b[N-i-1];
                b[N-i-1] = t;
            }
        }
};

int main() {
    OpCalls opCalls;

    // Create an 85-bit instruction example using bitset. Adjust bits as necessary for your desired opcode and parameters.
    // For this example, let's set opcode = 0b010 (matrix_multiply), addrMode = 1, deviceID = 0b10101,
    // sourceAddr = 0x00000010, destAddr = 0x00000020, and ImmSize = 0x100
    bitset<84> exampleInstruction("010110101000000000000000000000000000000100000000000000000000000000001000000001000100");
    bitset<84> exampleInstruction2("01100000000000000000000000000000000000000000000000000000000000000000000000000000000");
    //opCalls.reverse(exampleInstruction);

    cout << "Instruction: " << exampleInstruction << endl;
          

    // Load the instruction into the OpCalls class
    //opCalls.loadInstruction(exampleInstruction, 0);
    opCalls.loadInstruction(exampleInstruction, 0);

    // Run the instruction pipeline (fetch, decode, execute)
    opCalls.runPipeline();

    return 0;
} 
