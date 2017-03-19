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
			
			{ "STOP 0", 2, NULL },
			{ "LD DE,%04x", 3, op_0x11 },
			{ "LD (DE),A", 1, op_0x12 },
			{ "INC DE", 1, op_0x13 },
			{ "INC D", 1, op_0x14 },
			{ "DEC D", 1, op_0x15 },
			{ "LD D,%02x", 2, op_0x16 },
			{ "RLA", 1, op_0x17 },
			{ "JR %s%02x", 2 | SIGNED, op_0x18 },
			{ "ADD HL,DE", 1, op_0x19 },
			{ "LD A,(DE)", 1, op_0x1A },
			{ "DEC DE", 1, op_0x1B },
			{ "INC E", 1, op_0x1C },
			{ "DEC E", 1, op_0x1D },
			{ "LD E,%02x", 2, op_0x1E },
			{ "RRA", 1, op_0x1F },
			
			{ "JR NZ,%s%02x", 2 | SIGNED, op_0x20 },
			{ "LD HL,%04x", 3, op_0x21 },
			{ "LD (HL+),A", 1, op_0x22 },
			{ "INC HL", 1, op_0x23 },
			{ "INC H", 1, op_0x24 },
			{ "DEC H", 1, op_0x25 },
			{ "LD H,%02x", 2, op_0x26 },
			{ "DAA", 1, NULL },
			{ "JR Z,%s%02x", 2 | SIGNED, op_0x28 },
			{ "ADD HL,HL", 1, op_0x29 },
			{ "LD A,(HL+)", 1, op_0x2A },
			{ "DEC HL", 1, op_0x2B },
			{ "INC L", 1, op_0x2C },
			{ "DEC L", 1, op_0x2D },
			{ "LD L,%02x", 2, op_0x2E },
			{ "CPL", 1, op_0x2F },
			
			{ "JR NC,%s%02x", 2 | SIGNED, op_0x30 },
			{ "LD SP,%04x", 3, op_0x31 },
			{ "LD (HL-),A", 1, op_0x32 },
			{ "INC SP", 1, op_0x33 },
			{ "INC (HL)", 1, op_0x34 },
			{ "DEC (HL)", 1, op_0x35 },
			{ "LD (HL),d8", 2, op_0x36 },
			{ "SCF", 1, op_0x37 },
			{ "JR C,%s%02x", 2 | SIGNED, op_0x38 },
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
			{ "HALT", 1, NULL },
			{ "LD (HL),A", 1, op_0x77 },
			{ "LD A,B", 1, op_0x78 },
			{ "LD A,C", 1, op_0x79 },
			{ "LD A,D", 1, op_0x7A },
			{ "LD A,E", 1, op_0x7B },
			{ "LD A,H", 1, op_0x7C },
			{ "LD A,L", 1, op_0x7D },
			{ "LD A,(HL)", 1, op_0x7E },
			{ "LD A,A", 1, op_0x7F },
			
			{ "ADD A,B", 1, op_0x80 },
			{ "ADD A,C", 1, op_0x81 },
			{ "ADD A,D", 1, op_0x82 },
			{ "ADD A,E", 1, op_0x83 },
			{ "ADD A,H", 1, op_0x84 },
			{ "ADD A,L", 1, op_0x85 },
			{ "ADD A,(HL)", 1, op_0x86 },
			{ "ADD A,A", 1, op_0x87 },
			{ "ADC A,B", 1, op_0x88 },
			{ "ADC A,C", 1, op_0x89 },
			{ "ADC A,D", 1, op_0x8A },
			{ "ADC A,E", 1, op_0x8B },
			{ "ADC A,H", 1, op_0x8C },
			{ "ADC A,L", 1, op_0x8D },
			{ "ADC A,(HL)", 1, op_0x8E },
			{ "ADC A,A", 1, op_0x8F },
			
			{ "SUB B", 1, op_0x90 },
			{ "SUB C", 1, op_0x91 },
			{ "SUB D", 1, op_0x92 },
			{ "SUB E", 1, op_0x93 },
			{ "SUB H", 1, op_0x94 },
			{ "SUB L", 1, op_0x95 },
			{ "SUB (HL)", 1, op_0x96 },
			{ "SUB A", 1, op_0x97 },
			{ "SBC A,B", 1, op_0x98 },
			{ "SBC A,C", 1, op_0x99 },
			{ "SBC A,D", 1, op_0x9A },
			{ "SBC A,E", 1, op_0x9B },
			{ "SBC A,H", 1, op_0x9C },
			{ "SBC A,L", 1, op_0x9D },
			{ "SBC A,(HL)", 1, op_0x9E },
			{ "SBC A,A", 1, op_0x9F },
			
			{ "AND B", 1, op_0xA0 },
			{ "AND C", 1, op_0xA1 },
			{ "AND D", 1, op_0xA2 },
			{ "AND E", 1, op_0xA3 },
			{ "AND H", 1, op_0xA4 },
			{ "AND L", 1, op_0xA5 },
			{ "AND (HL)", 1, op_0xA6 },
			{ "AND A", 1, op_0xA7 },
			{ "XOR B", 1, op_0xA8 },
			{ "XOR C", 1, op_0xA9 },
			{ "XOR D", 1, op_0xAA },
			{ "XOR E", 1, op_0xAB },
			{ "XOR H", 1, op_0xAC },
			{ "XOR L", 1, op_0xAD },
			{ "XOR (HL)", 1, op_0xAE },
			{ "XOR A", 1, op_0xAF },
		
			{ "OR B", 1, op_0xB0 },
			{ "OR C", 1, op_0xB1 },
			{ "OR D", 1, op_0xB2 },
			{ "OR E", 1, op_0xB3 },
			{ "OR H", 1, op_0xB4 },
			{ "OR L", 1, op_0xB5 },
			{ "OR (HL)", 1, op_0xB6 },
			{ "OR A", 1, op_0xB7 },
			{ "CP B", 1, op_0xB8 },
			{ "CP C", 1, op_0xB9 },
			{ "CP D", 1, op_0xBA },
			{ "CP E", 1, op_0xBB },
			{ "CP H", 1, op_0xBC },
			{ "CP L", 1, op_0xBD },
			{ "CP (HL)", 1, op_0xBE },
			{ "CP A", 1, op_0xBF },
			
			{ "RET NZ", 1, op_0xC0 },
			{ "POP BC", 1, op_0xC1 },
			{ "JP NZ,$04x", 3, op_0xC2 },
			{ "JP $%04x", 3, op_0xC3 },
			{ "CALL NZ,$%04x", 3, op_0xC4 },
			{ "PUSH BC", 1, op_0xC5 },
			{ "ADD A,%02x", 2, op_0xC6 },
			{ "RST 00H", 1, op_0xC7 },
			{ "RET Z", 1, op_0xC8 },
			{ "RET", 1, op_0xC9 },
			{ "JP Z,$%04x", 3, op_0xCA },
			{ "PREFIX CB", 1, NULL },
			{ "CALL Z,$%04x", 3, op_0xCC },
			{ "CALL $%04x", 3, op_0xCD },
			{ "ADC A,%02x", 2, op_0xCE },
			{ "RST 08H", 1, op_0xCF },
			
			{ "RET NC", 1, op_0xD0 },
			{ "POP DE", 1, op_0xD1 },
			{ "JP NC,$%04x", 3, op_0xD2 },
			{ "--", 1, NULL },
			{ "CALL NC,$%04x", 3, op_0xD4 },
			{ "PUSH DE", 1, op_0xD5 },
			{ "SUB %02x", 2, op_0xD6 },
			{ "RST 10H", 1, op_0xD7 },
			{ "RET C", 1, op_0xD8 },
			{ "RETI", 1, op_0xD9 },
			{ "JP C,$%04x", 3, op_0xDA },
			{ "--", 1, NULL },
			{ "CALL C,$%04x", 3, op_0xDC },
			{ "--", 1, NULL },
			{ "SBC A,%02x", 2, op_0xDE },
			{ "RST 18H", 1, op_0xDF },
			
			{ "LDH (%02x),A", 2, op_0xE0 },
			{ "POP HL", 1, op_0xE1 },
			{ "LD (C),A", 1, op_0xE2 }, /* The opcode size may be 2 */
			{ "--", 1, NULL },
			{ "--", 1, NULL },
			{ "PUSH HL", 1, op_0xE5 },
			{ "AND %02x", 2, op_0xE6 },
			{ "RST 20H", 1, op_0xE7 },
			{ "ADD SP,%02x", 2, op_0xE8 },
			{ "JP (HL)", 1, op_0xE9 },
			{ "LD ($%04x),A", 3, op_0xEA },
			{ "--", 1, NULL },
			{ "--", 1, NULL },
			{ "--", 1, NULL },
			{ "XOR %02x", 2, op_0xEE },
			{ "RST 28H", 1, op_0xEF },
			
			{ "LDH A,(%02x)", 2, op_0xF0 },
			{ "POP AF", 1, op_0xF1 },
			{ "LD A,(C)", 1, op_0xF2 }, /* The opcode size may be 2 */
			{ "DI", 1, op_0xF3 },
			{ "--", 1, NULL },
			{ "PUSH AF", 1, op_0xF5 },
			{ "OR %02x", 2, op_0xF6 },
			{ "RST 30H", 1, op_0xF7 },
			{ "LD HL,SP+%02x", 2, NULL },
			{ "LD SP,HL", 1, op_0xF9 },
			{ "LD A,($%04x)", 3, op_0xFA },
			{ "EI", 1, op_0xFB },
			{ "--", 1, NULL },
			{ "--", 1, NULL },
			{ "CP %02x", 2, op_0xFE },
			{ "RST 38H", 1, op_0xFF } };

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

void
bootstrap(gb_cpu *cpu)
{
	FILE *fp;

	if ((fp = fopen("bootstrap.gb", "rb")) == NULL) {
		fprintf(stderr, "Failed to open bootstrap.gb: %s\n", strerror(errno));
		return;
	}

	cpu->pc = 0x0;
	fread(&cpu->memory[cpu->pc], 1, 0x100, fp);

	while (cpu->pc != 0x100) {
		cpu_status(cpu);

		if (exec_op(cpu) == -1)
			return;

		getchar();
	}
}

int
power(gb_cpu *cpu)
{
	/* Execute the DMG bootstrap */
	memset(cpu->memory, 0, 0x10000);
	/* bootstrap(cpu); */

	/* Load the first two ROM banks (32KB) into main mamery */
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
	void *arg;

	opcode = &ops[cpu->memory[cpu->pc]];
	arg = &cpu->memory[cpu->pc + 1];

	if (opcode->func == NULL) {
		printf("Missing implementation %s at $%04x", opcode->assembly, cpu->pc);
		return -1;
	}

	if (opcode->arg_size & SIGNED) {
		cpu->pc += opcode->arg_size - SIGNED;

		switch (opcode->arg_size - SIGNED) {
		case 1:
			((void(*)(gb_cpu*))opcode->func)(cpu);
			break;
		case 2:
			((void(*)(gb_cpu*, SIGNED_BYTE))opcode->func)(cpu, *(SIGNED_BYTE*)arg);
			break;
		case 3:
			((void(*)(gb_cpu*, SIGNED_WORD))opcode->func)(cpu, *(SIGNED_WORD*)arg);
			break;
		}

		return opcode->arg_size - SIGNED;
	} else {
		cpu->pc += opcode->arg_size;

		switch (opcode->arg_size) {
		case 1:
			((void(*)(gb_cpu*))opcode->func)(cpu);
			break;
		case 2:
			((void(*)(gb_cpu*, BYTE))opcode->func)(cpu, *(BYTE*)arg);
			break;
		case 3:
			((void(*)(gb_cpu*, WORD))opcode->func)(cpu, *(WORD*)arg);
			break;
		}

		return opcode->arg_size;
	}
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
	const op *opcode;

	const SIGNED_BYTE *sb;
	const SIGNED_WORD *sw;

	opcode = &ops[cpu->memory[cpu->pc]];

	printf("\x1B[2J\x1B[;H");
	printf("pc: %04x\tasm: (%02x) ", cpu->pc, cpu->memory[cpu->pc]);

	if (opcode->arg_size & SIGNED) {
		switch (opcode->arg_size - SIGNED) {
		case 1:
			printf(opcode->assembly);
			break;
		case 2:
			sb = (SIGNED_BYTE*)&cpu->memory[cpu->pc + 1];

			printf(opcode->assembly,
			       *sb < 0 ? "-" : "", *sb < 0 ? -*sb : *sb);
			break;
		case 3:
			sw = (SIGNED_WORD*)&cpu->memory[cpu->pc + 1];

			printf(opcode->assembly,
			       *sw < 0 ? "-" : "", *sw < 0 ? -*sw : *sw);
			
			break;
		}
	} else {
		switch (opcode->arg_size) {
		case 1:
			printf(opcode->assembly);
			break;
		case 2:
			printf(opcode->assembly, *(BYTE*)&cpu->memory[cpu->pc + 1]);
			break;
		case 3:
			printf(opcode->assembly, *(WORD*)&cpu->memory[cpu->pc + 1]);
			break;
		}
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
	       cpu->regs[REG_AF].reg, cpu->regs[REG_AF].hi, FLAG(cpu),
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
rot_byte(BYTE *flag, BYTE *b, BYTE rot_flags)
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
add_byte(BYTE *flag, BYTE *b, int val)
{
	RESET_FLAGS(*flag, BIT(FLAG_C) | BIT(FLAG_H) | BIT(FLAG_N) | BIT(FLAG_Z));

	if (*b + val > 0xFF)
		*flag |= BIT(FLAG_C);

	if ((*b ^ (*b + val)) & (1 << 4))
		*flag |= BIT(FLAG_H);

	*b += val;

	if (*b == 0)
		*flag |= BIT(FLAG_Z);
}

void
add_word(BYTE *flag, WORD *w, int val)
{
	RESET_FLAGS(*flag, BIT(FLAG_N));

	*flag ^= (-(*w + val > 0xFFFF) ^ *flag) & BIT(FLAG_C);

	*w += val;
}

void
sub_byte(BYTE *flag, BYTE *b, BYTE val)
{
	RESET_FLAGS(*flag, BIT(FLAG_C) | BIT(FLAG_H) | BIT(FLAG_Z));
	*flag |= BIT(FLAG_N);

	if (*b - val >= 0)
		*flag |= BIT(FLAG_C);

	if (!((*b ^ (*b - val)) & (1 << 3)))
		*flag |= BIT(FLAG_H);

	*b -= val;

	if (*b == 0)
		*flag |= BIT(FLAG_Z);
}

void
and_byte(BYTE *flag, BYTE *b, BYTE val)
{
	RESET_FLAGS(*flag, BIT(FLAG_C) | BIT(FLAG_N));
	*flag |= BIT(FLAG_H);

	*b &= val;

	if (*b == 0)
		*flag |= BIT(FLAG_Z);
}

void
xor_byte(BYTE *flag, BYTE *b, BYTE val)
{
	RESET_FLAGS(*flag, BIT(FLAG_C) | BIT(FLAG_H) | BIT(FLAG_N));

	*b ^= val;

	if (*b == 0)
		*flag |= BIT(FLAG_Z);
}

void
or_byte(BYTE *flag, BYTE *b, BYTE val)
{
	RESET_FLAGS(*flag, BIT(FLAG_C) | BIT(FLAG_H) | BIT(FLAG_N));

	*b |= val;

	if (*b == 0)
		*flag |= BIT(FLAG_Z);
}
void
cp_byte(BYTE *flag, BYTE b, BYTE val)
{
	sub_byte(flag, &b, val);
}

BYTE
read_byte(gb_cpu *cpu, WORD addr)
{
	return cpu->memory[addr];
}

WORD
read_word(gb_cpu *cpu, WORD addr)
{
	return *(WORD*)&cpu->memory[addr];
}

void
write_byte(gb_cpu *cpu, WORD addr, BYTE val)
{
	cpu->memory[addr] = val;
}

void
write_word(gb_cpu *cpu, WORD addr, WORD val)
{
	*(WORD*)&cpu->memory[addr] = val;
}

WORD
pop(gb_cpu *cpu)
{
	return (++cpu->stack)->reg;
}

void
push(gb_cpu *cpu, WORD val)
{
	(cpu->stack--)->reg = val;
}

void
call(gb_cpu *cpu, WORD addr)
{
	push(cpu, cpu->pc);
	cpu->pc = addr;
}

void
ret(gb_cpu *cpu)
{
	cpu->pc = pop(cpu);
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
	inc_byte(FLAG_P(cpu), &cpu->regs[REG_BC].hi);
}

/* DEC B */
void
op_0x05(gb_cpu *cpu)
{
	dec_byte(FLAG_P(cpu), &cpu->regs[REG_BC].hi);
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
	rot_byte(FLAG_P(cpu), &cpu->regs[REG_AF].hi, LEFT | CIRCULAR);
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
	add_word(FLAG_P(cpu), &cpu->regs[REG_HL].reg, cpu->regs[REG_BC].reg);
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
	inc_byte(FLAG_P(cpu), &cpu->regs[REG_BC].lo);
}

/* DEC C */
void
op_0x0D(gb_cpu *cpu)
{
	dec_byte(FLAG_P(cpu), &cpu->regs[REG_BC].lo);
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
	rot_byte(FLAG_P(cpu), &cpu->regs[REG_AF].hi, RIGHT | CIRCULAR);
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
	inc_byte(FLAG_P(cpu), &cpu->regs[REG_DE].hi);
}

/* DEC D */
void
op_0x15(gb_cpu *cpu)
{
	dec_byte(FLAG_P(cpu), &cpu->regs[REG_DE].hi);
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
	rot_byte(FLAG_P(cpu), &cpu->regs[REG_AF].hi, LEFT);
}

/* JR r8 */
void
op_0x18(gb_cpu *cpu, SIGNED_BYTE r8)
{
	cpu->pc += r8;
}

/* ADD HL,DE */
void
op_0x19(gb_cpu *cpu)
{
	add_word(FLAG_P(cpu), &cpu->regs[REG_HL].reg, cpu->regs[REG_DE].reg);
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
	inc_byte(FLAG_P(cpu), &cpu->regs[REG_DE].lo);
}

/* DEC E */
void
op_0x1D(gb_cpu *cpu)
{
	dec_byte(FLAG_P(cpu), &cpu->regs[REG_DE].lo);
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
	rot_byte(FLAG_P(cpu), &cpu->regs[REG_AF].hi, RIGHT);
}

/* JR NZ,r8 */
void
op_0x20(gb_cpu *cpu, SIGNED_BYTE r8)
{
	if (!(FLAG(cpu) & BIT(FLAG_Z)))
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
	inc_byte(FLAG_P(cpu), &cpu->regs[REG_HL].hi);
}

/* DEC H */
void
op_0x25(gb_cpu *cpu)
{
	dec_byte(FLAG_P(cpu), &cpu->regs[REG_HL].hi);
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
op_0x28(gb_cpu *cpu, SIGNED_BYTE r8)
{
	if (FLAG(cpu) & BIT(FLAG_Z))
		cpu->pc += r8;
}

/* ADD HL,HL */
void
op_0x29(gb_cpu *cpu)
{
	add_word(FLAG_P(cpu), &cpu->regs[REG_HL].reg, cpu->regs[REG_HL].reg);
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
	inc_byte(FLAG_P(cpu), &cpu->regs[REG_HL].lo);
}

/* DEC L */
void
op_0x2D(gb_cpu *cpu)
{
	dec_byte(FLAG_P(cpu), &cpu->regs[REG_HL].lo);
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

	FLAG(cpu) |= BIT(FLAG_N) | BIT(FLAG_H);
}

/* JR NC,r8 */
void op_0x30(gb_cpu *cpu, SIGNED_BYTE r8)
{
	if ((FLAG(cpu) & BIT(FLAG_C)))
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
void
op_0x34(gb_cpu *cpu)
{
	inc_byte(FLAG_P(cpu), &cpu->memory[cpu->regs[REG_HL].reg]);
}

/* DEC (HL) */
void
op_0x35(gb_cpu *cpu)
{
	dec_byte(FLAG_P(cpu), &cpu->memory[cpu->regs[REG_HL].reg]); 
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
	RESET_FLAGS(FLAG(cpu), BIT(FLAG_N) | BIT(FLAG_H));

	FLAG(cpu) |= BIT(FLAG_C);
}

/* JR C,r8 */ 
void
op_0x38(gb_cpu *cpu, SIGNED_BYTE r8)
{
	if (FLAG(cpu) & BIT(FLAG_C))
		cpu->pc += r8;
}

/* ADD HL,SP */
void
op_0x39(gb_cpu *cpu)
{
	add_word(FLAG_P(cpu), &cpu->regs[REG_HL].reg, cpu->stack->reg);
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
	inc_byte(FLAG_P(cpu), &cpu->regs[REG_AF].hi);
}

/* DEC A */
void
op_0x3D(gb_cpu *cpu)
{
	dec_byte(FLAG_P(cpu), &cpu->regs[REG_AF].hi);
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
	RESET_FLAGS(FLAG(cpu), BIT(FLAG_N) | BIT(FLAG_H));

	FLAG(cpu) ^= BIT(FLAG_C);
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

/* ADD A,B */
void
op_0x80(gb_cpu *cpu)
{
	add_byte(&cpu->regs[REG_AF].hi, &cpu->regs[REG_AF].hi, cpu->regs[REG_BC].hi);
}

/* ADD A,C */
void
op_0x81(gb_cpu *cpu)
{
	add_byte(&cpu->regs[REG_AF].hi, &cpu->regs[REG_AF].hi, cpu->regs[REG_BC].lo);
}

/* ADD A,D */
void
op_0x82(gb_cpu *cpu)
{
        add_byte(&cpu->regs[REG_AF].hi, &cpu->regs[REG_AF].hi, cpu->regs[REG_DE].hi);
}

/* ADD A,E */
void
op_0x83(gb_cpu *cpu)
{
        add_byte(&cpu->regs[REG_AF].hi, &cpu->regs[REG_AF].hi, cpu->regs[REG_DE].lo);
}

/* ADD A,H */
void
op_0x84(gb_cpu *cpu)
{
        add_byte(&cpu->regs[REG_AF].hi, &cpu->regs[REG_AF].hi, cpu->regs[REG_HL].hi);
}

/* ADD A,L */
void
op_0x85(gb_cpu *cpu)
{
        add_byte(&cpu->regs[REG_AF].hi, &cpu->regs[REG_AF].hi, cpu->regs[REG_HL].lo);
}

/* ADD A,(HL) */
void
op_0x86(gb_cpu *cpu)
{
        add_byte(&cpu->regs[REG_AF].hi, &cpu->regs[REG_AF].hi,
		 read_byte(cpu, cpu->regs[REG_HL].reg));
}

/* ADD A,A */
void
op_0x87(gb_cpu *cpu)
{
        add_byte(&cpu->regs[REG_AF].hi, &cpu->regs[REG_AF].hi, cpu->regs[REG_AF].hi);
}

/* ADC A,B */
void
op_0x88(gb_cpu *cpu)
{
        add_byte(FLAG_P(cpu), &cpu->regs[REG_AF].hi,
		 cpu->regs[REG_BC].hi + ((FLAG(cpu) & BIT(FLAG_C)) >> FLAG_C));
}

/* ADC A,C */
void
op_0x89(gb_cpu *cpu)
{
        add_byte(FLAG_P(cpu), &cpu->regs[REG_AF].hi,
                 cpu->regs[REG_BC].lo + ((FLAG(cpu) & BIT(FLAG_C)) >> FLAG_C));
}

/* ADC A,D */
void
op_0x8A(gb_cpu *cpu)
{
        add_byte(FLAG_P(cpu), &cpu->regs[REG_AF].hi,
                 cpu->regs[REG_DE].hi + ((FLAG(cpu) & BIT(FLAG_C)) >> FLAG_C));
}

/* ADC A,E */
void
op_0x8B(gb_cpu *cpu)
{
        add_byte(FLAG_P(cpu), &cpu->regs[REG_AF].hi,
                 cpu->regs[REG_DE].lo + ((FLAG(cpu) & BIT(FLAG_C)) >> FLAG_C));
}

/* ADC A,H */
void
op_0x8C(gb_cpu *cpu)
{
        add_byte(FLAG_P(cpu), &cpu->regs[REG_AF].hi,
                 cpu->regs[REG_HL].hi + ((FLAG(cpu) & BIT(FLAG_C)) >> FLAG_C));
}

/* ADC A,L */
void
op_0x8D(gb_cpu *cpu)
{
        add_byte(FLAG_P(cpu), &cpu->regs[REG_AF].hi,
                 cpu->regs[REG_HL].lo + ((FLAG(cpu) & BIT(FLAG_C)) >> FLAG_C));
}

/* ADC A,(HL) */
void
op_0x8E(gb_cpu *cpu)
{
        add_byte(FLAG_P(cpu), &cpu->regs[REG_AF].hi,
                 read_byte(cpu, cpu->regs[REG_HL].reg) + ((FLAG(cpu) & BIT(FLAG_C)) >> FLAG_C));
}

/* ADC A,A */
void
op_0x8F(gb_cpu *cpu)
{
        add_byte(FLAG_P(cpu), &cpu->regs[REG_AF].hi,
                 cpu->regs[REG_AF].hi + ((FLAG(cpu) & BIT(FLAG_C)) >> FLAG_C));
}

/* SUB B */
void
op_0x90(gb_cpu *cpu)
{
	sub_byte(FLAG_P(cpu), &cpu->regs[REG_AF].hi, cpu->regs[REG_BC].hi);
}

/* SUB C */
void
op_0x91(gb_cpu *cpu)
{
        sub_byte(FLAG_P(cpu), &cpu->regs[REG_AF].hi, cpu->regs[REG_BC].lo);
}

/* SUB D */
void
op_0x92(gb_cpu *cpu)
{
        sub_byte(FLAG_P(cpu), &cpu->regs[REG_AF].hi, cpu->regs[REG_DE].hi);
}

/* SUB E */
void
op_0x93(gb_cpu *cpu)
{
        sub_byte(FLAG_P(cpu), &cpu->regs[REG_AF].hi, cpu->regs[REG_DE].lo);
}

/* SUB H */
void
op_0x94(gb_cpu *cpu)
{
        sub_byte(FLAG_P(cpu), &cpu->regs[REG_AF].hi, cpu->regs[REG_HL].hi);
}

/* SUB L */
void
op_0x95(gb_cpu *cpu)
{
        sub_byte(FLAG_P(cpu), &cpu->regs[REG_AF].hi, cpu->regs[REG_HL].lo);
}

/* SUB (HL) */
void
op_0x96(gb_cpu *cpu)
{
	sub_byte(FLAG_P(cpu), &cpu->regs[REG_AF].hi,
		 read_byte(cpu, cpu->regs[REG_HL].reg));
}

/* SUB A */
void
op_0x97(gb_cpu *cpu)
{
        sub_byte(FLAG_P(cpu), &cpu->regs[REG_AF].hi, cpu->regs[REG_AF].hi);
}

/* SBC A,B */
void
op_0x98(gb_cpu *cpu)
{
        sub_byte(FLAG_P(cpu), &cpu->regs[REG_AF].hi,
		 cpu->regs[REG_BC].hi + ((FLAG(cpu) & BIT(FLAG_C)) << FLAG_C));
}

/* SBC A,C */
void
op_0x99(gb_cpu *cpu)
{
        sub_byte(FLAG_P(cpu), &cpu->regs[REG_AF].hi,
                 cpu->regs[REG_BC].lo + ((FLAG(cpu) & BIT(FLAG_C)) << FLAG_C));
}

/* SBC A,D */
void
op_0x9A(gb_cpu *cpu)
{
        sub_byte(FLAG_P(cpu), &cpu->regs[REG_AF].hi,
                 cpu->regs[REG_DE].hi + ((FLAG(cpu) & BIT(FLAG_C)) << FLAG_C));
}

/* SBC A,E */
void
op_0x9B(gb_cpu *cpu)
{
        sub_byte(FLAG_P(cpu), &cpu->regs[REG_AF].hi,
                 cpu->regs[REG_DE].lo + ((FLAG(cpu) & BIT(FLAG_C)) << FLAG_C));
}

/* SBC A,H */
void
op_0x9C(gb_cpu *cpu)
{
        sub_byte(FLAG_P(cpu), &cpu->regs[REG_AF].hi,
                 cpu->regs[REG_HL].hi + ((FLAG(cpu) & BIT(FLAG_C)) << FLAG_C));
}

/* SBC A,L */
void
op_0x9D(gb_cpu *cpu)
{
        sub_byte(FLAG_P(cpu), &cpu->regs[REG_AF].hi,
                 cpu->regs[REG_HL].lo + ((FLAG(cpu) & BIT(FLAG_C)) << FLAG_C));
}

/* SBC A,(HL) */
void
op_0x9E(gb_cpu *cpu)
{
        sub_byte(FLAG_P(cpu), &cpu->regs[REG_AF].hi,
                 read_byte(cpu, cpu->regs[REG_HL].reg) + ((FLAG(cpu) & BIT(FLAG_C)) << FLAG_C));
}

/* SBC A,A */
void
op_0x9F(gb_cpu *cpu)
{
        sub_byte(FLAG_P(cpu), &cpu->regs[REG_AF].hi,
                 cpu->regs[REG_AF].hi + ((FLAG(cpu) & BIT(FLAG_C)) << FLAG_C));
}

/* AND B */
void
op_0xA0(gb_cpu *cpu)
{
	and_byte(FLAG_P(cpu), &cpu->regs[REG_AF].hi, cpu->regs[REG_BC].hi);
}

/* AND C */
void
op_0xA1(gb_cpu *cpu)
{
        and_byte(FLAG_P(cpu), &cpu->regs[REG_AF].hi, cpu->regs[REG_BC].lo);
}

/* AND D */
void
op_0xA2(gb_cpu *cpu)
{
        and_byte(FLAG_P(cpu), &cpu->regs[REG_AF].hi, cpu->regs[REG_DE].hi);
}

/* AND E */
void
op_0xA3(gb_cpu *cpu)
{
        and_byte(FLAG_P(cpu), &cpu->regs[REG_AF].hi, cpu->regs[REG_DE].lo);
}

/* AND H */
void
op_0xA4(gb_cpu *cpu)
{
        and_byte(FLAG_P(cpu), &cpu->regs[REG_AF].hi, cpu->regs[REG_HL].hi);
}

/* AND L */
void
op_0xA5(gb_cpu *cpu)
{
        and_byte(FLAG_P(cpu), &cpu->regs[REG_AF].hi, cpu->regs[REG_HL].lo);
}

/* AND (HL) */
void
op_0xA6(gb_cpu *cpu)
{
        and_byte(FLAG_P(cpu), &cpu->regs[REG_AF].hi,
		 read_byte(cpu, cpu->regs[REG_HL].reg));
}

/* AND A */
void
op_0xA7(gb_cpu *cpu)
{
        and_byte(FLAG_P(cpu), &cpu->regs[REG_AF].hi, cpu->regs[REG_AF].hi);
}

/* XOR B */
void
op_0xA8(gb_cpu *cpu)
{
        xor_byte(FLAG_P(cpu), &cpu->regs[REG_AF].hi, cpu->regs[REG_BC].hi);
}

/* XOR C */
void
op_0xA9(gb_cpu *cpu)
{
        xor_byte(FLAG_P(cpu), &cpu->regs[REG_AF].hi, cpu->regs[REG_BC].lo);
}

/* XOR D */
void
op_0xAA(gb_cpu *cpu)
{
        xor_byte(FLAG_P(cpu), &cpu->regs[REG_AF].hi, cpu->regs[REG_DE].hi);
}

/* XOR E */
void
op_0xAB(gb_cpu *cpu)
{
        xor_byte(FLAG_P(cpu), &cpu->regs[REG_AF].hi, cpu->regs[REG_DE].lo);
}

/* XOR H */
void
op_0xAC(gb_cpu *cpu)
{
        xor_byte(FLAG_P(cpu), &cpu->regs[REG_AF].hi, cpu->regs[REG_HL].hi);
}

/* XOR L */
void
op_0xAD(gb_cpu *cpu)
{
        xor_byte(FLAG_P(cpu), &cpu->regs[REG_AF].hi, cpu->regs[REG_HL].lo);
}

/* XOR (HL) */
void
op_0xAE(gb_cpu *cpu)
{
        xor_byte(FLAG_P(cpu), &cpu->regs[REG_AF].hi,
		 read_byte(cpu, cpu->regs[REG_HL].reg));
}

/* XOR A */
void
op_0xAF(gb_cpu *cpu)
{
        xor_byte(FLAG_P(cpu), &cpu->regs[REG_AF].hi, cpu->regs[REG_AF].hi);
}

/* OR B */
void
op_0xB0(gb_cpu *cpu)
{
        or_byte(FLAG_P(cpu), &cpu->regs[REG_AF].hi, cpu->regs[REG_BC].hi);
}

/* OR C */
void
op_0xB1(gb_cpu *cpu)
{
        or_byte(FLAG_P(cpu), &cpu->regs[REG_AF].hi, cpu->regs[REG_BC].lo);
}

/* OR D */
void
op_0xB2(gb_cpu *cpu)
{
        or_byte(FLAG_P(cpu), &cpu->regs[REG_AF].hi, cpu->regs[REG_DE].hi);
}

/* OR E */
void
op_0xB3(gb_cpu *cpu)
{
        or_byte(FLAG_P(cpu), &cpu->regs[REG_AF].hi, cpu->regs[REG_DE].lo);
}

/* OR H */
void
op_0xB4(gb_cpu *cpu)
{
        or_byte(FLAG_P(cpu), &cpu->regs[REG_AF].hi, cpu->regs[REG_HL].hi);
}

/* OR L */
void
op_0xB5(gb_cpu *cpu)
{
        or_byte(FLAG_P(cpu), &cpu->regs[REG_AF].hi, cpu->regs[REG_HL].lo);
}

/* OR (HL) */
void
op_0xB6(gb_cpu *cpu)
{
        or_byte(FLAG_P(cpu), &cpu->regs[REG_AF].hi,
		read_byte(cpu, cpu->regs[REG_HL].reg));
}

/* OR A */
void
op_0xB7(gb_cpu *cpu)
{
        or_byte(FLAG_P(cpu), &cpu->regs[REG_AF].hi, cpu->regs[REG_AF].hi);
}

/* CP B */
void
op_0xB8(gb_cpu *cpu)
{
        cp_byte(FLAG_P(cpu), cpu->regs[REG_AF].hi, cpu->regs[REG_BC].hi);
}

/* CP C */
void
op_0xB9(gb_cpu *cpu)
{
        cp_byte(FLAG_P(cpu), cpu->regs[REG_AF].hi, cpu->regs[REG_BC].lo);
}

/* CP D */
void
op_0xBA(gb_cpu *cpu)
{
        cp_byte(FLAG_P(cpu), cpu->regs[REG_AF].hi, cpu->regs[REG_DE].hi);
}

/* CP E */
void
op_0xBB(gb_cpu *cpu)
{
        cp_byte(FLAG_P(cpu), cpu->regs[REG_AF].hi, cpu->regs[REG_DE].lo);
}

/* CP H */
void
op_0xBC(gb_cpu *cpu)
{
        cp_byte(FLAG_P(cpu), cpu->regs[REG_AF].hi, cpu->regs[REG_HL].hi);
}

/* CP L */
void
op_0xBD(gb_cpu *cpu)
{
        cp_byte(FLAG_P(cpu), cpu->regs[REG_AF].hi, cpu->regs[REG_HL].lo);
}

/* CP (HL) */
void
op_0xBE(gb_cpu *cpu)
{
        cp_byte(FLAG_P(cpu), cpu->regs[REG_AF].hi,
		read_byte(cpu, cpu->regs[REG_HL].reg));
}

/* CP A */
void
op_0xBF(gb_cpu *cpu)
{
        cp_byte(FLAG_P(cpu), cpu->regs[REG_AF].hi, cpu->regs[REG_AF].hi);
}

/* RET NZ */
void
op_0xC0(gb_cpu *cpu)
{
	if (!(FLAG(cpu) & BIT(FLAG_Z)))
		ret(cpu);
}

/* POP BC */
void
op_0xC1(gb_cpu *cpu)
{
	cpu->regs[REG_BC].reg = pop(cpu);
}

/* JP NZ,a16 */
void
op_0xC2(gb_cpu *cpu, WORD a16)
{
	if (!(FLAG(cpu) & BIT(FLAG_Z)))
		cpu->pc = a16;
}

/* JP a16 */
void
op_0xC3(gb_cpu *cpu, WORD a16)
{
	cpu->pc = a16;
}

/* CALL NZ,a16 */
void
op_0xC4(gb_cpu *cpu, WORD a16)
{
	if (!(FLAG(cpu) & BIT(FLAG_Z)))
		call(cpu, a16);
}

/* PUSH BC */
void
op_0xC5(gb_cpu *cpu)
{
	push(cpu, cpu->regs[REG_BC].reg);
}

/* ADD A,d8 */
void
op_0xC6(gb_cpu *cpu, BYTE d8)
{
	add_byte(FLAG_P(cpu), &cpu->regs[REG_AF].hi, d8);
}

/* RST 00H */
void
op_0xC7(gb_cpu *cpu)
{
	push(cpu, cpu->pc);
	cpu->pc = 0x00;
}

/* RET Z */
void
op_0xC8(gb_cpu *cpu)
{
	if (FLAG(cpu) & BIT(FLAG_Z))
		cpu->pc = pop(cpu);
}

/* RET */
void
op_0xC9(gb_cpu *cpu)
{
	cpu->pc = pop(cpu);
}

/* JP Z,a16 */
void
op_0xCA(gb_cpu *cpu, WORD a16)
{
	if (FLAG(cpu) & BIT(FLAG_Z))
		cpu->pc = a16;
}

/* PREFIX CB */
void
op_0xCB(gb_cpu *cpu)
{
	IMPLEMENT("PREFIX CB");
}

/* CALL Z,a16 */
void
op_0xCC(gb_cpu *cpu, WORD a16)
{
	if (FLAG(cpu) & BIT(FLAG_Z))
		call(cpu, a16);
}

/* CALL a16 */
void
op_0xCD(gb_cpu *cpu, WORD a16)
{
	call(cpu, a16);
}

/* ADC A,d8 */
void
op_0xCE(gb_cpu *cpu, BYTE d8)
{
	add_byte(FLAG_P(cpu), &cpu->regs[REG_AF].hi,
		 d8 + ((FLAG(cpu) & BIT(FLAG_C)) >> FLAG_C));
}

/* RST 08H */
void
op_0xCF(gb_cpu *cpu)
{
	push(cpu, cpu->pc);
	cpu->pc = 0x08;
}

/* RET NC */
void
op_0xD0(gb_cpu *cpu)
{
	if (!(FLAG(cpu) & BIT(FLAG_C)))
		cpu->pc = pop(cpu);
}

/* POP DE */
void
op_0xD1(gb_cpu *cpu)
{
	cpu->regs[REG_DE].reg = pop(cpu);
}

/* JP NC,a16 */
void
op_0xD2(gb_cpu *cpu, WORD a16)
{
	if (!(FLAG(cpu) & BIT(FLAG_C)))
		cpu->pc = a16;
}

/* CALL NC,a16 */
void
op_0xD4(gb_cpu *cpu, WORD a16)
{
	if (!(FLAG(cpu) & BIT(FLAG_C)))
		call(cpu, a16);
}

/* PUSH DE */
void
op_0xD5(gb_cpu *cpu)
{
	push(cpu, cpu->regs[REG_DE].reg);
}

/* SUB d8 */
void
op_0xD6(gb_cpu *cpu, BYTE d8)
{
	sub_byte(FLAG_P(cpu), &cpu->regs[REG_AF].hi, d8);
}

/* RST 10H */
void
op_0xD7(gb_cpu *cpu)
{
	push(cpu, cpu->pc);
	cpu->pc = 0x10;
}

/* RET C */
void
op_0xD8(gb_cpu *cpu)
{
	if (FLAG(cpu) & BIT(FLAG_C))
		ret(cpu);
}

/* RETI */
void
op_0xD9(gb_cpu *cpu)
{
	cpu->ime = 1;
	ret(cpu);
}

/* JP C,a16 */
void
op_0xDA(gb_cpu *cpu, WORD a16)
{
	if (FLAG(cpu) & BIT(FLAG_C))
		cpu->pc = a16;
}

/* CALL C,a16 */
void
op_0xDC(gb_cpu *cpu, WORD a16)
{
	if (FLAG(cpu) & BIT(FLAG_C))
		call(cpu, a16);
}

/* SBC A,d8 */
void
op_0xDE(gb_cpu *cpu, BYTE d8)
{
	sub_byte(FLAG_P(cpu), &cpu->regs[REG_AF].hi,
		 d8 + ((FLAG(cpu) & BIT(FLAG_C)) >> FLAG_C));
}

/* RST 18H */
void
op_0xDF(gb_cpu *cpu)
{
	push(cpu, cpu->pc);
	cpu->pc = 0x18;
}

/* LDH (a8),A */
void
op_0xE0(gb_cpu *cpu, BYTE a8)
{
	write_byte(cpu, 0xFF00 + a8, cpu->regs[REG_AF].hi);
}

/* POP HL */
void
op_0xE1(gb_cpu *cpu)
{
	cpu->regs[REG_HL].reg = pop(cpu);
}

/* LD (C),A */
void
op_0xE2(gb_cpu *cpu)
{
	write_byte(cpu, 0xFF00 + cpu->regs[REG_BC].lo, cpu->regs[REG_AF].hi);
}

/* PUSH HL */
void
op_0xE5(gb_cpu *cpu)
{
	push(cpu, cpu->regs[REG_HL].reg);
}

/* AND d8 */
void
op_0xE6(gb_cpu *cpu, BYTE d8)
{
	and_byte(FLAG_P(cpu), &cpu->regs[REG_AF].hi, d8);
}

/* RST 20H */
void
op_0xE7(gb_cpu *cpu)
{
	push(cpu, cpu->pc);
	cpu->pc = 0x20;
}

/* ADD SP,r8 */
/* add to the top of the stack or the stack pointer itself? */
void
op_0xE8(gb_cpu *cpu, SIGNED_BYTE r8)
{
	add_word(FLAG_P(cpu), &cpu->stack->reg, r8);
}

/* JP (HL) */
void
op_0xE9(gb_cpu *cpu)
{
	cpu->pc = read_word(cpu, cpu->regs[REG_HL].reg);
}

/* LD (a16),A */
void
op_0xEA(gb_cpu *cpu, WORD a16)
{
	write_byte(cpu, a16, cpu->regs[REG_AF].hi);
}

/* XOR d8 */
void
op_0xEE(gb_cpu *cpu, BYTE d8)
{
	xor_byte(FLAG_P(cpu), &cpu->regs[REG_AF].hi, d8);
}

/* RST 28H */
void
op_0xEF(gb_cpu *cpu)
{
	push(cpu, cpu->pc);
	cpu->pc = 0x28;
}

/* LDH A,a8 */
void
op_0xF0(gb_cpu *cpu, BYTE a8)
{
	cpu->regs[REG_AF].hi = read_byte(cpu, 0xFF00 + a8);
}

/* POP AF */
void
op_0xF1(gb_cpu *cpu)
{
	cpu->regs[REG_AF].reg = pop(cpu);
}

/* LD A,(C) */
void
op_0xF2(gb_cpu *cpu)
{
	cpu->regs[REG_AF].hi = read_byte(cpu, 0xFF00 + cpu->regs[REG_BC].lo);
}

/* DI */
void
op_0xF3(gb_cpu *cpu)
{
	cpu->ime = 0;
}

/* PUSH AF */
void
op_0xF5(gb_cpu *cpu)
{
	push(cpu, cpu->regs[REG_AF].reg);
}

/* OR d8 */
void
op_0xF6(gb_cpu *cpu, BYTE d8)
{
	or_byte(FLAG_P(cpu), &cpu->regs[REG_AF].hi, d8);
}

/* RST 30H */
void
op_0xF7(gb_cpu *cpu)
{
	push(cpu, cpu->pc);
	cpu->pc = 0x30;
}

/* LD HL,SP+r8 */
void
op_0xF8(gb_cpu *cpu, BYTE r8)
{
	IMPLEMENT("LD HL,SP+r8");
}

/* LD SP,HL */
void
op_0xF9(gb_cpu *cpu)
{
	cpu->stack->reg = cpu->regs[REG_HL].reg;
}

/* LD A,(a16) */
void
op_0xFA(gb_cpu *cpu, WORD a16)
{
	cpu->regs[REG_AF].hi = read_byte(cpu, a16);
}

/* EI */
void
op_0xFB(gb_cpu *cpu)
{
	cpu->ime = 1;
}

/* CP d8 */
void
op_0xFE(gb_cpu *cpu, BYTE d8)
{
	cp_byte(FLAG_P(cpu), cpu->regs[REG_AF].hi, d8);
}

/* RST 38H */
void
op_0xFF(gb_cpu *cpu)
{
	push(cpu, cpu->pc);
	cpu->pc = 0x38;
}

