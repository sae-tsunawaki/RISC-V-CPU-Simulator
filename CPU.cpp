#include "CPU.hpp"
#include <tgmath.h>

string CPU::Fetch(vector<string> instMem, int myPC) {

    // get the 4 bytes that make up an instruction based on the PC 
    string byte_1 = instMem[myPC];
    string byte_2 = instMem[myPC+1];
    string byte_3 = instMem[myPC+2];
    string byte_4 = instMem[myPC+3];

    // convert the string values to int 
    int int_byte_1 = stoi(byte_1);
    int int_byte_2 = stoi(byte_2);
    int int_byte_3 = stoi(byte_3);
    int int_byte_4 = stoi(byte_4);

    // convert the int values to binary  
    bitset<8> binary_byte_1(int_byte_1);
    bitset<8> binary_byte_2(int_byte_2);
    bitset<8> binary_byte_3(int_byte_3);
    bitset<8> binary_byte_4(int_byte_4);

    // convert binary back to string and concatenate to get instruction 
    string instr_byte_1 = binary_byte_1.to_string();
    string instr_byte_2 = binary_byte_2.to_string();
    string instr_byte_3 = binary_byte_3.to_string();
    string instr_byte_4 = binary_byte_4.to_string();

    // concatenate the strings to create the instruction 
    string instr = instr_byte_4 + instr_byte_3 + instr_byte_2 + instr_byte_1;

    return instr; 
}

string CPU::get_type(string instruction) {
    // get the type of instruction from the opcode
    // sets appropriate control signals 
    Instruction instr; 
    instr.opcode = instruction.substr(25,7);

    // R-type 
    if (instr.opcode == "0110011") {
        RegWrite_next = 1; 
        type_next = "R";
    }

    // I-type
    if (instr.opcode == "0010011") {
        RegWrite_next = 1;
        ALUSrc_next = 1;
        type_next = "I";
    }

    // LW
    if (instr.opcode == "0000011") {
        RegWrite_next = 1;
        ALUSrc_next = 1;
        MemRead_next = 1;
        MemtoReg_next = 1;
        type_next = "LW";
    }

    // SW 
    if (instr.opcode == "0100011") {
        ALUSrc_next = 1;
        MemWrite_next = 1;
        type_next = "SW";
    }

    return type_next;
}

// helper function to convert a binary string to a decimal 
int CPU::binary_string_to_decimal(string s) {
    int decimal = 0; 
    int l = s.length(); 
    for (int i = 0; i < l; i++) {
        if (s[i] == '1') {
            decimal += pow(2, l-i-1);
        }
    }
    return decimal; 
}

void CPU::Decode(string instr, Instruction instruction) {

    // get the instruction type from the opcode
    string type = get_type(instr);
    instruction.opcode = instr.substr(25,7);

    // if the instruction is a nop, increment the nop_count
    if (instruction.opcode == "0000000")
        nop_count++;

    // extract funct3 
    instruction.funct3 = instr.substr(17,3);

    // extract rs1 
    instruction.rs1 = instr.substr(12,5);

    // extract rs2 
    instruction.rs2 = instr.substr(7,5);

    // extract funct7
    instruction.funct7 = instr.substr(0,7);

    // extract rd 
    instruction.rd = instr.substr(20,5); 

    // R-type  
    if (type == "R") {
        // ADD 
        if (instruction.funct3 == "000" && instruction.funct7 == "0000000") {
            ALUOp_next = "ADD";
        }

        // SUB 
        else if (instruction.funct3 == "000" && instruction.funct7 == "0100000") {
            ALUOp_next = "SUB";
        }

        // OR 
       else if (instruction.funct3 == "110") {
            ALUOp_next = "OR";
        }

        // AND
        else if (instruction.funct3 == "111") {
            ALUOp_next = "AND";
        }
    }

    // I-type 
    if (type == "I") {
        // set the immediate values 
        instruction.immediate = instr.substr(0, 12);
        // I-type instruction do not use rs2 
        instruction.rs2 = "00000";
        // ADDI 
        if (instruction.funct3 == "000") {
            ALUOp_next = "ADDI";
        }
        // ORI 
        if (instruction.funct3 == "110") {
            ALUOp_next = "ORI";
        }
        // ANDI 
        if (instruction.funct3 == "111") {
            ALUOp_next = "ANDI";
        }
    }

    // LW 
    if (type == "LW") {
        // set the immediate values 
        instruction.immediate = instr.substr(0, 12);
        // LW instructions do not use rs2 
        instruction.rs2 = "00000";
        ALUOp_next = "LW";
    }

    // SW 
    if (type == "SW") {
        // set the immediate values 
        instruction.immediate = instr.substr(0, 7) + instr.substr(20,5);
        // SW instructions do not use rd 
        instruction.rd = "00000";
        ALUOp_next = "SW";
    }

    // read rs1 
    rs1_next = binary_string_to_decimal(instruction.rs1); 

    // read rs2 
    rs2_next = binary_string_to_decimal(instruction.rs2); 

    // read rd 
    rd_next = binary_string_to_decimal(instruction.rd); 

    // read immediate (positive)
    if (instruction.immediate[0] != '1') {
        imm_next = binary_string_to_decimal(instruction.immediate); 
    }
    // read immediate (negative)
    else {
        int l = instruction.immediate.length();
        for (int i = 0; i < l; i++) {
            if (i == 0)
                imm_next = pow(2, l-1)*(-1);
            if (instruction.immediate[i] == '1' && i != 0) {
                imm_next += pow(2, l-i-1);
            }
        }
    }
}

void CPU::Execute(string instr, Instruction instruction) { 
    // compute ALU operations for each instruction 
    if (ALUOp == "ADD") {
        ALU_result_next = regFile[rs1] + regFile[rs2]; 
    }
    if (ALUOp == "SUB") {
        ALU_result_next = regFile[rs1] - regFile[rs2]; 
    }
    if (ALUOp == "OR") {
        ALU_result_next = regFile[rs1] | regFile[rs2]; 
    }
    if (ALUOp == "AND") {
        ALU_result_next = regFile[rs1] & regFile[rs2]; 
    }
    if (ALUOp == "ADDI") {
        ALU_result_next = regFile[rs1] + imm; 
    }
    if (ALUOp == "ORI") {
        ALU_result_next = regFile[rs1] | imm; 
    }
    if (ALUOp == "ANDI") {
        ALU_result_next = regFile[rs1] & imm; 
    }
    // memory type instructions (compute address)
    if (ALUOp == "LW") {
        ALU_addr_next = regFile[rs1] + imm; 
    }
    if (ALUOp == "SW") {
        ALU_addr_next = regFile[rs1] + imm; 
    }
}

void CPU::Memory(string instr, int ALU_addr, bool MemWrite, bool MemRead) {
    // no need for memory access 
    if (!MemRead && !MemWrite)
        return; 

    // load instruction 
    if (ALUOp == "LW") {
        Mem_Result_next = ReadMem(ALU_addr_next); 
    }
    // store instruction 
    if (ALUOp == "SW") {
        WriteMem(ALU_addr_next);
    }
}

int CPU::ReadMem(int addr) {
    // read 4 bytes of memory starting from the input address as strings 
    string byte_1 = memory[addr].to_string();
    string byte_2 = memory[addr+1].to_string();
    string byte_3 = memory[addr+2].to_string();
    string byte_4 = memory[addr+3].to_string(); 
    string res = byte_4 + byte_3 + byte_2 + byte_1;
    // convert concatenated string to decimal 
    return binary_string_to_decimal(res);
}

void CPU::WriteMem(int addr) {
    // obtain value to be written 
    int write_val = regFile[rs2]; 

    // get 32 bit representation of write_val 
    bitset<32> binary_write_val(write_val);

    // get string representation of 32 bit binary write_val 
    string string_write_val = binary_write_val.to_string();

    // get string representation of each byte 
    string byte_1 = string_write_val.substr(24,8);
    string byte_2 = string_write_val.substr(16,8);
    string byte_3 = string_write_val.substr(8,8);
    string byte_4 = string_write_val.substr(0,8);

    // convert the string values of each byte to int 
    int int_byte_1 = binary_string_to_decimal(byte_1);
    int int_byte_2 = binary_string_to_decimal(byte_2);
    int int_byte_3 = binary_string_to_decimal(byte_3);
    int int_byte_4 = binary_string_to_decimal(byte_4);

    // convert the int values to binary  
    bitset<8> binary_byte_1(int_byte_1);
    bitset<8> binary_byte_2(int_byte_2);
    bitset<8> binary_byte_3(int_byte_3);
    bitset<8> binary_byte_4(int_byte_4);

    // store the appropriate bytes into memory 
    memory[addr] = binary_byte_1;
    memory[addr+1] = binary_byte_2;
    memory[addr+2] = binary_byte_3;
    memory[addr+3] = binary_byte_4;
}

void CPU::WriteBack() {
    // if we are writing to a register (R-type and I-type)
    if (ALUOp == "ADD" || ALUOp == "SUB" || ALUOp == "OR" || ALUOp == "AND" 
        || ALUOp == "ADDI" || ALUOp == "ORI" || ALUOp == "ANDI") {
        regFile[rd] = ALU_result_next;
    }
    // if we are writing to register from data memory (LW)
    if (ALUOp == "LW") {
        regFile[rd] = Mem_Result_next;
    }
}