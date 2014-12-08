/*
    Andrew Daws
    CEC 470-01
    Project 1: Two-Stage Instruction Decoder
    Due: November 7, 2014

    This program creates a simple 2-stage processor that uses four registers: PC, IR, MAR, and ACC.
    The program implements two functions (fetch next instruction cycle and the execute instruction cycle) that,
    simulate the registers and memory of the processor.

    References:
    [1] http://www.cprogramming.com/tutorial/cfileio.html
    [2] http://stackoverflow.com/questions/18693841/read-contents-of-a-file-as-hex-in-c
    [3] http://csciwww.etsu.edu/tarnoff/labs4717/decode_F06_test_suite.pdf
    [*] Also implements some of the example code provided in the project prompt
 */

#define HALT_OPCODE 0x19

// Import libraries
#include <bitset>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

void fetchNextInstruction(void);
void executeInstruction(void);

// Declare variables
std::vector<unsigned char> mem(65536);
unsigned char ACC = 0;	// Accumulator
unsigned char IR = 0;	// Instruction Register
unsigned int MAR = 0;	// Memory Address Register
unsigned int PC = 0;	// Program Counter
unsigned int oldPC;	// Old program counter
unsigned int operand = 0;	// Avoid reading out of file

int main(){ // Reads file and runs two decoder functions
	string hex_line;
	    int i = 0;
	    ifstream mem_read;

	    mem_read.open("mem_in.txt"); // Opens file
	    while (std::getline(mem_read, hex_line)){
	        istringstream stream_in((hex_line)); // Hex to binary conversion
	        for (unsigned hex_to_bin; stream_in >> hex >> hex_to_bin;){
	            mem[i] = hex_to_bin;
	            i++;
	        }
	    }

	    while(PC < mem.size() && (mem.at(PC)) != 0x19){
	        fetchNextInstruction();
	        executeInstruction();
	    }

	    FILE *file_output;
	    file_output = fopen("mem_out.txt", "w");

	    for(unsigned int i = 0; i < mem.size();){
	        for(int j = 0; j < 16; j++){
	            fprintf(file_output, "%02X ", (mem[i]));
	            i++;
	        }
	        fprintf(file_output, "\n");
	    }
	    fclose(file_output);
	return 0;
}

void fetchNextInstruction(void){
	IR = mem[PC];
	oldPC = PC;
	PC++; // Increment PC

	// Mathematical or Logical Operation
	if(IR & 0x80){
		switch(IR & 0x0c){ // Checks the Destination of the instruction
			case 0x00: // Sets the destination as an Indirect (MAR used as a pointer)
				switch(IR & 0x03){ // Checks the two least significant bits for the Source
					case 0: // Indirect (MAR used as a pointer)
						break;
					case 1: // Accumulator ACC
						break;
					case 2: // Constant
						PC++;
						break;
					case 3: // Memory
						PC += 2;
						break;
					default:
						break;
				}
				break;
			case 0x04: // Sets the destination as an ACC
				switch(IR & 0x03){ // Checks the two least significant bits for the Source
					case 0: // Indirect (MAR used as a pointer)
						break;
					case 1: // Accumulator ACC
						break;
					case 2: // Constant
						PC++;
						break;
					case 3: // Memory
						PC += 2;
						break;
					default:
						break;
				}
				break;
			case 0x08: // Sets the destination to Address register MAR
				switch(IR & 0x03){ // Checks the two least significant bits for the Source
					case 0: // Indirect (MAR used as a pointer)
						break;
					case 1: // Accumulator ACC
						break;
					case 2: // Constant
						PC += 2;
						break;
					case 3: // Memory
						PC += 2;
						break;
					default:
						break;
				}
				break;
			case 0x0c: // Sets the destination to Memory
				switch(IR & 0x03){ // Checks the two least significant bits for the Source
					case 0: // Indirect (MAR used as a pointer)
						PC += 2;
						break;
					case 1: // Accumulator ACC
						PC += 2;
						break;
					case 2: // Constant
						PC += 3;
						break;
					case 3: // Memory
						PC += 4;
						break;
					default:
						break;
				}
				break;
			default:
				break;
		}
	}
	else if((IR & 0xf0) == 0){ // 0000 XXXX
		switch(IR & 0x7){
			case 0: // Register = Accumulator ACC / Method = Operand is used as address
				PC += 2;
				break;
			case 1: // Register = Accumulator AAC / Method = Operand is used as a constant
				PC++;
				break;
			case 2: // Register = Accumulator ACC / Method = Indirect (MAR used as a pointer)
				break;
			case 4: // Register = Index register MAR / Method = Operand is used as address
				PC += 2;
				break;
			case 5: // Register = Index register MAR / Method = Operand is used as a constant
				PC += 2;
				break;
			case 6: // Register = Index register MAR / Method = Indirect (MAR used as a pointer)
				break;
			default:
				break;
		}
	}
	else if((IR & 0xf8) == 0X10) // Branches
		PC += 2;
	else
	{

	}

	operand = PC - oldPC - 1;
	PC &= 0xffff;	// Memory check
}

void executeInstruction(void){
	int address;

	if ((IR & 0x80) == 0x80){ // Mathematical or Logical Operation
		// Destination and source values
		int destination;
		int source;

		switch(IR & 0x0c){ // Destination
			case 0x0:
				destination = mem[MAR]; // Indirect (MAR used as a pointer)
				break;
			case 0x4:
				destination = ACC; // Accumulator ACC
				break;
			case 0x8:
				destination = MAR; // Address register MAR
				break;
			case 0xc:
				destination = mem[((mem[oldPC + 1] << 8) + mem[oldPC + 2])]; // Memory
				break;
		}

		switch(IR & 0x03){ // Source
			case 0x0:
				source = mem[MAR]; // Indirect (MAR used as a pointer)
				break;
			case 0x1:
				source = ACC; // Accumulator ACC
				break;
			case 0x2:
				if((IR & 0x0c) == 0x8) // Constant
					source = (mem[PC - 2] << 8) + mem[PC -1];
				else
					source = mem[PC - 1];
				break;
			case 0x3:
				if((IR & 0x0c) == 0x8){ // Memory
					address = ((mem[oldPC + 1] << 8) + mem[oldPC + 2]);
					source = (mem[address] << 8) + mem[address + 1];
				}
				else
					source = mem[((mem[oldPC + 1] << 8) + mem[oldPC + 2])];
				break;
		}

		switch (IR & 0x70){ // Checking the function
			case 0x00: // AND
				destination &= source;
				break;
			case 0x10: // OR
				destination |= source;
				break;
			case 0x20: // XOR
				destination ^= source;
				break;
			case 0x30: // ADD
				destination += source;
				break;
			case 0x40: // SUB
				destination -= source;
				break;
			case 0x50: // INC
				destination++;
				break;
			case 0x60: // DEC
				destination--;
				break;
			case 0x70: // NOT
				destination = !destination;
			break;
			default:
				break;
		}
		// Store destination
		switch(IR & 0x0c){ // Isolate destination id
			case 0x0:
				mem[MAR] = destination & 0xff; // Indirect (MAR used as a pointer)
				break;
			case 0x4:
				ACC = destination & 0xff; // Accumulator ACC
				break;
			case 0x8:
				MAR = destination & 0xffff; // Address register MAR
				break;
			case 0xc:
				mem[((mem[oldPC + 1] << 8) + mem[oldPC + 2])] = destination & 0xff; // Memory
				break;
		}
	}

	else if ((IR & 0xf0) == 0){ // Store/Load function
		if ((IR & 0x08) == 0){ // Store
			if ((IR & 0x04) == 0){ // Storing from Register = Accumulator ACC
				switch(IR & 0x03){
					case 0:
						mem[((mem[oldPC + 1] << 8) + mem[oldPC + 2])] = ACC; // Storing with Method = Operand is used as address
						break;
					case 1: // Operand is used as a Constant
						break;
					case 2:
						mem[MAR] = ACC; // Storing with Method = Indirect (MAR used as a pointer)
						break;
					default:
						break;
				}
			}
			else{ // Storing from Register = Index register MAR
				switch(IR & 0x03){
					case 0:
						mem[((mem[oldPC + 1] << 8) + mem[oldPC + 2])] = (MAR >> 8) & 0xff; // Storing with Method = Operand is used as address
						mem[((mem[oldPC + 1] << 8) + mem[oldPC + 2]) + 1] = MAR & 0xff;
						break;
					case 1: // Operand is used as a Constant
						break;
					case 2:
						mem[MAR] = (MAR >> 8) & 0xff; // Storing with Method = Indirect (MAR used as a pointer)
						mem[MAR + 1] = MAR & 0xff;
						break;
					default:
						break;
				}
			}
		}
		else{ // Load
			if ((IR & 0x04) == 0){ // Loading into register Accumulator ACC
				switch(IR & 0x03){
					case 0:
						ACC = mem[((mem[oldPC + 1] << 8) + mem[oldPC + 2])]; // Method = Operand is used as address
						break;
					case 1:
						ACC = mem[oldPC + 1]; // Method = Operand is used as a constant
						break;
					case 2:
						ACC = mem[MAR]; // Method = Indirect (MAR used as a pointer)
						break;
					default:
						break;
				}
			}
			else{ // Loading into MAR
				int oldMAR = MAR;

				switch(IR & 0x03){
					case 0: // Method = Operand is used as address
						MAR = mem[((mem[oldPC + 1] << 8) + mem[oldPC + 2])];
						MAR <<=8;
						MAR += mem[((mem[oldPC + 1] << 8) + mem[oldPC + 2]) + 1];
						break;
					case 1: // Method = Operand is used as a constant
						MAR = mem[oldPC + 1];
						MAR <<= 8;
						MAR += mem[oldPC + 2];
						break;
					case 2: // Method = Indirect (MAR used as pointer)
						MAR = mem[oldMAR];
						MAR <<= 8;
						MAR += mem[oldMAR + 1];
						break;
					default:
						break;
				}
			}
		}
	}

	else if ((IR & 0xF8) == 0x10){ // Branches
		address = (mem[oldPC + 1] << 8) + mem[oldPC + 2];

		switch (IR & 0x07){
			case 0: // BRA (Unconditional branch/branch always)
				PC = address;
				break;
			case 1: // BRZ (Branch if ACC = 0)
				if (ACC == 0)
					PC = address;
				break;
			case 2: // BNE (Branch if ACC != 0)
				if (ACC != 0)
					PC = address;
				break;
			case 3: // BLT (Branch if ACC < 0)
				if ((ACC & 0x80) != 0)
					PC = address;
				break;
			case 4: // BLE (Branch if ACC <= 0)
				if (((ACC & 0x80) != 0) || (ACC == 0))
					PC = address;
				break;
			case 5: // BGT (Branch if ACC > 0)
				if (((ACC & 0x80) == 0) && (ACC != 0))
					PC = address;
				break;
			case 6: // BGE (Branch if ACC >= 0)
				if ((ACC & 0x80) == 0)
					PC = address;
				break;
			default:
				break;
		}
	}

	else{ // All else is either a "No Operation", "Halt" or and illegal opcode.
		if(IR == 0x18) // NOP "No Operation"
		{

		}

		else if(IR == 0x19) // Halt
		{

		}

		else // Illegal Operation
		{

		}
	}
}
