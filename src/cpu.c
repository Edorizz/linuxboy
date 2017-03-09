#include "../include/linuxboy.h"

#include <stdio.h>
#include <string.h>
#include <errno.h>

const op ops[0x100] = { { "NOP", 1, nop },
			{ "LD BC,d16", 3, ld_bc_d16 },
			{ "LD (BC),A", 1, NULL },
			{ "INC BC", 1, NULL },
			{ "INC B", 1, NULL },
			{ "DEC B", 1, NULL },
			{ "LD B,d8", 2, NULL },
			{ "RLCA", 1, NULL },
			{ "LD (a16),SP", 3, NULL },
			{ "ADD HL,BC", 1, NULL },
			{ "LD A,(BC)", 1, NULL },
			{ "DEC BC", 1, NULL },
			{ "INC C", 1, NULL },
			{ "DEC C", 1, NULL },
			{ "LD C,d8", 2, NULL },
			{ "RRCA", 1, NULL },
			
			{ "STOP 0", 2, NULL },
			{ "LD DE,d16", 3, NULL },
			{ "LD (DE),A", 1, NULL },
			{ "INC DE", 1, NULL },
			{ "IND D", 1, NULL },
			{ "DEC D", 1, NULL },
			{ "LD D,d8", 2, NULL },
			{ "RLA", 1, NULL },
			{ "JR r8", 2, NULL },
			{ "ADD HL,DE", 1, NULL },
			{ "LD A,(DE)", 1, NULL },
			{ "DEC DE", 1, NULL },
			{ "INC E", 1, NULL },
			{ "DEC E", 1, NULL },
			{ "LD E,d8", 2, NULL },
			{ "RRA", 1, NULL },
			
			{ "JR NZ,r8", 2, NULL },
			{ "LD HL,d16", 3, NULL },
			{ "LD (HL+),A", 1, NULL },
			{ "INC HL", 1, NULL },
			{ "INC H", 1, NULL },
			{ "DEC H", 1, NULL },
			{ "LD H,d8", 2, NULL },
			{ "DAA", 1, NULL },
			{ "JR Z,r8", 2, NULL },
			{ "ADD HL,HL", 1, NULL },
			{ "LD A,(HL+)", 1, NULL },
			{ "DEC HL", 1, NULL },
			{ "INC L", 1, NULL },
			{ "DEC L", 1, NULL },
			{ "LD L,d8", 2, NULL },
			{ "CPL", 1, NULL },
			
			{ "JR NC,r8", 2, NULL },
			{ "LD SP,d16", 3, NULL },
			{ "LD (HL+),A", 1, NULL },
			{ "INC SP", 1, NULL },
			{ "INC (HL)", 1, NULL },
			{ "DEC (HL)", 1, NULL },
			{ "LD (HL),d8", 2, NULL },
			{ "SCF", 1, NULL },
			{ "JR C,r8", 2, NULL },
			{ "ADD HL,SP", 1, NULL },
			{ "LD A,(HL-)", 1, NULL },
			{ "DEC SP", 1, NULL },
			{ "INC A", 1, NULL },
			{ "DEC A", 1, NULL },
			{ "LD A,d8", 2, NULL },
			{ "CCF", 1, NULL },
			
			{ "LD B,B", 1, NULL },
			{ "LD B,C", 1, NULL },
			{ "LD B,D", 1, NULL },
			{ "LD B,E", 1, NULL },
			{ "LD B,H", 1, NULL },
			{ "LD B,L", 1, NULL },
			{ "LD B,(HL)", 1, NULL },
			{ "LD B,A", 1, NULL },
			{ "LD C,B", 1, NULL },
			{ "LD C,C", 1, NULL },
			{ "LD C,D", 1, NULL },
			{ "LD C,E", 1, NULL },
			{ "LD C,H", 1, NULL },
			{ "LD C,L", 1, NULL },
			{ "LD C,(HL)", 1, NULL },
			{ "LD C,A", 1, NULL },
			
			{ "LD D,B", 1, NULL },
			{ "LD D,C", 1, NULL },
			{ "LD D,D", 1, NULL },
			{ "LD D,E", 1, NULL },
			{ "LD D,H", 1, NULL },
			{ "LD D,L", 1, NULL },
			{ "LD D,(HL)", 1, NULL },
			{ "LD D,A", 1, NULL },
			{ "LD E,B", 1, NULL },
			{ "LD E,C", 1, NULL },
			{ "LD E,D", 1, NULL },
			{ "LD E,E", 1, NULL },
			{ "LD E,H", 1, NULL },
			{ "LD E,L", 1, NULL },
			{ "LD E,(HL)", 1, NULL },
			{ "LD E,A", 1, NULL },
			
			{ "LD H,B", 1, NULL },
			{ "LD H,C", 1, NULL },
			{ "LD H,D", 1, NULL },
			{ "LD H,E", 1, NULL },
			{ "LD H,H", 1, NULL },
			{ "LD H,L", 1, NULL },
			{ "LD H,(HL)", 1, NULL },
			{ "LD H,A", 1, NULL },
			{ "LD L,B", 1, NULL },
			{ "LD L,C", 1, NULL },
			{ "LD L,D", 1, NULL },
			{ "LD L,E", 1, NULL },
			{ "LD L,H", 1, NULL },
			{ "LD L,L", 1, NULL },
			{ "LD L,(HL)", 1, NULL },
			{ "LD L,A", 1, NULL },
			
			{ "LD (HL),B", 1, NULL },
			{ "LD (HL),C", 1, NULL },
			{ "LD (HL),D", 1, NULL },
			{ "LD (HL),E", 1, NULL },
			{ "LD (HL),H", 1, NULL },
			{ "LD (HL),L", 1, NULL },
			{ "HALT", 1, NULL },
			{ "LD (HL),A", 1, NULL },
			{ "LD A,B", 1, NULL },
			{ "LD A,C", 1, NULL },
			{ "LD A,D", 1, NULL },
			{ "LD A,E", 1, NULL },
			{ "LD A,H", 1, NULL },
			{ "LD A,L", 1, NULL },
			{ "LD A,(HL)", 1, NULL },
			{ "LD A,A", 1, NULL },
			
			{ "ADD A,B", 1, NULL },
			{ "ADD A,C", 1, NULL },
			{ "ADD A,D", 1, NULL },
			{ "ADD A,E", 1, NULL },
			{ "ADD A,H", 1, NULL },
			{ "ADD A,L", 1, NULL },
			{ "ADD A,(HL)", 1, NULL },
			{ "ADD A,A", 1, NULL },
			{ "ADC A,B", 1, NULL },
			{ "ADC A,C", 1, NULL },
			{ "ADC A,D", 1, NULL },
			{ "ADC A,E", 1, NULL },
			{ "ADC A,H", 1, NULL },
			{ "ADC A,L", 1, NULL },
			{ "ADC A,(HL)", 1, NULL },
			{ "ADC A,A", 1, NULL },
			
			{ "SUB B", 1, NULL },
			{ "SUB C", 1, NULL },
			{ "SUB D", 1, NULL },
			{ "SUB E", 1, NULL },
			{ "SUB H", 1, NULL },
			{ "SUB L", 1, NULL },
			{ "SUB (HL)", 1, NULL },
			{ "SUB A", 1, NULL },
			{ "SBC A,B", 1, NULL },
			{ "SBC A,C", 1, NULL },
			{ "SBC A,D", 1, NULL },
			{ "SBC A,E", 1, NULL },
			{ "SBC A,H", 1, NULL },
			{ "SBC A,L", 1, NULL },
			{ "SBC A,(HL)", 1, NULL },
			{ "SBC A,A", 1, NULL },
			
			{ "AND B", 1, NULL },
			{ "AND C", 1, NULL },
			{ "AND D", 1, NULL },
			{ "AND E", 1, NULL },
			{ "AND H", 1, NULL },
			{ "AND L", 1, NULL },
			{ "AND (HL)", 1, NULL },
			{ "AND A", 1, NULL },
			{ "XOR B", 1, NULL },
			{ "XOR C", 1, NULL },
			{ "XOR D", 1, NULL },
			{ "XOR E", 1, NULL },
			{ "XOR H", 1, NULL },
			{ "XOR L", 1, NULL },
			{ "XOR (HL)", 1, NULL },
			{ "XOR A", 1, NULL },
			
			{ "OR B", 1, NULL },
			{ "OR C", 1, NULL },
			{ "OR D", 1, NULL },
			{ "OR E", 1, NULL },
			{ "OR H", 1, NULL },
			{ "OR L", 1, NULL },
			{ "OR (HL)", 1, NULL },
			{ "OR A", 1, NULL },
			{ "CP B", 1, NULL },
			{ "CP C", 1, NULL },
			{ "CP D", 1, NULL },
			{ "CP E", 1, NULL },
			{ "CP H", 1, NULL },
			{ "CP L", 1, NULL },
			{ "CP (HL)", 1, NULL },
			{ "CP A", 1, NULL },
			
			{ "RET NZ", 1, NULL },
			{ "POP BC", 1, NULL },
			{ "JP NZ,a16", 3, NULL },
			{ "JP a16", 3, NULL },
			{ "CALL NZ,a16", 3, NULL },
			{ "PUSH BC", 1, NULL },
			{ "ADD A,d8", 2, NULL },
			{ "RST 00H", 1, NULL },
			{ "RET Z", 1, NULL },
			{ "RET", 1, NULL },
			{ "JP Z, a16", 3, NULL },
			{ "PREFIX CB", 1, NULL },
			{ "CALL Z,a16", 3, NULL },
			{ "CALL a16", 3, NULL },
			{ "ADC A,d8", 2, NULL },
			{ "RST 08H", 1, NULL },
			
			{ "RET NC", 1, NULL },
			{ "POP DE", 1, NULL },
			{ "JP NC,a16", 3, NULL },
			{ "--", 1, NULL },
			{ "CALL NC,a16", 3, NULL },
			{ "PUSH DE", 1, NULL },
			{ "SUB d8", 2, NULL },
			{ "RST 10H", 1, NULL },
			{ "RET C", 1, NULL },
			{ "RETI", 1, NULL },
			{ "JP C, a16", 3, NULL },
			{ "--", 1, NULL },
			{ "CALL C,a16", 3, NULL },
			{ "--", 1, NULL },
			{ "SBC A,d8", 2, NULL },
			{ "RST 18H", 1, NULL },
			
			{ "LDH (a8),A", 2, NULL },
			{ "POP HL", 1, NULL },
			{ "LD (C),A", 2, NULL },
			{ "--", 1, NULL },
			{ "--", 1, NULL },
			{ "PUSH HL", 1, NULL },
			{ "AND d8", 2, NULL },
			{ "RST 20H", 1, NULL },
			{ "ADD SP,r8", 2, NULL },
			{ "JP (HL)", 1, NULL },
			{ "LD (a16),A", 3, NULL },
			{ "--", 1, NULL },
			{ "--", 1, NULL },
			{ "--", 1, NULL },
			{ "XOR d8", 2, NULL },
			{ "RST 28H", 1, NULL },
			
			{ "LDH A,(a8)", 2, NULL },
			{ "POP AF", 1, NULL },
			{ "LD A,(C)", 2, NULL },
			{ "DI", 1, NULL },
			{ "--", 1, NULL },
			{ "PUSH AF", 1, NULL },
			{ "OR d8", 2, NULL },
			{ "RST 30H", 1, NULL },
			{ "LD HL,SP+r8", 2, NULL },
			{ "LD SP,HL", 1, NULL },
			{ "LD A,(a16)", 3, NULL },
			{ "EI", 1, NULL },
			{ "--", 1, NULL },
			{ "--", 1, NULL },
			{ "CP d8", 2, NULL },
			{ "RST 38H", 1, NULL } };

const op ext_ops[0x100] = { { "RLC B", 2, NULL },
			    { "RLC C", 2, NULL },
			    { "RLC D", 2, NULL },
			    { "RLC E", 2, NULL },
			    { "RLC H", 2, NULL },
			    { "RLC L", 2, NULL },
			    { "RLC (HL)", 2, NULL },
			    { "RLC A", 2, NULL },
			    { "RRC B", 2, NULL },
			    { "RRC C", 2, NULL },
			    { "RRC D", 2, NULL },
			    { "RRC E", 2, NULL },
			    { "RRC H", 2, NULL },
			    { "RRC L", 2, NULL },
			    { "RRC (HL)", 2, NULL },
			    { "RRC A", 2, NULL },

			    { "RL B", 2, NULL },
			    { "RL C", 2, NULL },
			    { "RL D", 2, NULL },
			    { "RL E", 2, NULL },
			    { "RL H", 2, NULL },
			    { "RL L", 2, NULL },
			    { "RL (HL)", 2, NULL },
			    { "RL A", 2, NULL },
			    { "RR B", 2, NULL },
			    { "RR C", 2, NULL },
			    { "RR D", 2, NULL },
			    { "RR E", 2, NULL },
			    { "RR H", 2, NULL },
			    { "RR L", 2, NULL },
			    { "RR (HL)", 2, NULL },
			    { "RR A", 2, NULL },

			    { "SLA B", 2, NULL },
			    { "SLA C", 2, NULL },
			    { "SLA D", 2, NULL },
			    { "SLA E", 2, NULL },
			    { "SLA H", 2, NULL },
			    { "SLA L", 2, NULL },
			    { "SLA (HL)", 2, NULL },
			    { "SLA A", 2, NULL },
			    { "SRA B", 2, NULL },
			    { "SRA C", 2, NULL },
			    { "SRA D", 2, NULL },
			    { "SRA E", 2, NULL },
			    { "SRA H", 2, NULL },
			    { "SRA L", 2, NULL },
			    { "SRA (HL)", 2, NULL },
			    { "SRA A", 2, NULL },

			    { "SWAP B", 2, NULL },
			    { "SWAP C", 2, NULL },
			    { "SWAP D", 2, NULL },
			    { "SWAP E", 2, NULL },
			    { "SWAP H", 2, NULL },
			    { "SWAP L", 2, NULL },
			    { "SWAP (HL)", 2, NULL },
			    { "SWAP A", 2, NULL },
			    { "SRL B", 2, NULL },
			    { "SRL C", 2, NULL },
			    { "SRL D", 2, NULL },
			    { "SRL E", 2, NULL },
			    { "SRL H", 2, NULL },
			    { "SRL L", 2, NULL },
			    { "SRL (HL)", 2, NULL },
			    { "SRL A", 2, NULL },

			    { "BIT 0,B", 2, NULL },
			    { "BIT 0,C", 2, NULL },
			    { "BIT 0,D", 2, NULL },
			    { "BIT 0,E", 2, NULL },
			    { "BIT 0,H", 2, NULL },
			    { "BIT 0,L", 2, NULL },
			    { "BIT 0,(HL)", 2, NULL },
			    { "BIT 0,A", 2, NULL },
			    { "BIT 1,B", 2, NULL },
			    { "BIT 1,C", 2, NULL },
			    { "BIT 1,D", 2, NULL },
			    { "BIT 1,E", 2, NULL },
			    { "BIT 1,H", 2, NULL },
			    { "BIT 1,L", 2, NULL },
			    { "BIT 1,(HL)", 2, NULL },
			    { "BIT 1,A", 2, NULL },

			    { "BIT 2,B", 2, NULL },
			    { "BIT 2,C", 2, NULL },
			    { "BIT 2,D", 2, NULL },
			    { "BIT 2,E", 2, NULL },
			    { "BIT 2,H", 2, NULL },
			    { "BIT 2,L", 2, NULL },
			    { "BIT 2,(HL)", 2, NULL },
			    { "BIT 2,A", 2, NULL },
			    { "BIT 3,B", 2, NULL },
			    { "BIT 3,C", 2, NULL },
			    { "BIT 3,D", 2, NULL },
			    { "BIT 3,E", 2, NULL },
			    { "BIT 3,H", 2, NULL },
			    { "BIT 3,L", 2, NULL },
			    { "BIT 3,(HL)", 2, NULL },
			    { "BIT 3,A", 2, NULL },

			    { "BIT 4,B", 2, NULL },
			    { "BIT 4,C", 2, NULL },
			    { "BIT 4,D", 2, NULL },
			    { "BIT 4,E", 2, NULL },
			    { "BIT 4,H", 2, NULL },
			    { "BIT 4,L", 2, NULL },
			    { "BIT 4,(HL)", 2, NULL },
			    { "BIT 4,A", 2, NULL },
			    { "BIT 5,B", 2, NULL },
			    { "BIT 5,C", 2, NULL },
			    { "BIT 5,D", 2, NULL },
			    { "BIT 5,E", 2, NULL },
			    { "BIT 5,H", 2, NULL },
			    { "BIT 5,L", 2, NULL },
			    { "BIT 5,(HL)", 2, NULL },
			    { "BIT 5,A", 2, NULL },

			    { "BIT 6,B", 2, NULL },
			    { "BIT 6,C", 2, NULL },
			    { "BIT 6,D", 2, NULL },
			    { "BIT 6,E", 2, NULL },
			    { "BIT 6,H", 2, NULL },
			    { "BIT 6,L", 2, NULL },
			    { "BIT 6,(HL)", 2, NULL },
			    { "BIT 6,A", 2, NULL },
			    { "BIT 7,B", 2, NULL },
			    { "BIT 7,C", 2, NULL },
			    { "BIT 7,D", 2, NULL },
			    { "BIT 7,E", 2, NULL },
			    { "BIT 7,H", 2, NULL },
			    { "BIT 7,L", 2, NULL },
			    { "BIT 7,(HL)", 2, NULL },
			    { "BIT 7,A", 2, NULL },
			    
			    { "RES 0,B", 2, NULL },
			    { "RES 0,C", 2, NULL },
			    { "RES 0,D", 2, NULL },
			    { "RES 0,E", 2, NULL },
			    { "RES 0,H", 2, NULL },
			    { "RES 0,L", 2, NULL },
			    { "RES 0,(HL)", 2, NULL },
			    { "RES 0,A", 2, NULL },
			    { "RES 1,B", 2, NULL },
			    { "RES 1,C", 2, NULL },
			    { "RES 1,D", 2, NULL },
			    { "RES 1,E", 2, NULL },
			    { "RES 1,H", 2, NULL },
			    { "RES 1,L", 2, NULL },
			    { "RES 1,(HL)", 2, NULL },
			    { "RES 1,A", 2, NULL },
			    
			    { "RES 2,B", 2, NULL },
			    { "RES 2,C", 2, NULL },
			    { "RES 2,D", 2, NULL },
			    { "RES 2,E", 2, NULL },
			    { "RES 2,H", 2, NULL },
			    { "RES 2,L", 2, NULL },
			    { "RES 2,(HL)", 2, NULL },
			    { "RES 2,A", 2, NULL },
			    { "RES 3,B", 2, NULL },
			    { "RES 3,C", 2, NULL },
			    { "RES 3,D", 2, NULL },
			    { "RES 3,E", 2, NULL },
			    { "RES 3,H", 2, NULL },
			    { "RES 3,L", 2, NULL },
			    { "RES 3,(HL)", 2, NULL },
			    { "RES 3,A", 2, NULL },

			    { "RES 4,B", 2, NULL },
			    { "RES 4,C", 2, NULL },
			    { "RES 4,D", 2, NULL },
			    { "RES 4,E", 2, NULL },
			    { "RES 4,H", 2, NULL },
			    { "RES 4,L", 2, NULL },
			    { "RES 4,(HL)", 2, NULL },
			    { "RES 4,A", 2, NULL },
			    { "RES 5,B", 2, NULL },
			    { "RES 5,C", 2, NULL },
			    { "RES 5,D", 2, NULL },
			    { "RES 5,E", 2, NULL },
			    { "RES 5,H", 2, NULL },
			    { "RES 5,L", 2, NULL },
			    { "RES 5,(HL)", 2, NULL },
			    { "RES 5,A", 2, NULL },

			    { "RES 6,B", 2, NULL },
			    { "RES 6,C", 2, NULL },
			    { "RES 6,D", 2, NULL },
			    { "RES 6,E", 2, NULL },
			    { "RES 6,H", 2, NULL },
			    { "RES 6,L", 2, NULL },
			    { "RES 6,(HL)", 2, NULL },
			    { "RES 6,A", 2, NULL },
			    { "RES 7,B", 2, NULL },
			    { "RES 7,C", 2, NULL },
			    { "RES 7,D", 2, NULL },
			    { "RES 7,E", 2, NULL },
			    { "RES 7,H", 2, NULL },
			    { "RES 7,L", 2, NULL },
			    { "RES 7,(HL)", 2, NULL },
			    { "RES 7,A", 2, NULL },

			    { "SET 0,B", 2, NULL },
			    { "SET 0,C", 2, NULL },
			    { "SET 0,D", 2, NULL },
			    { "SET 0,E", 2, NULL },
			    { "SET 0,H", 2, NULL },
			    { "SET 0,L", 2, NULL },
			    { "SET 0,(HL)", 2, NULL },
			    { "SET 0,A", 2, NULL },
			    { "SET 1,B", 2, NULL },
			    { "SET 1,C", 2, NULL },
			    { "SET 1,D", 2, NULL },
			    { "SET 1,E", 2, NULL },
			    { "SET 1,H", 2, NULL },
			    { "SET 1,L", 2, NULL },
			    { "SET 1,(HL)", 2, NULL },
			    { "SET 1,A", 2, NULL },
			    
			    { "SET 2,B", 2, NULL },
			    { "SET 2,C", 2, NULL },
			    { "SET 2,D", 2, NULL },
			    { "SET 2,E", 2, NULL },
			    { "SET 2,H", 2, NULL },
			    { "SET 2,L", 2, NULL },
			    { "SET 2,(HL)", 2, NULL },
			    { "SET 2,A", 2, NULL },
			    { "SET 3,B", 2, NULL },
			    { "SET 3,C", 2, NULL },
			    { "SET 3,D", 2, NULL },
			    { "SET 3,E", 2, NULL },
			    { "SET 3,H", 2, NULL },
			    { "SET 3,L", 2, NULL },
			    { "SET 3,(HL)", 2, NULL },
			    { "SET 3,A", 2, NULL },

			    { "SET 4,B", 2, NULL },
			    { "SET 4,C", 2, NULL },
			    { "SET 4,D", 2, NULL },
			    { "SET 4,E", 2, NULL },
			    { "SET 4,H", 2, NULL },
			    { "SET 4,L", 2, NULL },
			    { "SET 4,(HL)", 2, NULL },
			    { "SET 4,A", 2, NULL },
			    { "SET 5,B", 2, NULL },
			    { "SET 5,C", 2, NULL },
			    { "SET 5,D", 2, NULL },
			    { "SET 5,E", 2, NULL },
			    { "SET 5,H", 2, NULL },
			    { "SET 5,L", 2, NULL },
			    { "SET 5,(HL)", 2, NULL },
			    { "SET 5,A", 2, NULL },

			    { "SET 6,B", 2, NULL },
			    { "SET 6,C", 2, NULL },
			    { "SET 6,D", 2, NULL },
			    { "SET 6,E", 2, NULL },
			    { "SET 6,H", 2, NULL },
			    { "SET 6,L", 2, NULL },
			    { "SET 6,(HL)", 2, NULL },
			    { "SET 6,A", 2, NULL },
			    { "SET 7,B", 2, NULL },
			    { "SET 7,C", 2, NULL },
			    { "SET 7,D", 2, NULL },
			    { "SET 7,E", 2, NULL },
			    { "SET 7,H", 2, NULL },
			    { "SET 7,L", 2, NULL },
			    { "SET 7,(HL)", 2, NULL },
			    { "SET 7,A", 2, NULL } };

int
load_rom(gb_cpu *cpu, const char *rom_path)
{
	FILE *fp;
	int size;

	if ((fp = fopen(rom_path, "rb")) == NULL) {
		fprintf(stderr, "Failed to open file %s: %s\n", rom_path, strerror(errno));
		return 1;
	}

	fseek(fp, 0, SEEK_END);
	size = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	if (fread(cpu->rom, 1, size, fp) != size) {
		fprintf(stderr, "Failed reading file %s: %s\n", rom_path, strerror(errno));
		fclose(fp);
		return 1;
	}

	fprintf(stderr, "Successfully read %d bytes\n", size);
	cpu->rom_size = size; /* Can also read rom[0x148] to get rom size */

	fclose(fp);
	return 0;
}

int
power(gb_cpu *cpu)
{
	/* Load the first two banks (32KB) into main memory */
	memset(cpu->memory, 0, 0x10000);
	memcpy(cpu->memory, cpu->rom, 0x8000);

	/* Initialize Gameboy */
	cpu->pc = 0x100;
	cpu->stack = (WORD*)&cpu->memory[0xFFFE];

	cpu->regs[REG_AF] = 0x01B0;
	cpu->regs[REG_BC] = 0x0013;
	cpu->regs[REG_DE] = 0x00D8;
	cpu->regs[REG_HL] = 0x014D;

	cpu->memory[0xFF05] = 0x00; 
	cpu->memory[0xFF06] = 0x00; 
	cpu->memory[0xFF07] = 0x00; 
	cpu->memory[0xFF10] = 0x80; 
	cpu->memory[0xFF11] = 0xBF; 
	cpu->memory[0xFF12] = 0xF3; 
	cpu->memory[0xFF14] = 0xBF; 
	cpu->memory[0xFF16] = 0x3F; 
	cpu->memory[0xFF17] = 0x00; 
	cpu->memory[0xFF19] = 0xBF; 
	cpu->memory[0xFF1A] = 0x7F; 
	cpu->memory[0xFF1B] = 0xFF; 
	cpu->memory[0xFF1C] = 0x9F; 
	cpu->memory[0xFF1E] = 0xBF; 
	cpu->memory[0xFF20] = 0xFF; 
	cpu->memory[0xFF21] = 0x00; 
	cpu->memory[0xFF22] = 0x00; 
	cpu->memory[0xFF23] = 0xBF; 
	cpu->memory[0xFF24] = 0x77; 
	cpu->memory[0xFF25] = 0xF3;
	cpu->memory[0xFF26] = 0xF1; 
	cpu->memory[0xFF40] = 0x91; 
	cpu->memory[0xFF42] = 0x00; 
	cpu->memory[0xFF43] = 0x00; 
	cpu->memory[0xFF45] = 0x00; 
	cpu->memory[0xFF47] = 0xFC; 
	cpu->memory[0xFF48] = 0xFF; 
	cpu->memory[0xFF49] = 0xFF; 
	cpu->memory[0xFF4A] = 0x00; 
	cpu->memory[0xFF4B] = 0x00; 
	cpu->memory[0xFFFF] = 0x00;

	return 0;
}

int
exec_op(gb_cpu *cpu)
{
	const op *opcode;
	BYTE *arg;

	opcode = &ops[cpu->memory[cpu->pc]];
	arg = &cpu->memory[cpu->pc + 1];

	if (opcode->func == NULL)
		return -1;

	switch (opcode->arg_size) {
	case 1:
		((void(*)(gb_cpu*))opcode->func)(cpu);
		break;
	case 2:
		((void(*)(gb_cpu*, BYTE))opcode->func)(cpu, *arg);
		break;
	case 3:
		((void(*)(gb_cpu*, WORD))opcode->func)(cpu, *(WORD*)arg);
		break;
	}

	return opcode->arg_size;
}

void
disassemble(const gb_cpu *cpu)
{
	int pc;
	BYTE op;

	pc = 0;
	while (pc != cpu->rom_size) {
		op = cpu->rom[pc];

		if (op == 0xCB) {
			op = cpu->rom[pc + 1];
			printf("%6x: %s\n", pc, ext_ops[op].assembly);

			pc += ext_ops[op].arg_size + 1;
		} else {
			printf("%6x: %s\n", pc, ops[op].assembly);

			pc += ops[op].arg_size;
		}
	}
}

void
cpu_status(const gb_cpu *cpu)
{
	printf("\x1B[2J\x1B[;H");
	printf("pc: %04x (%s)\n"
	       "reg_af: %04x\n"
	       "\ta: %02x\n"
	       "\tf: %02x\n"
	       "reg_bc: %04x\n"
	       "\tb: %02x\n"
	       "\tc: %02x\n"
	       "reg_de: %04x\n"
	       "\td: %02x\n"
	       "\te: %02x\n"
	       "reg_hl: %04x\n"
	       "\th: %02x\n"
	       "\tl: %02x\n",
	       cpu->pc, ops[cpu->memory[cpu->pc]].assembly,
	       cpu->regs[REG_AF], HI(cpu->regs[REG_AF]), LO(cpu->regs[REG_AF]),
	       cpu->regs[REG_BC], HI(cpu->regs[REG_BC]), LO(cpu->regs[REG_BC]),
	       cpu->regs[REG_DE], HI(cpu->regs[REG_DE]), LO(cpu->regs[REG_DE]),
	       cpu->regs[REG_HL], HI(cpu->regs[REG_HL]), LO(cpu->regs[REG_HL]));
}

void
nop(gb_cpu *cpu)
{ }

void
ld_bc_d16(gb_cpu *cpu, WORD d16)
{ cpu->regs[REG_BC] = d16; }



