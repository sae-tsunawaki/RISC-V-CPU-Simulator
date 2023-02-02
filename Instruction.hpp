#include <iostream>
#include <string>
using namespace std; 

#ifndef INSTR_CLASS
#define INSTR_CLASS

class Instruction {
    public: 
        Instruction() {}
        string opcode; 
        string rd;
        string rs1;
        string rs2;
        string funct3;
        string funct7; 
        string immediate; 
};

#endif