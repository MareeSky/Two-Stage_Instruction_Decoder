# Two-Stage Instruction Decoder
Andrew Daws <br />
CEC 470-01 <br />
Project 1 <br />
November 7, 2014

### Description
This program creates a simple 2-stage processor that uses four registers: PC, IR, MAR, and ACC.
The program implements two functions (fetch next instruction cycle and the execute instruction cycle) that, simulate the registers and memory of the processor.

### Purpose:
This purpose of this project is to design a simple processor based on the simplified 2-step instruction cycle. Two functions were created in software, one to simulate the fetch next instruction cycle, fetchNextInstruction(), and one to simulate the execute instruction cycle, executeInstruction(). Neither of these functions have parameters nor will they return values. They will operation on global data meant to simulate the registers and memory of the processor. This simulated machine consists of four registers that will be represented in the software with four global variables.

### Included Files:
- README.md
- TwoStageInstrucionDecoder.cpp
- mem_in.txt
- TwoStageInstrucionDecoder.exe

### Instructions:
1. Before even attempting to run the program, a "mem_in.txt" file must be provided (or the original file provided file can be used) and placed in the base directory with the "TwoStageInstructionDecoder.c" file, such that the program will know where to find the input file.
2. Assuming that the test system has the GNU GCC build environment setup and is currently running Windows, the user can simply run the TwoStageInstrucionDecoder.exe executable which will run the program. <br />
3. Otherwise, if the build environment is not setup properly, or Windows is not currently being used, the the user must build and compile the code from "TwoStageInstructionDecoder.c" themselves. <br />
4. In either case the program will read the memory input sample from "mem_in.txt" and output the resulting memory based on the instructions provided into the output file "mem_out.txt", which will also be in the base directory.

### References:
[1] http://www.cprogramming.com/tutorial/cfileio.html <br />
[2] http://stackoverflow.com/questions/18693841/read-contents-of-a-file-as-hex-in-c <br />
[3] http://csciwww.etsu.edu/tarnoff/labs4717/decode_F06_test_suite.pdf <br />
[*] Also implements some of the example code provided in the project prompt
