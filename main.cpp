/*
	This is just a skeleton. It DOES NOT implement all the requirements.
	It only recognizes the RV32I "ADD", "SUB" and "ADDI" instructions only. 
	It prints "Unkown Instruction" for all other instructions!
	
	Usage example:
		$ rvcdiss t1.bin
	should print out:
		0x00000000	0x00100013	ADDI	x0, x0, 0x1
		0x00000004	0x00100093	ADDI	x1, x0, 0x1
		0x00000008	0x00100113	ADDI	x2, x0, 0x1
		0x0000000c	0x001001b3	ADD		x3, x0, x1
		0x00000010	0x00208233	ADD		x4, x1, x2
		0x00000014	0x004182b3	ADD		x5, x3, x4
		0x00000018	0x00100893	ADDI	x11, x0, 0x1
		0x0000001c	0x00028513	ADDI	xa, x5, 0x0
		0x00000020	0x00000073	Unkown Instruction 

	References:
	(1) The risc-v ISA Manual ver. 2.1 @ https://riscv.org/specifications/
	(2) https://github.com/michaeljclark/riscv-meta/blob/master/meta/opcodes
*/

#include <iostream>
#include <fstream>
#include "stdlib.h"
#include <iomanip>

using namespace std;

unsigned int pc = 0x0;

char memory[8*1024];	// only 8KB of memory located at address 0

void emitError(const char *s)
{
	cout << s;
	exit(0);
}

void printPrefix(unsigned int instA, unsigned int instW){
	cout << "0x" << hex << std::setfill('0') << std::setw(8) << instA << "\t0x" << std::setw(8) << instW;
}

void instDecExec(unsigned int instWord)
{
	unsigned int rd, rs1, rs2, funct3, funct7, funct2, funct6, opcode;
	unsigned int I_imm, S_imm, B_imm, U_imm, J_imm;
	unsigned int address;

	unsigned int instPC = pc - 4;

	opcode = instWord & 0x0000007F; //C OPCODE -> opcode = instWord & 0x2
	rd = (instWord >> 7) & 0x0000001F;
	funct3 = (instWord >> 12) & 0x00000007;
	rs1 = (instWord >> 15) & 0x0000001F;
	rs2 = (instWord >> 20) & 0x0000001F;
	funct7 = (instWord >> 25) & 0x0000007F;
	

	// â€” inst[31] â€” inst[30:25] inst[24:21] inst[20]
	I_imm = ((instWord >> 20) & 0x7FF) | (((instWord >> 31) ? 0xFFFFF800 : 0x0));

	printPrefix(instPC, instWord);

	if(opcode == 0x33){		// R Instructions
		switch(funct3){
			case 0: if(funct7 == 32) {
								cout << "\tSUB\tx" << rd << ", x" << rs1 << ", x" << rs2 << "\n";
							}
							else {
								cout << "\tADD\tx" << rd << ", x" << rs1 << ", x" << rs2 << "\n";
							}
							break;
				
			case 4: 				cout <<"\tXOR\tx" << rd << ", x" << rs1 << ", x" << rs2 << "\n";
							break;
			
			case 1:					cout <<"\tSLL\tx" << rd << ", x" << rs1 << ", x" << rs2 << "\n";
							break;
				
			case 2:					cout <<"\tSLT\tx" << rd << ", x" << rs1 << ", x" << rs2 << "\n";
							break;
				
			case 3:					cout <<"\tSLTU\tx" << rd << ", x" << rs1 << ", x" << rs2 << "\n";
							break;
				
			case 5: if(funct7 == 32) {
								cout <<"\tSRA\tx" << rd << ", x" << rs1 << ", x" << rs2 << "\n";
							}
							else {
								cout <<"\tSRL\tx" << rd << ", x" << rs1 << ", x" << rs2 << "\n";
							}
							break;
				
			case 6:					cout <<"\tOR\tx" << rd << ", x" << rs1 << ", x" << rs2 << "\n";
							break;
				
			case 7:					cout <<"\tAND\tx" << rd << ", x" << rs1 << ", x" << rs2 << "\n";
							break;
				
			default:
							cout << "\tUnkown R Instruction \n";
		}
	} 
	
	
	else if(opcode == 0x13){	// I instructions
		switch(funct3){
				
			/*if(I_imm>0x800)
			{
				I_imm = (I_imm xor 0xFFF)+1;
			   	I_imm =  -I_imm;
			}*/
				
			
			case 0:	cout << "\tADDI\tx" << rd << ", x" << rs1 << ", " << hex << "0x" << (int)I_imm << "\n";
					break;
				
			case 1:	I_imm = (instWord >> 25) & 0x0000007F; //imm[5:11]
				cout << "\tSLLI\tx" << rd << ", x" << rs1 << ", " << hex << "0x" << (int)I_imm << "\n";
					break;
				
			case 2:	cout << "\tSLTI\tx" << rd << ", x" << rs1 << ", " << hex << "0x" << (int)I_imm << "\n";
					break;
				
			case 3:	cout << "\tSLTIU\tx" << rd << ", x" << rs1 << ", " << hex << "0x" << (int)I_imm << "\n";
					break;
				
			case 4:	cout << "\tXORI\tx" << rd << ", x" << rs1 << ", " << hex << "0x" << (int)I_imm << "\n";
					break;
				
			case 5: I_imm = (instWord >> 25) & 0x0000007F; //imm[5:11]
				if (funct7 == 0)
				{
					cout << "\tSRLI\tx" << rd << ", x" << rs1 << ", " << hex << "0x" << (int)I_imm << "\n";
					break;
				}
				else
				{
					cout << "\tSRAI\tx" << rd << ", x" << rs1 << ", " << hex << "0x" << (int)I_imm << "\n";
					break;
				}
				
				
			case 6: cout << "\tORI\tx" << rd << ", x" << rs1 << ", " << hex << "0x" << (int)I_imm << "\n";
				break;
				
			case 7: cout << "\tANDI\tx" << rd << ", x" << rs1 << ", " << hex << "0x" << (int)I_imm << "\n";
				break
					
			default:
					cout << "\tUnkown I Instruction \n";
		}
	} 
	
	else if(opcode == 0x3) //Second I type
	{
		/*if(I_imm>0x800)
			{
				I_imm = (I_imm xor 0xFFF)+1;
			   	I_imm =  -I_imm;
			}*/
		
		switch (funct3){
		
			case 0: cout << "\tLB\tx" << rd << ", " << I_imm << " (x" << rs1 <<") "<< "\n";
				break;
				
			case 1: cout << "\tLH\tx" << rd << ", " << I_imm << " (x" << rs1 <<") "<< "\n";
				break;
				
			case 2: cout << "\tLW\tx" << rd << ", " << I_imm << " (x" << rs1 <<") "<< "\n";
				break;
				
			case 4: cout << "\tLBu\tx" << rd << ", " << I_imm << " (x" << rs1 <<") "<< "\n";
				break;
				
			case 5: cout << "\tLHu\tx" << rd << ", " << I_imm << " (x" << rs1 <<") "<< "\n";
				break;
				
			default:
					cout << "\tUnkown Second I Instruction \n";
		
		}
	}
	
	else if (opcode == 0x23) //S type
	{
		/*if(S_imm>0x800)
			{
				S_imm = (S_imm xor 0xFFF)+1;
			   	S_imm =  -S_imm;
			}*/
				
		switch (funct3)
		{
			case 0: cout << "\tSB\tx" << rs2 << ", " << S_imm << " (x" << rs1 <<") "<< "\n";
				break;
				
			case 1: cout << "\tSH\tx" << rs2 << ", " << S_imm << " (x" << rs1 <<") "<< "\n";
				break;
				
			case 2: cout << "\tSW\tx" << rs2 << ", " << S_imm << " (x" << rs1 <<") "<< "\n";
				break;
		}
	}
		
	else if (opcode == 0x63) //B type
	{
		int a = (instWord >> 8) & 0x0000000F;    //1:4
		int b = (instWord >> 25) & 0x0000003F;  // 5:10
		int c = (instWord >> 7) & 0x00000001;   //11
		int d = (instWord >> 31) & 0x00000001;   //12
		int e = d + c + b + a  //branch address
		
		switch (funct3)
		{
			case 0: cout << "\tBEQ\t"  << " x" << rs1 << ", x" << rs2 <<", "<< e <<"\n";
				break;
				
			case 1: cout << "\tBNE\t"  << " x" << rs1 << ", x" << rs2 <<", "<< e <<"\n";
				break;
				
			case 4: cout << "\tBLT\t"  << " x" << rs1 << ", x" << rs2 <<", "<< e <<"\n";
				break;
				
			case 5: cout << "\tBGE\t"  << " x" << rs1 << ", x" << rs2 <<", "<< e <<"\n";
				break;
				
			case 6: cout << "\tBLTu\t"  << " x" << rs1 << ", x" << rs2 <<", "<< e <<"\n";
				break;
				
			case 7: cout << "\tBGEu\t"  << " x" << rs1 << ", x" << rs2 <<", "<< e <<"\n";
				break;
			
			default:
				cout << "\tUnkown B Instruction \n";
		}
	}
		
	else if (opcode == 0x6F) //JAL
	{
		unsigned int a = (instWord >> 21) & 0x000003FF;    //1:10
           	unsigned int b = (instWord >> 20) & 0x0000001;     //11
            	unsigned int c = (instWord >> 12) & 0x000000FF;   //12:19
            	unsigned int d = (instWord >> 31) & 0x00000001;   //20
            	unsigned int e = d + c + b + a //branch address
		
		if (funct3 == 0)
			cout << "\tJALr\t" << "x" << rd << ", x" << rs1 << ", " << I_imm << "\n";
		else 
			cout << "\tJAL\t" << "x" << rd << ", " << e << "\n";
	}
		
	else if (opcode == 0x37){ // LUI
		int c = (instWord >> 12) & 0x0000FFFFF;
		cout << "\tLUI\t" << "x" << rd << ", " << c << "\n";
	}
	else if (opcode == 0x17){ // AUIPC
		int c = (instWord >> 12) & 0x0000FFFFF;
		cout << "\tAUIPC\t" << "x" << rd << ", " << c << "\n";
	}
	
	else if (opcode == 73){
		int c = (instWord >> 20) & 0x00000FFF;
		if(funct3 == 0x0)
		{
			if(funct7 == 0x0)
			{
				cout<<"\tEcall"<<"\n";
			}
		else {
			cout<<"\tEbreak"<<"\n";
		}
	}
		
		
	/*C INSTRUCTIONS
	C_opcode = instWord & 0x0000003;
	C_rd = (instWord >> 7) & 0x0000001F;
	C_funct3 = (instWord >> 12) & 0x00000007;
	C_rs1 = (instWord >> 15) & 0x0000001F;
	C_rs2 = (instWord >> 20) & 0x0000001F;
	C_funct7 = (instWord >> 25) & 0x0000007F;
	C_funct6 = (
		*/
		
		
	if ((opcode = instWord & 0x3 ) == 0x0) //C0 INST OPCODE 00 if (opcode = instWord & 0x3)
	{
		opcode = instWord & 0x0003; //C OPCODE -> opcode = instWord & 0x2
		rd = (instWord >> 2) & 0x0007;
		funct3 = (instWord >> 13) & 0x0007;
		rs1 = (instWord >> 7) & 0x0007;
		unsigned int a = (instWord >> 5) & 0x0001 //imm 6
		unsigned int b = (instWord >> 6) & 0x0001 //imm 2
		unsigned int c = (instWord >> 10) & 0x0003 //imm[3:5]
		unsigned int d = a + b + c;
		
	        if(d>0x800){
                d= (d xor 0x001F)+1;
                d =  -d;
		}
		
		if (funct3 == 0x2)
		{
			cout << "\tLW\tx" << rd << ", " << (int)d << " (x" << rs1 <<") "<< "\n";
		}
		
		else
		{
			rs2 = rd;
			cout << "\tSW\tx" << rs2 << ", " << d << " (x" << rs1 <<") "<< "\n";
		}
	
		
		//rs2 = (instWord >> 20) & 0x0000001F;
		//funct7 = (instWord >> 25) & 0x0000007F;
	}
		
	else if ((opcode = instWord & 0x3 ) == 0x1) //C1 INST OPCODE 01
	{
		
		opcode = instWord & 0x0003; //C OPCODE -> opcode = instWord & 0x2

		funct6 = (instWord >> 10) & 0x003F; // [10:15]
		funct3 = (instWord >> 13) & 0x0007; // [13:15]
		
		if (funct3 == 4){
		
			funct2 = (instWord >> 10) & 0003; // [10:11}
			rd = (instWord >> 7) & 0x0007; // [7:9]
			unsigned int C1_immA = (instWord >> 2) & 0x001F; // shamt [2:6]
			unsigned int C1_immB = (instWord >> 12);
				     C1_immB = 0;
			unsigned int C1_imm = C1_immA + C1_immB;
			
			if(C1_imm>0x800){
            		C1_imm = (C1_imm xor 0x001F)+1;
               		C1_imm =  -C1_imm;
			}
				
			switch (funct2){
				
				case 0: cout << "\tSRLI\tx" << rd << ", x" << rd << ", " << hex << "0x" << (int)C1_imm << "\n";
					break;
					
				case 1: cout << "\tSRAI\tx" << rd << ", x" << rd << ", " << hex << "0x" << (int)C1_imm << "\n";
					break;
					
				case 2: cout << "\tANDI\tx" << rd << ", x" << rd << ", " << hex << "0x" << (int)C1_imm << "\n";
					break;
					
				default:
					cout << "\tUnkown C1 Instruction \n";
			
			}
			
		
		}
		
		else if (funct6 == 35){
			
			rd = (instWord >> 7) & 0x0007; // [7:9]
			funct2 = (instWord >> 5) & 0x0003;	// [5:6]
			rs2 = (instWord >> 2) & 0x0007; // rs2 [2:4]
			
			switch (funct2){
					
				case 0: cout << "\SUB\tx" << rd << ", x" << rd <<", "<< rs2 <<"\n";  //C.SUB
					break;
				
				case 1: cout << "\XOR\tx" << rd << ", x" << rd <<", "<< rs2 <<"\n";  //C.XOR
					break;
					
				case 2: cout << "\OR\tx" << rd << ", x" << rd <<", "<< rs2 <<"\n";  //C.OR
					break;
					
				case 3: cout <<"\tOR\tx" << rd << ", x" << rd << ", x" << rs2 << "\n"; //C.AND
					break;
					
				default:
					cout << "\tUnkown C1 Instruction \n";
		
			}
		}
		
		
	}
		
	else if ((opcode = instWord & 0x3 ) == 0x2) //C2 INST OPCODE 10
	{
		
		
		
		
		
	}
	
	
	else {
		cout << "\tUnkown Instruction \n";
	}

}

int main(int argc,const char *argv[]){

	unsigned int instWord=0;
	ifstream inFile;
	ofstream outFile;

	if(argc<2) emitError("use: rvcdiss <machine_code_file_name>\n");

	inFile.open(argv[1], ios::in | ios::binary | ios::ate);

	if(inFile.is_open())
	{
		int fsize = inFile.tellg();

		inFile.seekg (0, inFile.beg);
		if(!inFile.read((const char *)memory, fsize)) emitError("Cannot read from input file\n");

		while(true){
				instWord = 	(unsigned char)memory[pc] |
							(((unsigned char)memory[pc+1])<<8) |
							(((unsigned char)memory[pc+2])<<16) |
							(((unsigned char)memory[pc+3])<<24);
				pc += 4;
				// remove the following line once you have a complete simulator
				if(pc==4) break;			// stop when PC reached address 32
				instDecExec(instWord);
		}
	} else emitError("Cannot access input file\n");
}
