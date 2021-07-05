# Disassembler

Omar Elsewedy 900183436
Hussein Elsotouhy  900181888

#Implementation details
The disassembler is a project that converts binary files to assembly files. We used RISC-V ISA in this projectWe divided the codes based the types of instructions: RV32I (R-type, I-type, S-type, B-type, U-type, J-type) and RV32IC (16 bit, half word, compressed instructions). All 32-bit instructions output correctly, however outputs in hexadecimal. Firstly, we differentiated between the types using the opcode and differentiated between each instruction using function 3 and function 7 in 32-bit instructions and function 2, function 4, and function 6 in the 16-bit instructions. Moreover, we did not include any pseudo instructions and did not include any instruction we did not take that belong to RISC V 64 and 128 ISA. The 16-bit instructions did not output right as we did not know how to adjust the program counter as to when to take 16-bit instructions and differentiate 32-bit instructions. Our test files are called R_dump, I_dump, U_dump, B_dump, S_dump. They include all RV32I instructions. All instructions are in memory register names, for example x0,x1,x2… We discovered that we need to change all registers to ABI registers a few hours before submission and were unable to complete it on time.

#Contrubtion
We were only a group of 2, so the workload was a bit high for both of us. At the beginning, we split the 32-bit RISC-V instruction types between me and Hussein. I worked on the R, I and B types and Hussein worked on the rest of them. After we finished all the 32-bit instructions, I, Omar, was more focused on the 16-bit instruction implementation; while Hussein, on the other hand, was working on the test cases. We made a test case file for each one of the 32-bit instructions, so we can check the output of all 32-bit instructions. At the end, we were both working together on the debugging, run test cases, check correctness and output of the program. 
