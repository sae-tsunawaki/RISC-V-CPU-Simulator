
/* 
Add all the required standard and developed libraries here. 
Remember to include all those files when you are submitting the project. 
*/
#include "CPU.hpp"
#include "Instruction.hpp"
#include <iostream>
#include <fstream> 
#include <stdio.h>
#include <string>
#include <vector>
using namespace std; 

/*
Put/Define any helper function/definitions you need here
*/

int main (int argc, char* argv[]) 
{
	// open input file to read 
	ifstream fin;
	fin.open(argv[1]);

	// define instruction memory and load the instructions in 
	vector<string> instMem; 
	string line; 
	while (getline(fin, line)) {
		instMem.push_back(line);
	}
	// push in 20 lines of nops 
	for (int i = 0; i < 20; i++) {
		instMem.push_back("0");
	}
	
	// Instantiate Instruction object 
	Instruction myInst; 

	// Instantiate CPU object 
	CPU myCPU; 
	string instruction; 

	// // Clock and PC
	int myClock = 0; 
	int myPC = 0; 

	while (1) {
		// fetch 
		instruction = myCPU.Fetch(instMem, myPC);

		// decode 
		myCPU.Decode(instruction, myInst);

		// execute 
		myCPU.Execute(instruction, myInst);

		// memory
		myCPU.Memory(instruction, myCPU.ALU_addr, myCPU.MemRead, myCPU.MemWrite);
		 
		// write-back 
		myCPU.WriteBack();

		myClock += 1; 
		myPC += 4;

		// next values written to current values 

		myCPU.ALUOp = myCPU.ALUOp_next;
		myCPU.rs1 = myCPU.rs1_next;
		myCPU.rs2 = myCPU.rs2_next;
		myCPU.rd = myCPU.rd_next;
		myCPU.imm = myCPU.imm_next;
		myCPU.ALUSrc = myCPU.ALUSrc_next; 
		myCPU.RegWrite = myCPU.RegWrite_next;
		myCPU.MemRead = myCPU.MemRead_next;
		myCPU.MemWrite = myCPU.MemWrite_next;
		myCPU.MemtoReg = myCPU.MemtoReg_next;
		myCPU.ALU_result = myCPU.ALU_result_next;
		myCPU.ALU_addr = myCPU.ALU_addr_next;
		myCPU.Mem_Result = myCPU.Mem_Result_next;
		myCPU.type = myCPU.type_next;

		// break out of the loop if there are 5 consecutive nop in the pipeline 
		if (myCPU.nop_count == 5)
			break;

		// cout << "(" << myCPU.regFile[10] << ", " << myCPU.regFile[11] << ")" << endl;
	}

	// print the stats
	cout << "(" << myCPU.regFile[10] << ", " << myCPU.regFile[11] << ")" << endl;

	return 0; 
}

