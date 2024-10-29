#include <iostream>
#include <fstream>
#include <array>
#include <bitset>
#include <cstdint>

using namespace std; 

struct Instruction{
    uint8_t opcode : 3;
    uint8_t addrMode: 1; 
    uint8_t deviceID : 5;
    uint32_t sourceAddr : 32;
    uint32_t destAddr : 32;
    uint32_t ImmSize : 12;
};


class instrFunctions{
    public:
        void read_host_memory(uint32_t sourceAddr, uint32_t destAddr, uint32_t ImmSize) {
            // Implementation of reading from host memory
        }

        void read_weights(uint32_t sourceAddr, uint32_t destAddr, uint32_t ImmSize) {
            // Implementation of reading weights
        }

        void matrix_multiply(uint32_t sourceAddr, uint32_t destAddr, uint32_t ImmSize) {
            // Implementation of matrix multiplication
        }

        void activate(uint32_t sourceAddr, uint32_t destAddr, uint32_t ImmSize) {
            // Implementation of activation function
        }

        void write_host_memory(uint32_t sourceAddr, uint32_t destAddr, uint32_t ImmSize) {
            // Implementation of writing to host memory
        }

        void nop() {
            // No operation
        }
};


class OpCalls {
std::bitset<85> exampleInstruction; // 85-bit bitset
    private:
        int pc = 0; // program counter
        instrFunctions instrFuncs; // Create an instance of instrFunctions

        Instruction parseInstr(bitset<85> memInstrRawInstr){
            Instruction instr;

        // Extract opcode (bits 84 to 82, 3 bits)
            instr.opcode = static_cast<uint8_t>((memInstrRawInstr >> 81).to_ulong() & 0x7);

        // Extract addrMode (bit 81, 1 bit)
            instr.addrMode = memInstrRawInstr[81];

        // Extract deviceID (bits 80 to 76, 5 bits)
            instr.deviceID = static_cast<uint8_t>((memInstrRawInstr >> 76).to_ulong() & 0x1F);

        // Extract sourceAddr (bits 75 to 44, 32 bits)
            instr.sourceAddr = static_cast<uint32_t>((memInstrRawInstr >> 44).to_ulong() & 0xFFFFFFFF);

        // Extract destAddr (bits 43 to 12, 32 bits)
            instr.destAddr = static_cast<uint32_t>((memInstrRawInstr >> 12).to_ulong() & 0xFFFFFFFF);

        // Extract ImmSize (bits 11 to 0, 12 bits)
            instr.ImmSize = static_cast<uint16_t>(memInstrRawInstr.to_ulong() & 0xFFF);

            return instr;
        }

        void opcodeSelect(const Instruction& instr){    
            switch (instr.opcode) {
                case 0b000: instrFuncs.read_host_memory(instr.sourceAddr, instr.destAddr, instr.ImmSize);   
                break;
                case 0b001: instrFuncs.read_weights(instr.sourceAddr, instr.destAddr, instr.ImmSize);     
                break; 
                case 0b010: instrFuncs.matrix_multiply(instr.sourceAddr, instr.destAddr, instr.ImmSize); //call systolic array operations with 256 x 256 instead //need buffer address and destination in accumulator?
                break;
                case 0b011: instrFuncs.activate(instr.sourceAddr, instr.destAddr, instr.ImmSize );   
                break; 
                case 0b100: instrFuncs.write_host_memory(instr.sourceAddr, instr.destAddr, instr.ImmSize ); 
                break;
                default: instrFuncs.nop(); 
                break;
            }
        }



    public:
        int currInstr[1024];

        bitset<85> fetch() {
            // Fetch instruction from instruction memory
            bitset<85> raw_instruction(currInstr[pc]);
            pc++;
            return raw_instruction;
        }

        Instruction decode(bitset<85>& raw_instruction) {
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
            cout << "Device ID: " << instr.deviceID << endl;
            opcodeSelect(instr);
            cout << "Operation: " << instr.opcode << endl;
        }

        // Method to load an instruction into memory
        void loadInstruction(bitset<85> instruction, int address) {
            for (address=0; address < 1024; address++) {
                currInstr[address] = instruction.to_ulong();
            }
        }

        void setMemInstrRawInstr(bitset<85> instruction) {
            for(int i = 0; i< 1024; i++){
            bitset<85> memInstrRawInstr = currInstr[i];
            }
        }

        // Method to run the instruction pipeline
        void runPipeline() {
            bitset<85> raw_inst = fetch();
            Instruction decoded_inst = decode(raw_inst);
            execute(decoded_inst);
        }
};


int maine() {
    OpCalls opCalls;

    // Create an 85-bit instruction example using bitset. Adjust bits as necessary for your desired opcode and parameters.
    // For this example, let's set opcode = 0b010 (matrix_multiply), addrMode = 1, deviceID = 0b10101,
    // sourceAddr = 0x00000010, destAddr = 0x00000020, and ImmSize = 0x100
    bitset<85> exampleInstruction("01010100000000000000000000000000000000010000000000000000000000000000100000000100");

    // Load the instruction into the OpCalls class
    opCalls.loadInstruction(exampleInstruction, 0);

    // Run the instruction pipeline (fetch, decode, execute)
    opCalls.runPipeline();

    return 0;
} 