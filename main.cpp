#include <iostream>
#include <fstream>
#include "stdlib.h"
#include <iomanip>

using namespace std;

unsigned int pc = 0x0;

unsigned char memory[8 * 1024];	// only 8KB of memory located at address 0

void emitError(const char* s)
{
	cout << s;
	exit(0);
}

void printPrefix(unsigned int instA, unsigned int instW) {
	cout << "0x" << hex << std::setfill('0') << std::setw(8) << instA << "\t0x" << std::setw(8) << instW;
}

/*char API(int reg) {

	char res = ' ';
	switch (reg) {

	case 0:  res= 'zero';
		break;

		return res;

	}

}*/

void instDecExec(unsigned int instWord)
{
	int rd, rs1, rs2, funct3, funct7, funct2, funct6, opcode;
	int I_imm, S_imm, B_imm, U_imm, J_imm;
	unsigned int address;
	funct6 = (instWord >> 10) & 0x003F; // [10:15]
	//unsigned int instPC;

	

	/*if (swt) {

		instPC = pc - 2;
	}

	else {

		instPC = pc - 4;
	}*/
	

	unsigned int instPC = pc - 4;

	opcode = instWord & 0x0000007F; //C OPCODE -> opcode = instWord & 0x2
	rd = (instWord >> 7) & 0x0000001F;
	funct3 = (instWord >> 12) & 0x00000007;
	rs1 = (instWord >> 15) & 0x0000001F;
	rs2 = (instWord >> 20) & 0x0000001F;
	funct7 = (instWord >> 25) & 0x0000007F;
	I_imm = (instWord >> 20) & 0x000000FFF;

	/*switch (rd) {

	case 0x0: rd = 0;
		break;


	default:
		"API Error";
		break;

	}*/


	// — inst[31] — inst[30:25] inst[24:21] inst[20]
	I_imm = ((instWord >> 20) & 0x7FF) | (((instWord >> 31) ? 0xFFFFF800 : 0x0));


	printPrefix(instPC, instWord);

	


	

	if (opcode == 0x33) {		// R Instructions
		

		switch (funct3) {

		case 0: {
			if (funct7 == 32) {
				cout << "\tSUB\tx" << rd << ", x" << rs1 << ", x" << rs2 << "\n";
			}
			else if (funct7 == 0x00) {
				cout << "\tADD\tx" << rd << ", x" << rs1 << ", x" << rs2 << "\n";
			}
		}
					break;

			case 4: cout << "\tXOR\tx" << rd << ", x" << rs1 << ", x" << rs2 << "\n";
					break;

			case 1:	cout << "\tSLI\tx" << rd << ", x" << rs1 << ", x" << rs2 << "\n";
					break;

			case 2:	cout << "\tSLT\tx" << rd << ", x" << rs1 << ", x" << rs2 << "\n";
					break;

			case 3:	cout << "\tSLTU\tx" << rd << ", x" << rs1 << ", x" << rs2 << "\n";
					break;

			case 5: { 
				if (funct7 == 32) {
				cout << "\tSRA\tx" << rd << ", x" << rs1 << ", x" << rs2 << "\n";
				}
				else {
				cout << "\tSRL\tx" << rd << ", x" << rs1 << ", x" << rs2 << "\n";
				}
			}
			  break;

			case 6:	cout << "\tOR\tx" << rd << ", x" << rs1 << ", x" << rs2 << "\n";
					break;

			case 7:	cout << "\tAND\tx" << rd << ", x" << rs1 << ", x" << rs2 << "\n";
					break;

			default:
					cout << "\tUnkown R Instruction \n";
		}
	}


	else if (opcode == 0x13) {	// I instructions

		switch (funct3) {

			
			I_imm = (instWord >> 20) & 0x00000FFF;


			if(I_imm>0x800)
			{
				I_imm = (I_imm xor 0xFFF)+1;
				I_imm =  -I_imm;
			}


			case 0:	cout << "\tADDI\tx" << rd << ", x" << rs1 << ", " << "0x" << I_imm << "\n";
					break;

			case 1: {
				//I_imm = (instWord >> 25) & 0x0000007F; //imm[5:11]
				cout << "\tSLLI\tx" << rd << ", x" << rs1 << ", " << "0x" << rs2 << "\n";
			}
				break;

			case 2:	cout << "\tSLTI\tx" << rd << ", x" << rs1 << ", " << "0x" << I_imm << "\n";
					break;

			case 3:	cout << "\tSLTIU\tx" << rd << ", x" << rs1 << ", " << "0x" << I_imm << "\n";
					break;

			case 4:	cout << "\tXORI\tx" << rd << ", x" << rs1 << ", " << "0x" << I_imm << "\n";
					break;

			case 5: {
				
				if (funct7 == 0)
				{
					cout << "\tSRLI\tx" << rd << ", x" << rs1 << ", " << "0x" << rs2<< "\n";
					break;
				}
				else
				{
					cout << "\tSRAI\tx" << rd << ", x" << rs1 << ", " << "0x" << rs2 << "\n";
					break;
				}
			}

			case 6: cout << "\tORI\tx" << rd << ", x" << rs1 << ", " << "0x" << I_imm << "\n";
					break;

			case 7: cout << "\tANDI\tx" << rd << ", x" << rs1 << ", " << "0x" << I_imm << "\n";
					break;

			default:
				cout << "\tUnkown I Instruction \n";
		}
	}

	else if (opcode == 0x3) //Second I type
	{
		
		I_imm = (instWord >> 20) & 0x0000007F;
	
		if(I_imm>0x800)
			{
				I_imm = (I_imm xor 0xFFF)+1;
				I_imm =  -I_imm;
			}

		switch (funct3) {

			case 0: cout << "\tLB\tx" << rd << ", " << I_imm << " (x" << rs1 << ") " << "\n";
					break;

			case 1: cout << "\tLH\tx" << rd << ", " << I_imm << " (x" << rs1 << ") " << "\n";
					break;

			case 2: cout << "\tLW\tx" << rd << ", " << I_imm << " (x" << rs1 << ") " << "\n";
					break;

			case 4: cout << "\tLBu\tx" << rd << ", " << I_imm << " (x" << rs1 << ") " << "\n";
					break;

			case 5: cout << "\tLHu\tx" << rd << ", " << I_imm << " (x" << rs1 << ") " << "\n";
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

		//unsigned int a = (instWord >> 7) & 0x0000001F;    //0:4
		//unsigned int b = (instWord >> 25) & 0x0000007F;    //5:11
		int c = rd | funct7;    //concatinate

		if (c > 0x800) {
			c = (c xor 0xFFF) + 1;
			c = - c;
		}

		switch (funct3)
		{
			case 0: cout << "\tSB\tx" << rs2 << ", " << c << " (x" << rs1 << ") " << "\n";
					break;

			case 1: cout << "\tSH\tx" << rs2 << ", " << c << " (x" << rs1 << ") " << "\n";
					break;

			case 2: cout << "\tSW\tx" << rs2 << ", " << c << " (x" << rs1 << ") " << "\n";
					break;

			default:
				cout << "\tUnkown S Instruction \n";
		}
	}

	else if (opcode == 0x63) //B type
	{
		unsigned int a = (instWord >> 7) & 0x0000000F;    //1:4
		unsigned int b = (instWord >> 25) & 0x0000003F;  // 5:10
		unsigned int c = (instWord >> 7) & 0x00000001;   //11
		unsigned int d = (instWord >> 31) & 0x00000001;   //12
		int e = d | c | b | a;  //branch address

		if (e > 0x800) {
			e = (e xor 0xFFF) + 1;
			e = - e;
		}


		switch (funct3)
		{
			case 0: cout << "\tBEQ\t" << " x" << rs1 << ", x" << rs2 << ", " << e << "\n";
					break;

			case 1: cout << "\tBNE\t" << " x" << rs1 << ", x" << rs2 << ", " << e << "\n";
					break;

			case 4: cout << "\tBLT\t" << " x" << rs1 << ", x" << rs2 << ", " << e << "\n";
					break;

			case 5: cout << "\tBGE\t" << " x" << rs1 << ", x" << rs2 << ", " << e << "\n";
					break;

			case 6: cout << "\tBLTu\t" << " x" << rs1 << ", x" << rs2 << ", " << e << "\n";
					break;

			case 7: cout << "\tBGEu\t" << " x" << rs1 << ", x" << rs2 << ", " << e << "\n";
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
		int e = d | c | b | a; //branch address

		if (e > 0x800) {
			e = (e xor 0xFFF) + 1;
			e = - e;
		}
			cout << "\tJAL\t" << "x" << rd << ", " << e << "\n";
	}

	else if (opcode == 0x67) //JALR
	{

		if (funct3 == 0) {
			I_imm = (instWord >> 20) & 0x00000FFF;
		
			if (I_imm > 0x800) {
				I_imm = (I_imm xor 0xFFF) + 1;
				I_imm = -I_imm;
			}
			cout << "\tJALR\t" << "x" << rd << ", x" << rs1 << ", " << I_imm << "\n";
		}
	else 
		cout << "\tUnkown I Instruction \n"; 
		
	}

	else if (opcode == 0x37) { // LUI
		int c = (instWord >> 12) & 0x0000FFFFF;

		if (c > 0x800) {
			c= (c xor 0xFFF) + 1;
			c = -c;
		}
		cout << "\tLUI\t" << "x" << rd << ", " << c << "\n";
	}

	else if (opcode == 0x17) { // AUIPC
		int c = (instWord >> 12) & 0x0000FFFFF;
		if (c > 0x800) {
			c = (c xor 0xFFF) + 1;
			c = -c;
		}
		cout << "\tAUIPC\t" << "x" << rd << ", " << c << "\n";
	}

	else if (opcode == 0x73) {
		int c = (instWord >> 20) & 0x00000FFF;

		if (c > 0x800) {
			c = (c xor 0xFFF) + 1;
			c = -c;
		}

		if (funct3 == 0x0)
		{
			if (funct7 == 0x0)
			{
				cout << "\tEcall" << "\n";
			}

			else {
				cout << "\tEbreak" << "\n";
			}
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

	else if ((opcode & 0x3) == 0x0) //C0 INST OPCODE 00 if (opcode = instWord & 0x3)
	{
			opcode = instWord & 0x0003; //C OPCODE -> opcode = instWord & 0x2
			rd = (instWord >> 2) & 0x0007;
			funct3 = (instWord >> 13) & 0x0007;
			rs1 = (instWord >> 7) & 0x0007;
			unsigned int a = (instWord >> 5) & 0x0001; //imm 6
			unsigned int b = (instWord >> 6) & 0x0001; //imm 2
			unsigned int c = (instWord >> 10) & 0x0003; //imm[3:5]
			int d = a | b | c;

			if (d > 0x800) {
				d = (d xor 0x001F) + 1;
				d = -d;
			}

			if (funct3 == 0x2)
			{
				cout << "\tC.LW\tx" << rd << ", " << (int)d << " (x" << rs1 << ") " << "\n";
			}

			else
			{
				//rs2 = rd;
				cout << "\tC.SW\tx" << rs1 << ", " << (int)d << " (x" << rs2 << ") " << "\n";
			}


			//rs2 = (instWord >> 20) & 0x0000001F;
			//funct7 = (instWord >> 25) & 0x0000007F;
	}

	else if ((opcode & 0x3) == 0x1) //C1 INST OPCODE 01
	{

		opcode = instWord & 0x0003; //C OPCODE -> opcode = instWord & 0x2

		funct6 = (instWord >> 10) & 0x003F; // [10:15]
		funct3 = (instWord >> 13) & 0x0007; // [13:15]

		if (funct3 == 4) {

			funct2 = (instWord >> 10) & 0003; // [10:11}
			rd = (instWord >> 7) & 0x0007; // [7:9]
			unsigned int C1_immA = (instWord >> 2) & 0x001F; // shamt [2:6]
			unsigned int C1_immB = (instWord >> 12);
			C1_immB = 0;
			int C1_imm = C1_immA | C1_immB;

			if (C1_imm > 0x800) {
				C1_imm = (C1_imm xor 0x001F) + 1;
				C1_imm = -C1_imm;
			}

			switch (funct2) {

			case 0: cout << "\tC.SRLI\tx" << rd << ", x" << rd << ", " << hex << "0x" << C1_imm << "\n";
				break;

			case 1: cout << "\tC.SRAI\tx" << rd << ", x" << rd << ", " << hex << "0x" << C1_imm << "\n";
				break;

			case 2: cout << "\tC.ANDI\tx" << rd << ", x" << rd << ", " << hex << "0x" << C1_imm << "\n";
				break;

			default:
				cout << "\tUnkown C1 Instruction \n";

			}
		}

		else if (funct3 == 0) {

			rd = (instWord >> 7) & 0x0001F; // [7:11]
			unsigned int C1_immA = (instWord >> 2) & 0x001F; // shamt [2:6]
			unsigned int C1_immB = (instWord >> 12) & 0x0001;
			//C1_immB = 0;
			int C1_imm = C1_immA | C1_immB;

			if (C1_imm > 0x800) {
				C1_imm = (C1_imm xor 0x001F) + 1;
				C1_imm = -C1_imm;
			}


			cout << "\tC.ADDI\tx" << rd << ", x" << rd << ", " << "0x" << (int)C1_imm << "\n";
		}


		else if (funct3 == 1) {
			unsigned int a1 = (instWord >> 2) & 0x0001; //5
			unsigned int b1 = (instWord >> 3) & 0x0007; //[1:3]
			unsigned int c1 = (instWord >> 6) & 0x0001; //7
			unsigned int d1 = (instWord >> 7) & 0x0001; //6
			unsigned int e1 = (instWord >> 8) & 0x0001; //10
			unsigned int f1 = (instWord >> 9) & 0x0003; //[8:9]
			unsigned int g1 = (instWord >> 10) & 0x0001; //4
			unsigned int i1 = (instWord >> 11) & 0x0001; //10
			unsigned int C1_imm = a1 | b1 | c1 | d1 | e1 | f1 | g1 | i1;
			cout << "\tC.JAL\t" << "x" << rd << ", " << C1_imm << "\n";
		}

		else if (funct3 == 2) {
			unsigned int a = (instWord >> 2) & 0x0001F; //[0:4]
			rd = (instWord >> 7) & 0x0001F; //rd
			unsigned int b = (instWord >> 12) & 0x0001; //5
			unsigned int C1_imm = a | b;
			cout << "\tC.LI\t" << "x" << rd << ", " << C1_imm << "\n";
		}

		else if (funct3 == 3) {
			unsigned int a = (instWord >> 2) & 0x0001F; //[12:16]
			rd = (instWord >> 7) & 0x0001F; //rd
			unsigned int b = (instWord >> 12) & 0x0001; //17
			unsigned int C1_imm = a | b;
			cout << "\tC.LUI\t" << "x" << rd << ", " << C1_imm << "\n";
		}

		else if (funct6 == 35) {

			rd = (instWord >> 7) & 0x0007; // [7:9]
			funct2 = (instWord >> 5) & 0x0003;	// [5:6]
			rs2 = (instWord >> 2) & 0x0007; // rs2 [2:4]

			switch (funct2) {

			case 0: cout << "\C.SUB\tx" << rd << ", x" << rd << ", " << rs2 << "\n";  //C.SUB
				break;

			case 1: cout << "\C.XOR\tx" << rd << ", x" << rd << ", " << rs2 << "\n";  //C.XOR
				break;

			case 2: cout << "\C.OR\tx" << rd << ", x" << rd << ", " << rs2 << "\n";  //C.OR
				break;

			case 3: cout << "\tC.AND\tx" << rd << ", x" << rd << ", x" << rs2 << "\n"; //C.AND
				break;

			default:
				cout << "\tUnkown C1 Instruction \n";

			}
		}

	}

		

	else if ((opcode & 0x3) == 0x2) //C2 INST OPCODE 10
	{


			opcode = instWord & 0x0003; //C OPCODE -> opcode = instWord & 0x2
			//rd = (instWord >> 2) & 0x0007;
			int funct3 = (instWord >> 12) & 0x000F;
			rd = (instWord >> 7) & 0x0001F;
			rs2 = (instWord >> 2) & 0x0000001F;
			/*int a = (instWord >> 5) & 0x0001 //imm 6
			int b = (instWord >> 6) & 0x0001 //imm 2
			int c = (instWord >> 10) & 0x0003 //imm[5:3]
			int d = a + b + c;*/
			int shamt2 = (instWord >> 12) & 0x0001;

			//funct7 = (instWord >> 25) & 0x0000007F;

			if (funct3 == 0x0) {
				cout << "\tC.ADD\tx" << rd << ", x" << rd << ", x" << rs2 << "\n";
			}
			else if (funct3 == 0x1)
			{
				//I_imm = (instWord >> 25) & 0x0000007F; //imm[5:11]
				unsigned int shamt = (instWord >> 2) & 0x1F; // shift amount [4:0]
				unsigned int x = shamt | shamt2;
				cout << "\tC.SLLI\tx" << rd << ", x" << rd << ", " << "0x" << (int)x << "\n";
			}

			else
				cout << "\tUnkown C2 Instruction \n";

	}

	
	switch (rd) {

	case 0x0: rd == 0;
		break;


	default :
		"API Error";
		break;

	}

		
}



int main(int argc, const char* argv[]) 
{
		bool swt = false;
		unsigned int instWord = 0;
		ifstream inFile;
		ofstream outFile;
		unsigned int pc = 0;

		if (argc < 2) emitError("use: rvcdiss <machine_code_file_name>\n");

		inFile.open(argv[1], ios::in | ios::binary | ios::ate);
		std::cout << "file name: " << argv[1] << "\n";


		//inFile.open("t1.bin", ios::in | ios::binary | ios::ate);

		if (inFile.is_open())
		{

			int fsize = inFile.tellg();

			inFile.seekg(0, inFile.beg);

			if (!inFile.read((char*)memory, fsize))

				emitError("Cannot read from input file\n");

			while (true) {
				instWord = (unsigned char)memory[pc] |
					(((unsigned char)memory[pc + 1]) << 8) |
					(((unsigned char)memory[pc + 2]) << 16) |
					(((unsigned char)memory[pc + 3]) << 24);

				/*unsigned int check = instWord & 0x00000003;

				if (check == 0x00 | 0x01 | 0x02) {

					pc += 2;
					swt = true;

					// remove the following line once you have a complete simulator
					if (pc == fsize + 2)
						break;			// stop when PC reached address 32
					instDecExec(instWord, swt);
				}

				else {*/

					pc += 4;
					// remove the following line once you have a complete simulator
					if (pc == fsize + 4)
						break;			// stop when PC reached address 32
					instDecExec(instWord);

				}
			
		}
		else 
			emitError("Cannot access input file\n");

		//return 0;
	}
