#include "../include/linuxboy.h"

#include <stdio.h>
#include <string.h>
#include <errno.h>

const op ops[0x100] = { { "NOP", 1, op_0x00 },
			{ "LD BC,%04x", 3, op_0x01 },
			{ "LD (BC),A", 1, op_0x02 },
			{ "INC BC", 1, op_0x03 },
			{ "INC B", 1, op_0x04 },
			{ "DEC B", 1, op_0x05 },
			{ "LD B,%02x", 2, op_0x06 },
			{ "RLCA", 1, op_0x07 },
			{ "LD ($%04x),SP", 3, op_0x08 },
			{ "ADD HL,BC", 1, op_0x09 },
			{ "LD A,(BC)", 1, op_0x0A },
			{ "DEC BC", 1, op_0x0B },
			{ "INC C", 1, op_0x0C },
			{ "DEC C", 1, op_0x0D },
			{ "LD C,%02x", 2, op_0x0E },
			{ "RRCA", 1, op_0x0F },
			
			{ "STOP 0", 2, op_0x10 },
			{ "LD DE,%04x", 3, op_0x11 },
			{ "LD (DE),A", 1, op_0x12 },
			{ "INC DE", 1, op_0x13 },
			{ "INC D", 1, op_0x14 },
			{ "DEC D", 1, op_0x15 },
			{ "LD D,%02x", 2, op_0x16 },
			{ "RLA", 1, op_0x17 },
			{ "JR %02x", 2, op_0x18 },
			{ "ADD HL,DE", 1, op_0x19 },
			{ "LD A,(DE)", 1, op_0x1A },
			{ "DEC DE", 1, op_0x1B },
			{ "INC E", 1, op_0x1C },
			{ "DEC E", 1, op_0x1D },
			{ "LD E,%02x", 2, op_0x1E },
			{ "RRA", 1, op_0x1F },
			
			{ "JR NZ,%02x", 2, op_0x20 },
			{ "LD HL,%04x", 3, op_0x21 },
			{ "LD (HL+),A", 1, op_0x22 },
			{ "INC HL", 1, op_0x23 },
			{ "INC H", 1, op_0x24 },
			{ "DEC H", 1, op_0x25 },
			{ "LD H,%02x", 2, op_0x26 },
			{ "DAA", 1, op_0x27 },
			{ "JR Z,%02x", 2, op_0x28 },
			{ "ADD HL,HL", 1, op_0x29 },
			{ "LD A,(HL+)", 1, op_0x2A },
			{ "DEC HL", 1, op_0x2B },
			{ "INC L", 1, op_0x2C },
			{ "DEC L", 1, op_0x2D },
			{ "LD L,%02x", 2, op_0x2E },
			{ "CPL", 1, op_0x2F },
			
			{ "JR NC,%02x", 2, op_0x30 },
			{ "LD SP,%04x", 3, op_0x31 },
			{ "LD (HL-),A", 1, op_0x32 },
			{ "INC SP", 1, op_0x33 },
			{ "INC (HL)", 1, op_0x34 },
			{ "DEC (HL)", 1, op_0x35 },
			{ "LD (HL),d8", 2, op_0x36 },
			{ "SCF", 1, op_0x37 },
			{ "JR C,%02x", 2, op_0x38 },
			{ "ADD HL,SP", 1, op_0x39 },
			{ "LD A,(HL-)", 1, op_0x3A },
			{ "DEC SP", 1, op_0x3B },
			{ "INC A", 1, op_0x3C },
			{ "DEC A", 1, op_0x3D },
			{ "LD A,%02x", 2, op_0x3E },
			{ "CCF", 1, op_0x3F },
			
			{ "LD B,B", 1, op_0x40 },
			{ "LD B,C", 1, op_0x41 },
			{ "LD B,D", 1, op_0x42 },
			{ "LD B,E", 1, op_0x43 },
			{ "LD B,H", 1, op_0x44 },
			{ "LD B,L", 1, op_0x45 },
			{ "LD B,(HL)", 1, op_0x46 },
			{ "LD B,A", 1, op_0x47 },
			{ "LD C,B", 1, op_0x48 },
			{ "LD C,C", 1, op_0x49 },
			{ "LD C,D", 1, op_0x4A },
			{ "LD C,E", 1, op_0x4B },
			{ "LD C,H", 1, op_0x4C },
			{ "LD C,L", 1, op_0x4D },
			{ "LD C,(HL)", 1, op_0x4E },
			{ "LD C,A", 1, op_0x4F },
			
			{ "LD D,B", 1, op_0x50 },
			{ "LD D,C", 1, op_0x51 },
			{ "LD D,D", 1, op_0x52 },
			{ "LD D,E", 1, op_0x53 },
			{ "LD D,H", 1, op_0x54 },
			{ "LD D,L", 1, op_0x55 },
			{ "LD D,(HL)", 1, op_0x56 },
			{ "LD D,A", 1, op_0x57 },
			{ "LD E,B", 1, op_0x58 },
			{ "LD E,C", 1, op_0x59 },
			{ "LD E,D", 1, op_0x5A },
			{ "LD E,E", 1, op_0x5B },
			{ "LD E,H", 1, op_0x5C },
			{ "LD E,L", 1, op_0x5D },
			{ "LD E,(HL)", 1, op_0x5E },
			{ "LD E,A", 1, op_0x5F },
			
			{ "LD H,B", 1, op_0x60 },
			{ "LD H,C", 1, op_0x61 },
			{ "LD H,D", 1, op_0x62 },
			{ "LD H,E", 1, op_0x63 },
			{ "LD H,H", 1, op_0x64 },
			{ "LD H,L", 1, op_0x65 },
			{ "LD H,(HL)", 1, op_0x66 },
			{ "LD H,A", 1, op_0x67 },
			{ "LD L,B", 1, op_0x68 },
			{ "LD L,C", 1, op_0x69 },
			{ "LD L,D", 1, op_0x6A },
			{ "LD L,E", 1, op_0x6B },
			{ "LD L,H", 1, op_0x6C },
			{ "LD L,L", 1, op_0x6D },
			{ "LD L,(HL)", 1, op_0x6E },
			{ "LD L,A", 1, op_0x6F },
			
			{ "LD (HL),B", 1, op_0x70 },
			{ "LD (HL),C", 1, op_0x71 },
			{ "LD (HL),D", 1, op_0x72 },
			{ "LD (HL),E", 1, op_0x73 },
			{ "LD (HL),H", 1, op_0x74 },
			{ "LD (HL),L", 1, op_0x75 },
			{ "HALT", 1, op_0x76 },
			{ "LD (HL),A", 1, op_0x77 },
			{ "LD A,B", 1, op_0x78 },
			{ "LD A,C", 1, op_0x79 },
			{ "LD A,D", 1, op_0x7A },
			{ "LD A,E", 1, op_0x7B },
			{ "LD A,H", 1, op_0x7C },
			{ "LD A,L", 1, op_0x7D },
			{ "LD A,(HL)", 1, op_0x7E },
			{ "LD A,A", 1, op_0x7F },
			
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
			{ "JP $%04x", 3, NULL },
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
	cpu->stack = (reg*)&cpu->memory[0xFFFE];

	cpu->regs[REG_AF].reg = 0x01B0;
	cpu->regs[REG_BC].reg = 0x0013;
	cpu->regs[REG_DE].reg = 0x00D8;
	cpu->regs[REG_HL].reg = 0x014D;

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

	cpu->pc += opcode->arg_size;

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
	op opcode = ops[cpu->memory[cpu->pc]];

	printf("\x1B[2J\x1B[;H");
	printf("pc: %04x\tasm: ", cpu->pc);

	switch (opcode.arg_size) {
	case 1:
		printf(opcode.assembly);
		break;
	case 2:
		printf(opcode.assembly, cpu->memory[cpu->pc + 1]);
		break;
	case 3:
		printf(opcode.assembly, *(WORD*)&cpu->memory[cpu->pc + 1]);
		break;
	}

	printf("\nreg_af: %04x\n"
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
	       cpu->regs[REG_AF].reg, cpu->regs[REG_AF].hi, cpu->regs[REG_AF].lo,
	       cpu->regs[REG_BC].reg, cpu->regs[REG_BC].hi, cpu->regs[REG_BC].lo,
	       cpu->regs[REG_DE].reg, cpu->regs[REG_DE].hi, cpu->regs[REG_DE].lo,
	       cpu->regs[REG_HL].reg, cpu->regs[REG_HL].hi, cpu->regs[REG_HL].lo);
}

/*
 * ---=== OPCODE HELPER FUNCTIONS ===---
 */

void
inc_byte(BYTE *flag, BYTE *b)
{
	RESET_FLAGS(*flag, BIT(FLAG_Z) | BIT(FLAG_N) | BIT(FLAG_H));

	if ((BYTE)(*b + 1) == 0)
		*flag |= BIT(FLAG_Z);

	if (*b + 1 == BIT(4))
		*flag |= BIT(FLAG_H);

	++*b;
}

void
dec_byte(BYTE *flag, BYTE *b)
{
	RESET_FLAGS(*flag, BIT(FLAG_Z) | BIT(FLAG_N) | BIT(FLAG_H));

	if ((BYTE)(*b - 1) == 0)
		*flag |= BIT(FLAG_Z);

	if (*b - 1 == -1)
		*flag |= BIT(FLAG_H);

	--*b;
}

void
rot_byte(BYTE *flag, BYTE *b, int rot_flags)
{
	BYTE bit;

	if (rot_flags & LEFT) {
		bit = *b >> 7;
		
		*b <<= 1;
		if (rot_flags & CIRCULAR)
			*b |= bit;
		else
			*b |= (*flag >> FLAG_C) & 1;
	} else {
		bit = *b & 1;

		*b >>= 1;
		if (rot_flags & CIRCULAR)
			*b |= bit << 7;
		else
			*b |= (*flag & BIT(FLAG_C)) << (8 - FLAG_C);
	}

	*flag ^= (-bit ^ *flag) & BIT(FLAG_C);
}

void
add_word(BYTE *flag, WORD *w, WORD val)
{
	RESET_FLAGS(*flag, BIT(FLAG_N));

	*flag ^= (-(*w + val > 0xFFFF) ^ *flag) & BIT(FLAG_C);

	*w += val;
}

BYTE
read_byte(gb_cpu *cpu, WORD addr)
{
	return cpu->memory[addr];
}

void
write_byte(gb_cpu *cpu, WORD addr, BYTE val)
{
	cpu->memory[addr] = val;
}

WORD
read_word(gb_cpu *cpu, WORD addr)
{
	return *(WORD*)&cpu->memory[addr];
}

void
write_word(gb_cpu *cpu, WORD addr, WORD val)
{
	*(WORD*)&cpu->memory[addr] = val;
}

/*
 * ---=== OPCODES ===---
 */

/* NOP */
void
op_0x00(gb_cpu *cpu)
{ }

/* LD BC,d16 */
void
op_0x01(gb_cpu *cpu, WORD d16)
{
	cpu->regs[REG_BC].reg = d16;
}

/* LD (BC),A */
void
op_0x02(gb_cpu *cpu)
{
	write_byte(cpu, cpu->regs[REG_BC].reg, cpu->regs[REG_AF].hi);
}
	
/* INC BC */
void
op_0x03(gb_cpu *cpu)
{
	++cpu->regs[REG_BC].reg;
}

/* INC B */
void
op_0x04(gb_cpu *cpu)
{
	inc_byte(&cpu->regs[REG_AF].lo, &cpu->regs[REG_BC].hi);
}

/* DEC B */
void
op_0x05(gb_cpu *cpu)
{
	dec_byte(&cpu->regs[REG_AF].lo, &cpu->regs[REG_BC].hi);
}

/* LD B,d8 */
void
op_0x06(gb_cpu *cpu, BYTE d8)
{
	cpu->regs[REG_BC].hi = d8;
}

/* RLCA */
void
op_0x07(gb_cpu *cpu)
{
	/*
	BYTE most_sig;

	RESET_FLAGS(cpu->regs[REG_AF].lo, FLAG_C | FLAG_N | FLAG_H);

	most_sig = cpu->regs[REG_AF].hi >> 7;

	cpu->regs[REG_AF].hi = (cpu->regs[REG_AF].hi << 1) | most_sig;
	cpu->regs[REG_AF].lo ^= (-most_sig ^ cpu->regs[REG_AF].lo) & (most_size << FLAG_C);
	*/
	rot_byte(&cpu->regs[REG_AF].lo, &cpu->regs[REG_AF].hi, LEFT | CIRCULAR);
}

/* LD (a16),SP */
void
op_0x08(gb_cpu *cpu, WORD a16)
{
	write_word(cpu, cpu->memory[a16], cpu->stack->reg);
}

/* ADD HL,BC */
void
op_0x09(gb_cpu *cpu)
{
	add_word(&cpu->regs[REG_AF].lo, &cpu->regs[REG_HL].reg, cpu->regs[REG_BC].reg);
}

/* LD A,(BC) */
void
op_0x0A(gb_cpu *cpu)
{
	cpu->regs[REG_AF].hi = read_byte(cpu, cpu->regs[REG_BC].reg);
}

/* DEC BC */
void
op_0x0B(gb_cpu *cpu)
{
	--cpu->regs[REG_BC].reg;
}

/* INC C */
void
op_0x0C(gb_cpu *cpu)
{
	inc_byte(&cpu->regs[REG_AF].lo, &cpu->regs[REG_BC].lo);
}

/* DEC C */
void
op_0x0D(gb_cpu *cpu)
{
	dec_byte(&cpu->regs[REG_AF].lo, &cpu->regs[REG_BC].lo);
}

/* LD C,d8 */
void
op_0x0E(gb_cpu *cpu, BYTE d8)
{
	cpu->regs[REG_BC].lo = d8;
}

/* RRCA */
void
op_0x0F(gb_cpu *cpu)
{
	rot_byte(&cpu->regs[REG_AF].lo, &cpu->regs[REG_AF].hi, RIGHT | CIRCULAR);
}

/* STOP 0 */
void
op_0x10(gb_cpu *cpu, BYTE d8)
{
	IMPLEMENT("STOP 0");
}

/* LD DE,d16 */
void
op_0x11(gb_cpu *cpu, WORD d16)
{
	cpu->regs[REG_DE].reg = d16;
}

/* LD (DE),A */
void
op_0x12(gb_cpu *cpu)
{
	write_byte(cpu, cpu->regs[REG_DE].reg, cpu->regs[REG_AF].hi);
}

/* INC DE */
void
op_0x13(gb_cpu *cpu)
{
	++cpu->regs[REG_DE].reg;
}

/* INC D */
void
op_0x14(gb_cpu *cpu)
{
	inc_byte(&cpu->regs[REG_AF].lo, &cpu->regs[REG_DE].hi);
}

/* DEC D */
void
op_0x15(gb_cpu *cpu)
{
	dec_byte(&cpu->regs[REG_AF].lo, &cpu->regs[REG_DE].hi);
}

/* LD D,d8 */
void
op_0x16(gb_cpu *cpu, BYTE d8)
{
	cpu->regs[REG_DE].hi = d8;
}

/* RLA */
void
op_0x17(gb_cpu *cpu)
{
	rot_byte(&cpu->regs[REG_AF].lo, &cpu->regs[REG_AF].hi, LEFT);
}

/* JR r8 */
void
op_0x18(gb_cpu *cpu, BYTE r8)
{
	cpu->pc += r8;
}

/* ADD HL,DE */
void
op_0x19(gb_cpu *cpu)
{
	add_word(&cpu->regs[REG_AF].lo, &cpu->regs[REG_HL].reg, cpu->regs[REG_DE].reg);
}

/* LD A,(DE) */
void
op_0x1A(gb_cpu *cpu)
{
	cpu->regs[REG_AF].hi = read_byte(cpu, cpu->regs[REG_DE].reg);
}

/* DEC DE */
void
op_0x1B(gb_cpu *cpu)
{
	--cpu->regs[REG_DE].reg;
}

/* INC E */
void
op_0x1C(gb_cpu *cpu)
{
	inc_byte(&cpu->regs[REG_AF].lo, &cpu->regs[REG_DE].lo);
}

/* DEC E */
void
op_0x1D(gb_cpu *cpu)
{
	dec_byte(&cpu->regs[REG_AF].lo, &cpu->regs[REG_DE].lo);
}

/* LD E,d8 */
void
op_0x1E(gb_cpu *cpu, BYTE d8)
{
	cpu->regs[REG_DE].lo = d8;
}

/* RRA */
void
op_0x1F(gb_cpu *cpu)
{
	rot_byte(&cpu->regs[REG_AF].lo, &cpu->regs[REG_AF].hi, RIGHT);
}

/* JR NZ,r8 */
void
op_0x20(gb_cpu *cpu, BYTE r8)
{
	if (!(cpu->regs[REG_AF].lo & BIT(FLAG_Z)))
		cpu->pc += r8;
}

/* LD HL,d16 */
void op_0x21(gb_cpu *cpu, WORD d16)
{
	cpu->regs[REG_HL].reg = d16;
}

/* LD (HL+),A */
void op_0x22(gb_cpu *cpu)
{
	write_byte(cpu, cpu->regs[REG_HL].reg++, cpu->regs[REG_AF].hi);
}

/* INC HL */
void
op_0x23(gb_cpu *cpu)
{
	++cpu->regs[REG_HL].reg;
}

/* INC H */
void
op_0x24(gb_cpu *cpu)
{
	inc_byte(&cpu->regs[REG_AF].lo, &cpu->regs[REG_HL].hi);
}

/* DEC H */
void
op_0x25(gb_cpu *cpu)
{
	dec_byte(&cpu->regs[REG_AF].lo, &cpu->regs[REG_HL].hi);
}

/* LD H,d8 */
void
op_0x26(gb_cpu *cpu, BYTE d8)
{
	cpu->regs[REG_HL].hi = d8;
}

/* DDA */
void
op_0x27(gb_cpu *cpu)
{
	IMPLEMENT("DDA");
}

/* JR Z,r8 */
void
op_0x28(gb_cpu *cpu, BYTE r8)
{
	if (cpu->regs[REG_AF].lo & BIT(FLAG_Z))
		cpu->pc += r8;
}

/* ADD HL,HL */
void
op_0x29(gb_cpu *cpu)
{
	add_word(&cpu->regs[REG_AF].lo, &cpu->regs[REG_HL].reg, cpu->regs[REG_HL].reg);
}

/* LD A,(HL+) */
void
op_0x2A(gb_cpu *cpu)
{
	cpu->regs[REG_AF].hi = read_byte(cpu, cpu->regs[REG_HL].reg++);
}

/* DEC HL */
void
op_0x2B(gb_cpu *cpu)
{
	--cpu->regs[REG_HL].reg;
}

/* INC L */
void
op_0x2C(gb_cpu *cpu)
{
	inc_byte(&cpu->regs[REG_AF].lo, &cpu->regs[REG_HL].lo);
}

/* DEC L */
void
op_0x2D(gb_cpu *cpu)
{
	dec_byte(&cpu->regs[REG_AF].lo, &cpu->regs[REG_HL].lo);
}

/* LD L,d8 */
void
op_0x2E(gb_cpu *cpu, BYTE d8)
{
	cpu->regs[REG_HL].lo = d8;
}

/* CPL */
void
op_0x2F(gb_cpu *cpu)
{
	cpu->regs[REG_AF].hi = ~cpu->regs[REG_AF].hi;

	cpu->regs[REG_AF].lo |= BIT(FLAG_N) | BIT(FLAG_H);
}

/* JR NC,r8 */
void op_0x30(gb_cpu *cpu, BYTE r8)
{
	if ((cpu->regs[REG_AF].lo & BIT(FLAG_C)))
		cpu->pc += r8;
}

/* LD SP,d16 */
void
op_0x31(gb_cpu *cpu, WORD d16)
{
	cpu->stack->reg = d16;
}

/* LD (HL-),A */
void
op_0x32(gb_cpu *cpu)
{
	write_byte(cpu, cpu->regs[REG_HL].reg--, cpu->regs[REG_AF].hi);
}

/* INC SP */
void
op_0x33(gb_cpu *cpu)
{
	++cpu->stack->reg;
}

/* INC (HL) */
/* ?????????????? */
void
op_0x34(gb_cpu *cpu)
{
	inc_byte(&cpu->regs[REG_AF].lo, &cpu->memory[cpu->regs[REG_HL].reg]);
}

/* DEC (HL) */
void
op_0x35(gb_cpu *cpu)
{
	dec_byte(&cpu->regs[REG_AF].lo, &cpu->memory[cpu->regs[REG_HL].reg]); 
}

/* LD (HL),d8 */
void
op_0x36(gb_cpu *cpu, BYTE d8)
{
	write_byte(cpu, cpu->regs[REG_HL].reg, d8);
}

/* SCF */
void
op_0x37(gb_cpu *cpu)
{
	RESET_FLAGS(cpu->regs[REG_AF].lo, BIT(FLAG_N) | BIT(FLAG_H));

	cpu->regs[REG_AF].lo |= BIT(FLAG_C);
}

/* JR C,r8 */ 
void
op_0x38(gb_cpu *cpu, BYTE r8)
{
	if (cpu->regs[REG_AF].lo & BIT(FLAG_C))
		cpu->pc += r8;
}

/* ADD HL,SP */
void
op_0x39(gb_cpu *cpu)
{
	add_word(&cpu->regs[REG_AF].lo, &cpu->regs[REG_HL].reg, cpu->stack->reg);
}

/* LD A,(HL-) */
void
op_0x3A(gb_cpu *cpu)
{
	cpu->regs[REG_AF].hi = read_byte(cpu, cpu->regs[REG_HL].reg--);
}

/* DEC SP */
void
op_0x3B(gb_cpu *cpu)
{
	--cpu->stack->reg;
}

/* INC A */
void
op_0x3C(gb_cpu *cpu)
{
	inc_byte(&cpu->regs[REG_AF].lo, &cpu->regs[REG_AF].hi);
}

/* DEC A */
void
op_0x3D(gb_cpu *cpu)
{
	dec_byte(&cpu->regs[REG_AF].lo, &cpu->regs[REG_AF].hi);
}

/* LD A,d8 */
void
op_0x3E(gb_cpu *cpu, BYTE d8)
{
	cpu->regs[REG_AF].hi = d8;
}

/* CCF */
void
op_0x3F(gb_cpu *cpu)
{
	RESET_FLAGS(cpu->regs[REG_AF].lo, BIT(FLAG_N) | BIT(FLAG_H));

	cpu->regs[REG_AF].lo ^= BIT(FLAG_C);
}

/* LD B,B */
void
op_0x40(gb_cpu *cpu)
{
	cpu->regs[REG_BC].hi = cpu->regs[REG_BC].hi;
}

/* LD B,C */
void
op_0x41(gb_cpu *cpu)
{
	cpu->regs[REG_BC].hi = cpu->regs[REG_BC].lo;
}

/* LD B,D */
void
op_0x42(gb_cpu *cpu)
{
        cpu->regs[REG_BC].hi = cpu->regs[REG_DE].hi;
}

/* LD B,E */
void
op_0x43(gb_cpu *cpu)
{
        cpu->regs[REG_BC].hi = cpu->regs[REG_DE].lo;
}

/* LD B,H */
void
op_0x44(gb_cpu *cpu)
{
	cpu->regs[REG_BC].hi = cpu->regs[REG_HL].hi;
}

/* LD B,L */
void
op_0x45(gb_cpu *cpu)
{
        cpu->regs[REG_BC].hi = cpu->regs[REG_HL].lo;
}

/* LD B,(HL) */
void
op_0x46(gb_cpu *cpu)
{
	cpu->regs[REG_BC].hi = read_byte(cpu, cpu->regs[REG_HL].reg);
}

/* LD B,A */
void
op_0x47(gb_cpu *cpu)
{
        cpu->regs[REG_BC].hi = cpu->regs[REG_AF].hi;
}

/* LD C,B */
void
op_0x48(gb_cpu *cpu)
{
	cpu->regs[REG_BC].lo = cpu->regs[REG_BC].hi;
}

/* LD C,C */
void
op_0x49(gb_cpu *cpu)
{
        cpu->regs[REG_BC].lo = cpu->regs[REG_BC].lo;
}

/* LD C,D */
void
op_0x4A(gb_cpu *cpu)
{
        cpu->regs[REG_BC].lo = cpu->regs[REG_DE].hi;
}

/* LD C,E */
void
op_0x4B(gb_cpu *cpu)
{
        cpu->regs[REG_BC].lo = cpu->regs[REG_DE].lo;
}

/* LD C,H */
void
op_0x4C(gb_cpu *cpu)
{
        cpu->regs[REG_BC].lo = cpu->regs[REG_HL].hi;
}

/* LD C,L */
void
op_0x4D(gb_cpu *cpu)
{
        cpu->regs[REG_BC].lo = cpu->regs[REG_HL].lo;
}

/* LD C,(HL) */
void
op_0x4E(gb_cpu *cpu)
{
        cpu->regs[REG_BC].lo = read_byte(cpu, cpu->regs[REG_HL].reg);
}

/* LD C,A */
void
op_0x4F(gb_cpu *cpu)
{
        cpu->regs[REG_BC].lo = cpu->regs[REG_AF].hi;
}

/* LD D,B */
void
op_0x50(gb_cpu *cpu)
{
        cpu->regs[REG_DE].hi = cpu->regs[REG_BC].hi;
}

/* LD D,C */
void
op_0x51(gb_cpu *cpu)
{
        cpu->regs[REG_DE].hi = cpu->regs[REG_BC].lo;
}

/* LD D,D */
void
op_0x52(gb_cpu *cpu)
{
        cpu->regs[REG_DE].hi = cpu->regs[REG_DE].hi;
}

/* LD D,E */
void
op_0x53(gb_cpu *cpu)
{
        cpu->regs[REG_DE].hi = cpu->regs[REG_DE].lo;
}

/* LD D,H */
void
op_0x54(gb_cpu *cpu)
{
        cpu->regs[REG_DE].hi = cpu->regs[REG_HL].hi;
}

/* LD D,L */
void
op_0x55(gb_cpu *cpu)
{
        cpu->regs[REG_DE].hi = cpu->regs[REG_HL].lo;
}

/* LD D,(HL) */
void
op_0x56(gb_cpu *cpu)
{
        cpu->regs[REG_DE].hi = read_byte(cpu, cpu->regs[REG_HL].reg);
}

/* LD D,A */
void
op_0x57(gb_cpu *cpu)
{
        cpu->regs[REG_DE].hi = cpu->regs[REG_AF].hi;
}

/* LD E,B */
void
op_0x58(gb_cpu *cpu)
{
        cpu->regs[REG_DE].lo = cpu->regs[REG_BC].hi;
}

/* LD E,C */
void
op_0x59(gb_cpu *cpu)
{
        cpu->regs[REG_DE].lo = cpu->regs[REG_BC].lo;
}

/* LD E,D */
void
op_0x5A(gb_cpu *cpu)
{
        cpu->regs[REG_DE].lo = cpu->regs[REG_DE].hi;
}

/* LD E,E */
void
op_0x5B(gb_cpu *cpu)
{
        cpu->regs[REG_DE].lo = cpu->regs[REG_DE].lo;
}

/* LD E,H */
void
op_0x5C(gb_cpu *cpu)
{
        cpu->regs[REG_DE].lo = cpu->regs[REG_HL].hi;
}

/* LD E,L */
void
op_0x5D(gb_cpu *cpu)
{
        cpu->regs[REG_DE].lo = cpu->regs[REG_HL].lo;
}

/* LD E,(HL) */
void
op_0x5E(gb_cpu *cpu)
{
        cpu->regs[REG_DE].lo = read_byte(cpu, cpu->regs[REG_HL].reg);
}

/* LD E,A */
void
op_0x5F(gb_cpu *cpu)
{
        cpu->regs[REG_DE].lo = cpu->regs[REG_AF].hi;
}

/* LD H,B */
void
op_0x60(gb_cpu *cpu)
{
        cpu->regs[REG_HL].hi = cpu->regs[REG_BC].hi;
}

/* LD H,C */
void
op_0x61(gb_cpu *cpu)
{
        cpu->regs[REG_HL].hi = cpu->regs[REG_BC].lo;
}

/* LD H,D */ 
void
op_0x62(gb_cpu *cpu)
{
        cpu->regs[REG_HL].hi = cpu->regs[REG_DE].hi;
}

/* LD H,E */
void
op_0x63(gb_cpu *cpu)
{
        cpu->regs[REG_HL].hi = cpu->regs[REG_DE].lo;
}

/* LD H,H */
void
op_0x64(gb_cpu *cpu)
{
        cpu->regs[REG_HL].hi = cpu->regs[REG_HL].hi;
}

/* LD H,L */
void
op_0x65(gb_cpu *cpu)
{
        cpu->regs[REG_HL].hi = cpu->regs[REG_HL].lo;
}

/* LD H,(HL) */
void
op_0x66(gb_cpu *cpu)
{
        cpu->regs[REG_HL].hi = read_byte(cpu, cpu->regs[REG_HL].reg);
}

/* LD H,A */
void
op_0x67(gb_cpu *cpu)
{
        cpu->regs[REG_HL].hi = cpu->regs[REG_AF].hi;
}

/* LD L,B */
void
op_0x68(gb_cpu *cpu)
{
        cpu->regs[REG_HL].lo = cpu->regs[REG_BC].hi;
}

/* LD L,C */
void
op_0x69(gb_cpu *cpu)
{
	cpu->regs[REG_HL].lo = cpu->regs[REG_BC].lo;
}

/* LD L,D */
void
op_0x6A(gb_cpu *cpu)
{
        cpu->regs[REG_HL].lo = cpu->regs[REG_DE].hi;
}

/* LD L,E */
void
op_0x6B(gb_cpu *cpu)
{
        cpu->regs[REG_HL].lo = cpu->regs[REG_DE].lo;
}

/* LD L,H */
void
op_0x6C(gb_cpu *cpu)
{	
        cpu->regs[REG_HL].lo = cpu->regs[REG_HL].hi;
}

/* LD L,L */
void
op_0x6D(gb_cpu *cpu)
{
        cpu->regs[REG_HL].lo = cpu->regs[REG_HL].lo;
}

/* LD L,(HL) */
void
op_0x6E(gb_cpu *cpu)
{
        cpu->regs[REG_HL].lo = read_byte(cpu, cpu->regs[REG_HL].reg);
}

/* LD L,A */
void
op_0x6F(gb_cpu *cpu)
{
        cpu->regs[REG_HL].lo = cpu->regs[REG_AF].hi;
}

/* LD (HL),B */
void
op_0x70(gb_cpu *cpu)
{
	write_byte(cpu, cpu->regs[REG_HL].reg, cpu->regs[REG_BC].hi);
}

/* LD (HL),C */
void
op_0x71(gb_cpu *cpu)
{
        write_byte(cpu, cpu->regs[REG_HL].reg, cpu->regs[REG_BC].lo);
}

/* LD (HL),D */
void
op_0x72(gb_cpu *cpu)
{
	write_byte(cpu, cpu->regs[REG_HL].reg, cpu->regs[REG_DE].hi);
}

/* LD (HL),E */
void
op_0x73(gb_cpu *cpu)
{
        write_byte(cpu, cpu->regs[REG_HL].reg, cpu->regs[REG_DE].lo);
}

/* LD (HL),H */
void
op_0x74(gb_cpu *cpu)
{
        write_byte(cpu, cpu->regs[REG_HL].reg, cpu->regs[REG_HL].hi);
}

/* LD (HL),L */
void
op_0x75(gb_cpu *cpu)
{
        write_byte(cpu, cpu->regs[REG_HL].reg, cpu->regs[REG_HL].lo);
}

/* HALT */
void
op_0x76(gb_cpu *cpu)
{
	IMPLEMENT("HALT");
}

/* LD (HL),A */
void
op_0x77(gb_cpu *cpu)
{
        write_byte(cpu, cpu->regs[REG_HL].reg, cpu->regs[REG_AF].hi);
}

/* LD A,B */
void
op_0x78(gb_cpu *cpu)
{
        cpu->regs[REG_AF].hi = cpu->regs[REG_BC].hi;
}

/* LD A,C */
void
op_0x79(gb_cpu *cpu)
{
        cpu->regs[REG_AF].hi = cpu->regs[REG_BC].lo;
}

/* LD A,D */
void
op_0x7A(gb_cpu *cpu)
{
        cpu->regs[REG_AF].hi = cpu->regs[REG_DE].hi;
}

/* LD A,E */
void
op_0x7B(gb_cpu *cpu)
{
        cpu->regs[REG_AF].hi = cpu->regs[REG_DE].lo;
}

/* LD A,H */
void
op_0x7C(gb_cpu *cpu)
{
        cpu->regs[REG_AF].hi = cpu->regs[REG_HL].hi;
}

/* LD A,L */
void
op_0x7D(gb_cpu *cpu)
{
        cpu->regs[REG_AF].hi = cpu->regs[REG_HL].lo;
}

/* LD A,(HL) */
void
op_0x7E(gb_cpu *cpu)
{
        cpu->regs[REG_AF].hi = read_byte(cpu, cpu->regs[REG_HL].reg);
}

/* LD A,A */
void
op_0x7F(gb_cpu *cpu)
{
        cpu->regs[REG_AF].hi = cpu->regs[REG_AF].hi;
}

