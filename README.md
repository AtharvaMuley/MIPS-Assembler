## MIPS Assembler 
# CMSC 611 Computer Architecture Project

# Team members:
1. Mohit Nayak
2. Pratik Jogdand
3. Atharva Muley

This is an implementation of MIPS Assembler. This assembler currently
supports 5 instructions namely add, addi, and, lw and j. To support more 
isntructions they just have to be added to the instruction structure 
defined at the top in the main file.


# Steps to Run:
1. make clean to clean all the build objects and output
2. run make to compile the files
3. run ./mips-assembler <inputfilenaem>  <outputfilename>

# Note:
The code is compiled and written for linux systems. If it has to be run 
on MAC replace u_intx_t by uintx_t where x is either 8 or 32.

A sample assembly code file code.S is also provided for reference