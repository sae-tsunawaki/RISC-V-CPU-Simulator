#include "Instruction.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <bitset>
using namespace std;

#ifndef CPU_CLASS
#define CPU_CLASS

class CPU {
    public:
        // constructor for CPU object 
        CPU() {
            ALUOp = "";
            ALUOp_next = "";
            ALUSrc = 0;
            RegWrite = 0;
            MemRead = 0;
            MemWrite = 0;
            MemtoReg = 0;
            rs1 = 0; 
            rs2 = 0; 
            rd = 0; 
            imm = 0; 
            rs1_next = 0; 
            rs2_next = 0; 
            rd_next = 0;
            imm_next = 0;
            ALU_result = 0; 
            ALU_result_next = 0; 
            ALU_addr = 0;
            ALU_addr_next = 0;
            Mem_Result = 0; 
            Mem_Result_next = 0;
            type = "";
            type_next = "";
            for (int i = 0; i < 31; i++) {
                regFile[i] = 0;
            }
            nop_count = 0;
        }
        string Fetch(vector<string> instMem, int myPC);
        string get_type(string instruction);
        void Decode(string instr, Instruction instruction);
        int binary_string_to_decimal(string s);
        void Execute(string instr, Instruction instruction);
        void Memory(string instr, int ALU_addr, bool MemWrite, bool MemRead);
        void WriteBack();
        // register file (array of 32 ints)
        int regFile[32]; 
        string ALUOp, ALUOp_next;
        int rs1, rs2, rd, imm, rs1_next, rs2_next, rd_next, imm_next; 
        bool ALUSrc, RegWrite, MemRead, MemWrite, MemtoReg; 
        bool ALUSrc_next, RegWrite_next, MemRead_next, MemWrite_next, MemtoReg_next; 
        int ALU_result, ALU_result_next; 
        int ALU_addr, ALU_addr_next; 
        int Mem_Result, Mem_Result_next; 
        string type, type_next; 
        // data memory (array of 8 bits bitset)
        bitset<8> memory[1024];
        int ReadMem(int addr);
        void WriteMem(int addr);
        int nop_count;
};

#endif