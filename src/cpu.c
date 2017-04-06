/* Header file */
#include <linuxboy.h>
/* C library */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

const color colors[MAX_COLORS] = { {   8,  24,  32 },	/* BLACK */
				   {  52, 104,  86 },	/* DARK_GRAY */
				   { 136, 192, 112 },	/* LIGHT_GRAY */
				   { 224, 248, 208 } };	/* WHITE */

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
			{ "PREFIX CB", 1, op_0xCB },
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

const op ext_ops[0x100] = { { "RLC B", 2, ext_op_0x00 },
			    { "RLC C", 2, ext_op_0x01 },
			    { "RLC D", 2, ext_op_0x02 },
			    { "RLC E", 2, ext_op_0x03 },
			    { "RLC H", 2, ext_op_0x04 },
			    { "RLC L", 2, ext_op_0x05 },
			    { "RLC (HL)", 2, ext_op_0x06 },
			    { "RLC A", 2, ext_op_0x07 },
			    { "RRC B", 2, ext_op_0x08 },
			    { "RRC C", 2, ext_op_0x09 },
			    { "RRC D", 2, ext_op_0x0A },
			    { "RRC E", 2, ext_op_0x0B },
			    { "RRC H", 2, ext_op_0x0C },
			    { "RRC L", 2, ext_op_0x0D },
			    { "RRC (HL)", 2, ext_op_0x0E },
			    { "RRC A", 2, ext_op_0x0F },
			    
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
			    { "SWAP A", 2, ext_op_0x37 },
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
load_rom(gb_cpu *cpu)
{
	FILE *fp;
	int size;
	
	if ((fp = fopen(cpu->rom_path, "rb")) == NULL) {
		fprintf(stderr, "Failed to open file %s: %s\n", cpu->rom_path, strerror(errno));
		return 0;
	}
	
	fseek(fp, 0, SEEK_END);
	size = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	
	if (fread(cpu->rom, 1, size, fp) != size) {
		fprintf(stderr, "Failed reading file %s: %s\n", cpu->rom_path, strerror(errno));
		fclose(fp);
		return 0;
	}
	
	fprintf(stderr, "Successfully read %d bytes\n", size);
	cpu->rom_size = size; /* Can also read rom[0x148] to get rom size */
	
	fclose(fp);
	return 1;
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
	
	/* Initialize registers */
	cpu->regs[REG_AF].reg = 0x01B0;
	cpu->regs[REG_BC].reg = 0x0013;
	cpu->regs[REG_DE].reg = 0x00D8;
	cpu->regs[REG_HL].reg = 0x014D;
	
	/* Initialize memory */
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
	
	/* Enable interrupts */
	cpu->ime = 1;
	
	/* Initialize timers */
	cpu->divider_counter = CLOCK_RATE / 16384;
	cpu->timer_counter = CLOCK_RATE / 4096;
	
	/* Initialize GPU */
	init_gpu(&cpu->gpu, SCREEN_WIDTH, SCREEN_HEIGHT);
	
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
		printf("Missing implementation %s at $%04x\n", opcode->assembly, cpu->pc);
		exit(1);
	}
	
	if (opcode->arg_size & SIGNED) {
		cpu->pc += opcode->arg_size - SIGNED;
		
		switch (opcode->arg_size - SIGNED) {
		case 1:
			return ((int(*)(gb_cpu*))opcode->func)(cpu);
		case 2:
			return ((int(*)(gb_cpu*, SIGNED_BYTE))opcode->func)(cpu, *(SIGNED_BYTE*)arg);
		case 3:
			return ((int(*)(gb_cpu*, SIGNED_WORD))opcode->func)(cpu, *(SIGNED_WORD*)arg);
		}
	} else {
		cpu->pc += opcode->arg_size;
		
		switch (opcode->arg_size) {
		case 1:
			return ((int(*)(gb_cpu*))opcode->func)(cpu);
		case 2:
			return ((int(*)(gb_cpu*, BYTE))opcode->func)(cpu, *(BYTE*)arg);
		case 3:
			return ((int(*)(gb_cpu*, WORD))opcode->func)(cpu, *(WORD*)arg);
		}
	}
	
	return 0;
}

void
dma_transfer(gb_cpu *cpu, BYTE val)
{
	WORD address = val * 100;
	
	for (int i = 0; i != 0xA0; ++i)
		write_byte(cpu, 0xFE00 + i,
			   read_byte(cpu, address));
}

void
set_frequency(gb_cpu *cpu)
{
	switch (read_byte(cpu, TMC) & 0x3) {
	case 0:
		cpu->timer_counter = CLOCK_RATE / 4096;
		break;
	case 1:
		cpu->timer_counter = CLOCK_RATE / 262144;
		break;
	case 2:
		cpu->timer_counter = CLOCK_RATE / 65536;
		break;
	case 3:
		cpu->timer_counter = CLOCK_RATE / 16384;
		break;
	}
}

void
divider_register(gb_cpu *cpu, int ops)
{
	if ((cpu->divider_counter += ops) > 255) {
		cpu->divider_counter = 0;
		++cpu->memory[DIVIDER_REGISTER];
	}
}

void
update_timers(gb_cpu *cpu, int ops)
{
	/* UPDATE DIVIDER REGISTER */
	divider_register(cpu, ops);
	
	/* UPDATE TIMER */
	if (read_byte(cpu, TMC) & BIT(2)) {
		if ((cpu->timer_counter -= ops) <= 0) {
			set_frequency(cpu);
			
			if (read_byte(cpu, TIMA) == 255) {
				write_byte(cpu, TIMA,
					   read_byte(cpu, TMA));
				request_interrupt(cpu, TIMER);
			} else {
				write_byte(cpu, TIMA,
					   read_byte(cpu, TIMA) + 1);
			}
		}
	}
}

void
set_lcd_status(gb_cpu *cpu)
{
	BYTE status;
	BYTE prev_mode;
	BYTE scanline;
	
	prev_mode = (status = read_byte(cpu, LCD_STATUS)) & 0x3;
	scanline = read_byte(cpu, CURR_SCANLINE);
	if (read_byte(cpu, LCD_CONTROL) & BIT(7)) {
		if (scanline > 144) {
			/* MODE 1 */
			status = ((status >> 2) << 2) | 0x1;
			prev_mode |= BIT(3);
		} else {
			if (cpu->scanline_counter > 456 - 80) {
				/* MODE 2 */
				status = ((status >> 2) << 2) | 0x2;
				prev_mode |= BIT(3);
			} else if (cpu->scanline_counter > 456 - 252) {
				/* MODE 3 */
				status = ((status >> 2) << 2) | 0x3;
			} else {
				/* MODE 0 */
				status = (status >> 2) << 2;
				prev_mode |= BIT(3);
			}
		}

		if (prev_mode & BIT(3) && ((prev_mode & 0x3) != (status & 0x3)))
			request_interrupt(cpu, LCD_STAT);

		if (read_byte(cpu, TARGET_SCANLINE) == scanline) {
			status |= BIT(2);
			if (status & BIT(6))
				request_interrupt(cpu, LCD_STAT);
		} else {
			status &= ~BIT(2);
		}
	} else {
		status = ((status >> 2) << 2) | 0x1;
		cpu->scanline_counter = 456;
		write_byte(cpu, CURR_SCANLINE, 0);
	}
	
	write_byte(cpu, LCD_STATUS, status);
}

void
update_graphics(gb_cpu *cpu, int ops)
{
	BYTE scanline;
	
	set_lcd_status(cpu);
	
	if (read_byte(cpu, LCD_CONTROL) & BIT(7)) {
		if ((cpu->scanline_counter -= ops) <= 0) {
			++cpu->memory[CURR_SCANLINE];
			scanline = read_byte(cpu, CURR_SCANLINE);
			cpu->scanline_counter = 456;
			
			if (scanline == 144) {
				request_interrupt(cpu, VBLANK);
			} else if (scanline == 153) {
				cpu->memory[CURR_SCANLINE] = 0;
			} else if (scanline < 144) {
				/* TODO: DRAW SCANLINE */
			}
		}
	}
}

void
init_gpu(gb_gpu *gpu, int width, int height)
{
	/* Quad vertices */
	static const GLfloat vertex_data[] = { -1.0f,  1.0f, 1.0f, 0.0f, 1.0f,
					       -1.0f, -1.0f, 1.0f, 0.0f, 0.0f,
					       1.0f, -1.0f, 1.0f, 1.0f, 0.0f,
					       1.0f, -1.0f, 1.0f, 1.0f, 0.0f,
					       1.0f,  1.0f, 1.0f, 1.0f, 1.0f,
					       -1.0f,  1.0f, 1.0f, 0.0f, 1.0f };
	
	/* Setup screen properties */
	gpu->screen_width = width;
	gpu->screen_height = height;
	gpu->screen_data = (GLubyte*)malloc(width * height * 3);
	
	/* float scale = 255.0f / width; */
	for (int i = 0; i != width * height; ++i) {
		/*
		  Testing
		gpu->screen_data[i * 3] = i % width * scale;
		gpu->screen_data[i * 3 + 1] = i % width * scale;
		gpu->screen_data[i * 3 + 2] = i % width * scale;
		*/

		gpu->screen_data[i * 3] = colors[WHITE].r;
		gpu->screen_data[i * 3 + 1] = colors[WHITE].g;
		gpu->screen_data[i * 3 + 2] = colors[WHITE].b;
	}
	
	/* Basic shaders */
	gpu->shader_program = compile_program("res/vertex.glsl", "res/fragment.glsl");
	
	/* Screen quad */
	glGenVertexArrays(1, &gpu->vao);
	glGenBuffers(1, &gpu->vbo);
	
	glBindVertexArray(gpu->vao);
	
	glBindBuffer(GL_ARRAY_BUFFER, gpu->vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data, GL_STATIC_DRAW);
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, (GLvoid*)(sizeof(GLfloat) * 3));
	glEnableVertexAttribArray(1);
	
	glBindVertexArray(0);
	
	/* Display texture */
	glGenTextures(1, &gpu->texture);
	glBindTexture(GL_TEXTURE_2D, gpu->texture);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, gpu->screen_data);
	
	glBindTexture(GL_TEXTURE_2D, 0);
}

void
free_gpu(gb_gpu *gpu)
{
	free(gpu->screen_data);
}

void
draw(gb_gpu *gpu)
{
	glUseProgram(gpu->shader_program);
	
	glBindTexture(GL_TEXTURE_2D, gpu->texture);
	glBindVertexArray(gpu->vao);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	
	glUseProgram(0);
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
	
	BYTE flag;
	
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
	
	/* Stack pointer */
	printf("\nsp(%04x): %04x\n", (WORD)((BYTE*)cpu->stack - cpu->memory), cpu->stack->reg);
	
	/* Pretty flag */
	flag = cpu->regs[REG_AF].lo;
	printf("\nflags: %c%c%c%c",
	       flag & BIT(FLAG_Z) ? 'Z' : '.',
	       flag & BIT(FLAG_N) ? 'N' : '.',
	       flag & BIT(FLAG_H) ? 'H' : '.',
	       flag & BIT(FLAG_C) ? 'C' : '.');
	
	/* Print registers */
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

	/* Print other stuff */
	printf("\nie: %02x"
	       "\nif: %02x"
	       "\nime: %d\n\n",
	       cpu->memory[IE],
	       cpu->memory[IF],
	       cpu->ime);
}

/*
 * ---=== OPCODE HELPER FUNCTIONS ===---
 */

/* DOESN'T MODIFY GAME MEMORY */
BYTE
inc_byte(BYTE *flag, BYTE b)
{
	RESET_FLAGS(*flag, BIT(FLAG_Z) | BIT(FLAG_N) | BIT(FLAG_H));
	
	if ((BYTE)(b + 1) == 0)
		*flag |= BIT(FLAG_Z);
	
	/* Half-carry if bit 4 changed (not sure) */
	if ((b ^ (BYTE)(b + 1)) & BIT(4))
		*flag |= BIT(FLAG_H);
	
	return b + 1;
}

/* DOESN'T MODIFY GAME MEMORY */
BYTE
dec_byte(BYTE *flag, BYTE b)
{
	RESET_FLAGS(*flag, BIT(FLAG_Z) | BIT(FLAG_H));
	*flag |= BIT(FLAG_N);
	
	if ((BYTE)(b - 1) == 0)
		*flag |= BIT(FLAG_Z);
	
	/* Half-carry if lower nibble is 0 */
	if (!(b & 0x0F))
		*flag |= BIT(FLAG_H);
	
	return b - 1;
}

void
swap_byte(BYTE *flag, BYTE *b)
{
	BYTE nibble;
	
	nibble = *b & 0x0F;
	*b = (*b >> 4) | (nibble << 4);
	
	if (*b == 0)
		*flag |= BIT(FLAG_Z);
}

void
rot_byte(BYTE *flag, BYTE *b, BYTE rot_flags)
{
	BYTE bit;
	
	RESET_FLAGS(*flag, BIT(FLAG_C) | BIT(FLAG_H) | BIT(FLAG_N) | BIT(FLAG_Z)); /* ? */
	
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
	
	if (rot_flags & BIT(FLAG_Z) && *b == 0)
		*flag |= BIT(FLAG_Z);
	
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
	RESET_FLAGS(*flag, BIT(FLAG_C) | BIT(FLAG_N) | BIT(FLAG_Z));
	*flag |= BIT(FLAG_H);
	
	*b &= val;
	
	if (*b == 0)
		*flag |= BIT(FLAG_Z);
}

void
xor_byte(BYTE *flag, BYTE *b, BYTE val)
{
	RESET_FLAGS(*flag, BIT(FLAG_C) | BIT(FLAG_H) | BIT(FLAG_N) | BIT(FLAG_Z));
	
	*b ^= val;
	
	if (*b == 0)
		*flag |= BIT(FLAG_Z);
}

void
or_byte(BYTE *flag, BYTE *b, BYTE val)
{
	RESET_FLAGS(*flag, BIT(FLAG_C) | BIT(FLAG_H) | BIT(FLAG_N) | BIT(FLAG_Z));
	
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
	if (addr < 0x8000) {
		/* ROM */
	} else if (addr >= 0xE000 && addr < 0xFE00) {
		/* ECHO memory */
		cpu->memory[addr] = val;
		write_byte(cpu, addr - 0x2000, val);
	} else if (addr >= 0xFEA0 && addr < 0xFF00) {
		/* Not usable */
	} else if (addr == DIVIDER_REGISTER) {
		/* Writing to the divider register resets it */
		cpu->memory[addr] = 0;
	} else if (addr == 0xFF46) {
		dma_transfer(cpu, val);
	} else {
		cpu->memory[addr] = val;
	}
}

void
write_word(gb_cpu *cpu, WORD addr, WORD val)
{
	if (addr < 0x8000) {
		printf("poop\n");
		getchar();
		exit(1);
	}
	
	*(WORD*)&cpu->memory[addr] = val;
}

WORD
pop(gb_cpu *cpu)
{
	return (cpu->stack++)->reg;
}

void
push(gb_cpu *cpu, WORD val)
{
	(--cpu->stack)->reg = val;
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
int
op_0x00(gb_cpu *cpu)
{
	return 4;
}

/* LD BC,d16 */
int
op_0x01(gb_cpu *cpu, WORD d16)
{
	cpu->regs[REG_BC].reg = d16;
	
	return 12;
}

/* LD (BC),A */
int
op_0x02(gb_cpu *cpu)
{
	write_byte(cpu, cpu->regs[REG_BC].reg, cpu->regs[REG_AF].hi);
	
	return 8;
}

/* INC BC */
int
op_0x03(gb_cpu *cpu)
{
	++cpu->regs[REG_BC].reg;
	
	return 8;
}

/* INC B */
int
op_0x04(gb_cpu *cpu)
{
	cpu->regs[REG_BC].hi = inc_byte(FLAG_P(cpu), cpu->regs[REG_BC].hi);
	
	return 4;
}

/* DEC B */
int
op_0x05(gb_cpu *cpu)
{
	cpu->regs[REG_BC].hi = dec_byte(FLAG_P(cpu), cpu->regs[REG_BC].hi);
	
	return 4;
}

/* LD B,d8 */
int
op_0x06(gb_cpu *cpu, BYTE d8)
{
	cpu->regs[REG_BC].hi = d8;
	
	return 8;
}

/* RLCA */
int
op_0x07(gb_cpu *cpu)
{
	rot_byte(FLAG_P(cpu), &cpu->regs[REG_AF].hi, LEFT | CIRCULAR);
	
	return 4;
}

/* LD (a16),SP */
int
op_0x08(gb_cpu *cpu, WORD a16)
{
	write_byte(cpu, a16, cpu->stack->lo);
	write_byte(cpu, a16 + 1, cpu->stack->hi);
	
	return 20;
}

/* ADD HL,BC */
int
op_0x09(gb_cpu *cpu)
{
	add_word(FLAG_P(cpu), &cpu->regs[REG_HL].reg, cpu->regs[REG_BC].reg);
	
	return 8;
}

/* LD A,(BC) */
int
op_0x0A(gb_cpu *cpu)
{
	cpu->regs[REG_AF].hi = read_byte(cpu, cpu->regs[REG_BC].reg);
	
	return 8;
}

/* DEC BC */
int
op_0x0B(gb_cpu *cpu)
{
	--cpu->regs[REG_BC].reg;
	
	return 8;
}

/* INC C */
int
op_0x0C(gb_cpu *cpu)
{
	cpu->regs[REG_BC].lo = inc_byte(FLAG_P(cpu), cpu->regs[REG_BC].lo);
	
	return 4;
}

/* DEC C */
int
op_0x0D(gb_cpu *cpu)
{
	cpu->regs[REG_BC].lo = dec_byte(FLAG_P(cpu), cpu->regs[REG_BC].lo);
	
	return 4;
}

/* LD C,d8 */
int
op_0x0E(gb_cpu *cpu, BYTE d8)
{
	cpu->regs[REG_BC].lo = d8;
	
	return 8;
}

/* RRCA */
int
op_0x0F(gb_cpu *cpu)
{
	rot_byte(FLAG_P(cpu), &cpu->regs[REG_AF].hi, RIGHT | CIRCULAR);
	
	return 4;
}

/* STOP 0 */
int
op_0x10(gb_cpu *cpu, BYTE d8)
{
	IMPLEMENT("STOP 0");
	
	return 4;
}

/* LD DE,d16 */
int
op_0x11(gb_cpu *cpu, WORD d16)
{
	cpu->regs[REG_DE].reg = d16;
	
	return 12;
}

/* LD (DE),A */
int
op_0x12(gb_cpu *cpu)
{
	write_byte(cpu, cpu->regs[REG_DE].reg, cpu->regs[REG_AF].hi);
	
	return 8;
}

/* INC DE */
int
op_0x13(gb_cpu *cpu)
{
	++cpu->regs[REG_DE].reg;
	
	return 8;
}

/* INC D */
int

op_0x14(gb_cpu *cpu)
{
	cpu->regs[REG_DE].hi = inc_byte(FLAG_P(cpu), cpu->regs[REG_DE].hi);
	
	return 4;
}

/* DEC D */
int
op_0x15(gb_cpu *cpu)
{
	cpu->regs[REG_DE].hi = dec_byte(FLAG_P(cpu), cpu->regs[REG_DE].hi);
	
	return 4;
}

/* LD D,d8 */
int
op_0x16(gb_cpu *cpu, BYTE d8)
{
	cpu->regs[REG_DE].hi = d8;
	
	return 8;
}

/* RLA */
int
op_0x17(gb_cpu *cpu)
{
	rot_byte(FLAG_P(cpu), &cpu->regs[REG_AF].hi, LEFT);
	
	return 4;
}

/* JR r8 */
int
op_0x18(gb_cpu *cpu, SIGNED_BYTE r8)
{
	cpu->pc += r8;
	
	return 12;
}

/* ADD HL,DE */
int
op_0x19(gb_cpu *cpu)
{
	add_word(FLAG_P(cpu), &cpu->regs[REG_HL].reg, cpu->regs[REG_DE].reg);
	
	return 8;
}

/* LD A,(DE) */
int
op_0x1A(gb_cpu *cpu)
{
	cpu->regs[REG_AF].hi = read_byte(cpu, cpu->regs[REG_DE].reg);
	
	return 8;
}

/* DEC DE */
int
op_0x1B(gb_cpu *cpu)
{
	--cpu->regs[REG_DE].reg;
	
	return 8;
}

/* INC E */
int
op_0x1C(gb_cpu *cpu)
{
	cpu->regs[REG_DE].lo = inc_byte(FLAG_P(cpu), cpu->regs[REG_DE].lo);
	
	return 4;
}

/* DEC E */
int
op_0x1D(gb_cpu *cpu)
{
	cpu->regs[REG_DE].lo = dec_byte(FLAG_P(cpu), cpu->regs[REG_DE].lo);
	
	return 4;
}

/* LD E,d8 */
int
op_0x1E(gb_cpu *cpu, BYTE d8)
{
	cpu->regs[REG_DE].lo = d8;
	
	return 8;
}

/* RRA */
int
op_0x1F(gb_cpu *cpu)
{
	rot_byte(FLAG_P(cpu), &cpu->regs[REG_AF].hi, RIGHT);
	
	return 4;
}

/* JR NZ,r8 */
int
op_0x20(gb_cpu *cpu, SIGNED_BYTE r8)
{
	if (!(FLAG(cpu) & BIT(FLAG_Z))) {
		cpu->pc += r8;
		return 12;
	}
	
	return 8;
}

/* LD HL,d16 */
int op_0x21(gb_cpu *cpu, WORD d16)
{
	cpu->regs[REG_HL].reg = d16;
	
	return 12;
}

/* LD (HL+),A */
int op_0x22(gb_cpu *cpu)
{
	write_byte(cpu, cpu->regs[REG_HL].reg++, cpu->regs[REG_AF].hi);
	
	return 8;
}

/* INC HL */
int
op_0x23(gb_cpu *cpu)
{
	++cpu->regs[REG_HL].reg;
	
	return 8;
}

/* INC H */
int
op_0x24(gb_cpu *cpu)
{
	cpu->regs[REG_HL].hi = inc_byte(FLAG_P(cpu), cpu->regs[REG_HL].hi);
	
	return 4;
}

/* DEC H */
int
op_0x25(gb_cpu *cpu)
{
	cpu->regs[REG_HL].lo = dec_byte(FLAG_P(cpu), cpu->regs[REG_HL].hi);
	
	return 4;
}

/* LD H,d8 */
int
op_0x26(gb_cpu *cpu, BYTE d8)
{
	cpu->regs[REG_HL].hi = d8;
	
	return 8;
}

/* DDA */
int
op_0x27(gb_cpu *cpu)
{
	IMPLEMENT("DDA");
	
	return 4;
}

/* JR Z,r8 */
int
op_0x28(gb_cpu *cpu, SIGNED_BYTE r8)
{
	if (FLAG(cpu) & BIT(FLAG_Z)) {
		cpu->pc += r8;
		return 12;
	}
	
	return 8;
}

/* ADD HL,HL */
int
op_0x29(gb_cpu *cpu)
{
	add_word(FLAG_P(cpu), &cpu->regs[REG_HL].reg, cpu->regs[REG_HL].reg);
	
	return 8;
}

/* LD A,(HL+) */
int
op_0x2A(gb_cpu *cpu)
{
	cpu->regs[REG_AF].hi = read_byte(cpu, cpu->regs[REG_HL].reg++);
	
	return 8;
}

/* DEC HL */
int
op_0x2B(gb_cpu *cpu)
{
	--cpu->regs[REG_HL].reg;
	
	return 8;
}

/* INC L */
int
op_0x2C(gb_cpu *cpu)
{
	cpu->regs[REG_HL].lo = inc_byte(FLAG_P(cpu), cpu->regs[REG_HL].lo);
	
	return 4;
}

/* DEC L */
int
op_0x2D(gb_cpu *cpu)
{
	cpu->regs[REG_HL].lo = dec_byte(FLAG_P(cpu), cpu->regs[REG_HL].lo);
	
	return 4;
}

/* LD L,d8 */
int
op_0x2E(gb_cpu *cpu, BYTE d8)
{
	cpu->regs[REG_HL].lo = d8;
	
	return 8;
}

/* CPL */
int
op_0x2F(gb_cpu *cpu)
{
	cpu->regs[REG_AF].hi = ~cpu->regs[REG_AF].hi;
	
	FLAG(cpu) |= BIT(FLAG_N) | BIT(FLAG_H);
	
	return 4;
}

/* JR NC,r8 */
int op_0x30(gb_cpu *cpu, SIGNED_BYTE r8)
{
	if ((FLAG(cpu) & BIT(FLAG_C))) {
		cpu->pc += r8;
		return 12;
	}
	
	return 8;
}

/* LD SP,d16 */
int
op_0x31(gb_cpu *cpu, WORD d16)
{
	cpu->stack = (reg*)&cpu->memory[d16];
	
	return 12;
}

/* LD (HL-),A */
int
op_0x32(gb_cpu *cpu)
{
	write_byte(cpu, cpu->regs[REG_HL].reg--, cpu->regs[REG_AF].hi);
	
	return 8;
}

/* INC SP */
int
op_0x33(gb_cpu *cpu)
{
	++cpu->stack->reg;
	
	return 8;
}

/* INC (HL) */
int
op_0x34(gb_cpu *cpu)
{
	write_byte(cpu, cpu->regs[REG_HL].reg,
		   inc_byte(FLAG_P(cpu), read_byte(cpu, cpu->regs[REG_HL].reg)));
	
	return 4;
}

/* DEC (HL) */
int
op_0x35(gb_cpu *cpu)
{
	write_byte(cpu, cpu->regs[REG_HL].reg,
		   dec_byte(FLAG_P(cpu), read_byte(cpu, cpu->regs[REG_HL].reg)));
	
	return 4;
}

/* LD (HL),d8 */
int
op_0x36(gb_cpu *cpu, BYTE d8)
{
	write_byte(cpu, cpu->regs[REG_HL].reg, d8);
	
	return 8;
}

/* SCF */
int
op_0x37(gb_cpu *cpu)
{
	RESET_FLAGS(FLAG(cpu), BIT(FLAG_N) | BIT(FLAG_H));
	
	FLAG(cpu) |= BIT(FLAG_C);
	
	return 4;
}

/* JR C,r8 */ 
int
op_0x38(gb_cpu *cpu, SIGNED_BYTE r8)
{
	if (FLAG(cpu) & BIT(FLAG_C)) {
		cpu->pc += r8;
		return 12;
	}
	
	return 8;
}

/* ADD HL,SP */
int
op_0x39(gb_cpu *cpu)
{
	add_word(FLAG_P(cpu), &cpu->regs[REG_HL].reg, cpu->stack->reg);
	
	return 8;
}

/* LD A,(HL-) */
int
op_0x3A(gb_cpu *cpu)
{
	cpu->regs[REG_AF].hi = read_byte(cpu, cpu->regs[REG_HL].reg--);
	
	return 8;
}

/* DEC SP */
int
op_0x3B(gb_cpu *cpu)
{
	--cpu->stack->reg;
	
	return 8;
}

/* INC A */
int
op_0x3C(gb_cpu *cpu)
{
	cpu->regs[REG_AF].hi = inc_byte(FLAG_P(cpu), cpu->regs[REG_AF].hi);
	
	return 4;
}

/* DEC A */
int
op_0x3D(gb_cpu *cpu)
{
	cpu->regs[REG_AF].hi = dec_byte(FLAG_P(cpu), cpu->regs[REG_AF].hi);
	
	return 4;
}

/* LD A,d8 */
int
op_0x3E(gb_cpu *cpu, BYTE d8)
{
	cpu->regs[REG_AF].hi = d8;
	
	return 8;
}

/* CCF */
int
op_0x3F(gb_cpu *cpu)
{
	RESET_FLAGS(FLAG(cpu), BIT(FLAG_N) | BIT(FLAG_H));
	
	FLAG(cpu) ^= BIT(FLAG_C);
	
	return 4;
}

/* LD B,B */
int
op_0x40(gb_cpu *cpu)
{
	cpu->regs[REG_BC].hi = cpu->regs[REG_BC].hi;
	
	return 4;
}

/* LD B,C */
int
op_0x41(gb_cpu *cpu)
{
	cpu->regs[REG_BC].hi = cpu->regs[REG_BC].lo;
	
	return 4;
}

/* LD B,D */
int
op_0x42(gb_cpu *cpu)
{
	cpu->regs[REG_BC].hi = cpu->regs[REG_DE].hi;
	
	return 4;
}

/* LD B,E */
int
op_0x43(gb_cpu *cpu)
{
	cpu->regs[REG_BC].hi = cpu->regs[REG_DE].lo;
	
	return 4;
}

/* LD B,H */
int
op_0x44(gb_cpu *cpu)
{
	cpu->regs[REG_BC].hi = cpu->regs[REG_HL].hi;
	
	return 4;
}

/* LD B,L */
int
op_0x45(gb_cpu *cpu)
{
	cpu->regs[REG_BC].hi = cpu->regs[REG_HL].lo;
	
	return 4;
}

/* LD B,(HL) */
int
op_0x46(gb_cpu *cpu)
{
	cpu->regs[REG_BC].hi = read_byte(cpu, cpu->regs[REG_HL].reg);
	
	return 8;
}

/* LD B,A */
int
op_0x47(gb_cpu *cpu)
{
	cpu->regs[REG_BC].hi = cpu->regs[REG_AF].hi;
	
	return 4;
}

/* LD C,B */
int
op_0x48(gb_cpu *cpu)
{
	cpu->regs[REG_BC].lo = cpu->regs[REG_BC].hi;
	
	return 4;
}

/* LD C,C */
int
op_0x49(gb_cpu *cpu)
{
	cpu->regs[REG_BC].lo = cpu->regs[REG_BC].lo;
	
	return 4;
}

/* LD C,D */
int
op_0x4A(gb_cpu *cpu)
{
	cpu->regs[REG_BC].lo = cpu->regs[REG_DE].hi;
	
	return 4;
}

/* LD C,E */
int
op_0x4B(gb_cpu *cpu)
{
	cpu->regs[REG_BC].lo = cpu->regs[REG_DE].lo;
	
	return 4;
}

/* LD C,H */
int
op_0x4C(gb_cpu *cpu)
{
	cpu->regs[REG_BC].lo = cpu->regs[REG_HL].hi;
	
	return 4;
}

/* LD C,L */
int
op_0x4D(gb_cpu *cpu)
{
	cpu->regs[REG_BC].lo = cpu->regs[REG_HL].lo;
	
	return 4;
}

/* LD C,(HL) */
int
op_0x4E(gb_cpu *cpu)
{
	cpu->regs[REG_BC].lo = read_byte(cpu, cpu->regs[REG_HL].reg);
	
	return 8;
}

/* LD C,A */
int
op_0x4F(gb_cpu *cpu)
{
	cpu->regs[REG_BC].lo = cpu->regs[REG_AF].hi;
	
	return 4;
}

/* LD D,B */
int
op_0x50(gb_cpu *cpu)
{
	cpu->regs[REG_DE].hi = cpu->regs[REG_BC].hi;
	
	return 4;
}

/* LD D,C */
int
op_0x51(gb_cpu *cpu)
{
	cpu->regs[REG_DE].hi = cpu->regs[REG_BC].lo;
	
	return 4;
}

/* LD D,D */
int
op_0x52(gb_cpu *cpu)
{
	cpu->regs[REG_DE].hi = cpu->regs[REG_DE].hi;
	
	return 4;
}

/* LD D,E */
int
op_0x53(gb_cpu *cpu)
{
	cpu->regs[REG_DE].hi = cpu->regs[REG_DE].lo;
	
	return 4;
}

/* LD D,H */
int
op_0x54(gb_cpu *cpu)
{
	cpu->regs[REG_DE].hi = cpu->regs[REG_HL].hi;
	
	return 4;
}

/* LD D,L */
int
op_0x55(gb_cpu *cpu)
{
	cpu->regs[REG_DE].hi = cpu->regs[REG_HL].lo;
	
	return 4;
}

/* LD D,(HL) */
int
op_0x56(gb_cpu *cpu)
{
	cpu->regs[REG_DE].hi = read_byte(cpu, cpu->regs[REG_HL].reg);
	
	return 8;
}

/* LD D,A */
int
op_0x57(gb_cpu *cpu)
{
	cpu->regs[REG_DE].hi = cpu->regs[REG_AF].hi;
	
	return 4;
}

/* LD E,B */
int
op_0x58(gb_cpu *cpu)
{
	cpu->regs[REG_DE].lo = cpu->regs[REG_BC].hi;
	
	return 4;
}

/* LD E,C */
int
op_0x59(gb_cpu *cpu)
{
	cpu->regs[REG_DE].lo = cpu->regs[REG_BC].lo;
	
	return 4;
}

/* LD E,D */
int
op_0x5A(gb_cpu *cpu)
{
	cpu->regs[REG_DE].lo = cpu->regs[REG_DE].hi;
	
	return 4;
}

/* LD E,E */
int
op_0x5B(gb_cpu *cpu)
{
	cpu->regs[REG_DE].lo = cpu->regs[REG_DE].lo;
	
	return 4;
}

/* LD E,H */
int
op_0x5C(gb_cpu *cpu)
{
	cpu->regs[REG_DE].lo = cpu->regs[REG_HL].hi;
	
	return 4;
}

/* LD E,L */
int
op_0x5D(gb_cpu *cpu)
{
	cpu->regs[REG_DE].lo = cpu->regs[REG_HL].lo;
	
	return 4;
}

/* LD E,(HL) */
int
op_0x5E(gb_cpu *cpu)
{
	cpu->regs[REG_DE].lo = read_byte(cpu, cpu->regs[REG_HL].reg);
	
	return 8;
}

/* LD E,A */
int
op_0x5F(gb_cpu *cpu)
{
	cpu->regs[REG_DE].lo = cpu->regs[REG_AF].hi;
	
	return 4;
}

/* LD H,B */
int
op_0x60(gb_cpu *cpu)
{
	cpu->regs[REG_HL].hi = cpu->regs[REG_BC].hi;
	
	return 4;
}

/* LD H,C */
int
op_0x61(gb_cpu *cpu)
{
	cpu->regs[REG_HL].hi = cpu->regs[REG_BC].lo;
	
	return 4;
}

/* LD H,D */ 
int
op_0x62(gb_cpu *cpu)
{
	cpu->regs[REG_HL].hi = cpu->regs[REG_DE].hi;
	
	return 4;
}

/* LD H,E */
int
op_0x63(gb_cpu *cpu)
{
	cpu->regs[REG_HL].hi = cpu->regs[REG_DE].lo;
	
	return 4;
}

/* LD H,H */
int
op_0x64(gb_cpu *cpu)
{
	cpu->regs[REG_HL].hi = cpu->regs[REG_HL].hi;
	
	return 4;
}

/* LD H,L */
int
op_0x65(gb_cpu *cpu)
{
	cpu->regs[REG_HL].hi = cpu->regs[REG_HL].lo;
	
	return 4;
}

/* LD H,(HL) */
int
op_0x66(gb_cpu *cpu)
{
	cpu->regs[REG_HL].hi = read_byte(cpu, cpu->regs[REG_HL].reg);
	
	return 8;
}

/* LD H,A */
int
op_0x67(gb_cpu *cpu)
{
	cpu->regs[REG_HL].hi = cpu->regs[REG_AF].hi;
	
	return 4;
}

/* LD L,B */
int
op_0x68(gb_cpu *cpu)
{
	cpu->regs[REG_HL].lo = cpu->regs[REG_BC].hi;
	
	return 4;
}

/* LD L,C */
int
op_0x69(gb_cpu *cpu)
{
	cpu->regs[REG_HL].lo = cpu->regs[REG_BC].lo;
	
	return 4;
}

/* LD L,D */
int
op_0x6A(gb_cpu *cpu)
{
	cpu->regs[REG_HL].lo = cpu->regs[REG_DE].hi;
	
	return 4;
}

/* LD L,E */
int
op_0x6B(gb_cpu *cpu)
{
	cpu->regs[REG_HL].lo = cpu->regs[REG_DE].lo;
	
	return 4;
}

/* LD L,H */
int
op_0x6C(gb_cpu *cpu)
{	
	cpu->regs[REG_HL].lo = cpu->regs[REG_HL].hi;
	
	return 4;
}

/* LD L,L */
int
op_0x6D(gb_cpu *cpu)
{
	cpu->regs[REG_HL].lo = cpu->regs[REG_HL].lo;
	
	return 4;
}

/* LD L,(HL) */
int
op_0x6E(gb_cpu *cpu)
{
	cpu->regs[REG_HL].lo = read_byte(cpu, cpu->regs[REG_HL].reg);
	
	return 8;
}

/* LD L,A */
int
op_0x6F(gb_cpu *cpu)
{
	cpu->regs[REG_HL].lo = cpu->regs[REG_AF].hi;
	
	return 4;
}

/* LD (HL),B */
int
op_0x70(gb_cpu *cpu)
{
	write_byte(cpu, cpu->regs[REG_HL].reg, cpu->regs[REG_BC].hi);
	
	return 8;
}

/* LD (HL),C */
int
op_0x71(gb_cpu *cpu)
{
	write_byte(cpu, cpu->regs[REG_HL].reg, cpu->regs[REG_BC].lo);
	
	return 8;
}

/* LD (HL),D */
int
op_0x72(gb_cpu *cpu)
{
	write_byte(cpu, cpu->regs[REG_HL].reg, cpu->regs[REG_DE].hi);
	
	return 8;
}

/* LD (HL),E */
int
op_0x73(gb_cpu *cpu)
{
	write_byte(cpu, cpu->regs[REG_HL].reg, cpu->regs[REG_DE].lo);
	
	return 8;
}

/* LD (HL),H */
int
op_0x74(gb_cpu *cpu)
{
	write_byte(cpu, cpu->regs[REG_HL].reg, cpu->regs[REG_HL].hi);
	
	return 8;
}

/* LD (HL),L */
int
op_0x75(gb_cpu *cpu)
{
	write_byte(cpu, cpu->regs[REG_HL].reg, cpu->regs[REG_HL].lo);
	
	return 8;
}

/* HALT */
int
op_0x76(gb_cpu *cpu)
{
	IMPLEMENT("HALT");
	
	return 4;
}

/* LD (HL),A */
int
op_0x77(gb_cpu *cpu)
{
	write_byte(cpu, cpu->regs[REG_HL].reg, cpu->regs[REG_AF].hi);
	
	return 8;
}

/* LD A,B */
int
op_0x78(gb_cpu *cpu)
{
	cpu->regs[REG_AF].hi = cpu->regs[REG_BC].hi;
	
	return 4;
}

/* LD A,C */
int
op_0x79(gb_cpu *cpu)
{
	cpu->regs[REG_AF].hi = cpu->regs[REG_BC].lo;
	
	return 4;
}

/* LD A,D */
int
op_0x7A(gb_cpu *cpu)
{
	cpu->regs[REG_AF].hi = cpu->regs[REG_DE].hi;
	
	return 4;
}

/* LD A,E */
int
op_0x7B(gb_cpu *cpu)
{
	cpu->regs[REG_AF].hi = cpu->regs[REG_DE].lo;
	
	return 4;
}

/* LD A,H */
int
op_0x7C(gb_cpu *cpu)
{
	cpu->regs[REG_AF].hi = cpu->regs[REG_HL].hi;
	
	return 4;
}

/* LD A,L */
int
op_0x7D(gb_cpu *cpu)
{
	cpu->regs[REG_AF].hi = cpu->regs[REG_HL].lo;
	
	return 4;
}

/* LD A,(HL) */
int
op_0x7E(gb_cpu *cpu)
{
	cpu->regs[REG_AF].hi = read_byte(cpu, cpu->regs[REG_HL].reg);
	
	return 8;
}

/* LD A,A */
int
op_0x7F(gb_cpu *cpu)
{
	cpu->regs[REG_AF].hi = cpu->regs[REG_AF].hi;
	
	return 4;
}

/* ADD A,B */
int
op_0x80(gb_cpu *cpu)
{
	add_byte(&cpu->regs[REG_AF].hi, &cpu->regs[REG_AF].hi, cpu->regs[REG_BC].hi);
	
	return 4;
}

/* ADD A,C */
int
op_0x81(gb_cpu *cpu)
{
	add_byte(&cpu->regs[REG_AF].hi, &cpu->regs[REG_AF].hi, cpu->regs[REG_BC].lo);
	
	return 4;
}

/* ADD A,D */
int
op_0x82(gb_cpu *cpu)
{
	add_byte(&cpu->regs[REG_AF].hi, &cpu->regs[REG_AF].hi, cpu->regs[REG_DE].hi);
	
	return 4;
}

/* ADD A,E */
int
op_0x83(gb_cpu *cpu)
{
	add_byte(&cpu->regs[REG_AF].hi, &cpu->regs[REG_AF].hi, cpu->regs[REG_DE].lo);
	
	return 4;
}

/* ADD A,H */
int
op_0x84(gb_cpu *cpu)
{
	add_byte(&cpu->regs[REG_AF].hi, &cpu->regs[REG_AF].hi, cpu->regs[REG_HL].hi);
	
	return 4;
}

/* ADD A,L */
int
op_0x85(gb_cpu *cpu)
{
	add_byte(&cpu->regs[REG_AF].hi, &cpu->regs[REG_AF].hi, cpu->regs[REG_HL].lo);
	
	return 4;
}

/* ADD A,(HL) */
int
op_0x86(gb_cpu *cpu)
{
	add_byte(&cpu->regs[REG_AF].hi, &cpu->regs[REG_AF].hi,
		 read_byte(cpu, cpu->regs[REG_HL].reg));
	
	return 8;
}

/* ADD A,A */
int
op_0x87(gb_cpu *cpu)
{
	add_byte(&cpu->regs[REG_AF].hi, &cpu->regs[REG_AF].hi, cpu->regs[REG_AF].hi);
	
	return 4;
}

/* ADC A,B */
int
op_0x88(gb_cpu *cpu)
{
	add_byte(FLAG_P(cpu), &cpu->regs[REG_AF].hi,
		 cpu->regs[REG_BC].hi + ((FLAG(cpu) & BIT(FLAG_C)) >> FLAG_C));
	
	return 4;
}

/* ADC A,C */
int
op_0x89(gb_cpu *cpu)
{
	add_byte(FLAG_P(cpu), &cpu->regs[REG_AF].hi,
		 cpu->regs[REG_BC].lo + ((FLAG(cpu) & BIT(FLAG_C)) >> FLAG_C));
	
	return 4;
}

/* ADC A,D */
int
op_0x8A(gb_cpu *cpu)
{
	add_byte(FLAG_P(cpu), &cpu->regs[REG_AF].hi,
		 cpu->regs[REG_DE].hi + ((FLAG(cpu) & BIT(FLAG_C)) >> FLAG_C));
	
	return 4;
}

/* ADC A,E */
int
op_0x8B(gb_cpu *cpu)
{
	add_byte(FLAG_P(cpu), &cpu->regs[REG_AF].hi,
		 cpu->regs[REG_DE].lo + ((FLAG(cpu) & BIT(FLAG_C)) >> FLAG_C));
	
	return 4;
}

/* ADC A,H */
int
op_0x8C(gb_cpu *cpu)
{
	add_byte(FLAG_P(cpu), &cpu->regs[REG_AF].hi,
		 cpu->regs[REG_HL].hi + ((FLAG(cpu) & BIT(FLAG_C)) >> FLAG_C));
	
	return 4;
}

/* ADC A,L */
int
op_0x8D(gb_cpu *cpu)
{
	add_byte(FLAG_P(cpu), &cpu->regs[REG_AF].hi,
		 cpu->regs[REG_HL].lo + ((FLAG(cpu) & BIT(FLAG_C)) >> FLAG_C));
	
	return 4;
}

/* ADC A,(HL) */
int
op_0x8E(gb_cpu *cpu)
{
	add_byte(FLAG_P(cpu), &cpu->regs[REG_AF].hi,
		 read_byte(cpu, cpu->regs[REG_HL].reg) + ((FLAG(cpu) & BIT(FLAG_C)) >> FLAG_C));
	
	return 8;
}

/* ADC A,A */
int
op_0x8F(gb_cpu *cpu)
{
	add_byte(FLAG_P(cpu), &cpu->regs[REG_AF].hi,
		 cpu->regs[REG_AF].hi + ((FLAG(cpu) & BIT(FLAG_C)) >> FLAG_C));
	
	return 4;
}

/* SUB B */
int
op_0x90(gb_cpu *cpu)
{
	sub_byte(FLAG_P(cpu), &cpu->regs[REG_AF].hi, cpu->regs[REG_BC].hi);
	
	return 4;
}

/* SUB C */
int
op_0x91(gb_cpu *cpu)
{
	sub_byte(FLAG_P(cpu), &cpu->regs[REG_AF].hi, cpu->regs[REG_BC].lo);
	
	return 4;
}

/* SUB D */
int
op_0x92(gb_cpu *cpu)
{
	sub_byte(FLAG_P(cpu), &cpu->regs[REG_AF].hi, cpu->regs[REG_DE].hi);
	
	return 4;
}

/* SUB E */
int
op_0x93(gb_cpu *cpu)
{
	sub_byte(FLAG_P(cpu), &cpu->regs[REG_AF].hi, cpu->regs[REG_DE].lo);
	
	return 4;
}

/* SUB H */
int
op_0x94(gb_cpu *cpu)
{
	sub_byte(FLAG_P(cpu), &cpu->regs[REG_AF].hi, cpu->regs[REG_HL].hi);
	
	return 4;
}

/* SUB L */
int
op_0x95(gb_cpu *cpu)
{
	sub_byte(FLAG_P(cpu), &cpu->regs[REG_AF].hi, cpu->regs[REG_HL].lo);
	
	return 4;
}

/* SUB (HL) */
int
op_0x96(gb_cpu *cpu)
{
	sub_byte(FLAG_P(cpu), &cpu->regs[REG_AF].hi,
		 read_byte(cpu, cpu->regs[REG_HL].reg));
	
	return 8;
}

/* SUB A */
int
op_0x97(gb_cpu *cpu)
{
	sub_byte(FLAG_P(cpu), &cpu->regs[REG_AF].hi, cpu->regs[REG_AF].hi);
	
	return 4;
}

/* SBC A,B */
int
op_0x98(gb_cpu *cpu)
{
	sub_byte(FLAG_P(cpu), &cpu->regs[REG_AF].hi,
		 cpu->regs[REG_BC].hi + ((FLAG(cpu) & BIT(FLAG_C)) << FLAG_C));
	
	return 4;
}

/* SBC A,C */
int
op_0x99(gb_cpu *cpu)
{
	sub_byte(FLAG_P(cpu), &cpu->regs[REG_AF].hi,
		 cpu->regs[REG_BC].lo + ((FLAG(cpu) & BIT(FLAG_C)) << FLAG_C));
	
	return 4;
}

/* SBC A,D */
int
op_0x9A(gb_cpu *cpu)
{
	sub_byte(FLAG_P(cpu), &cpu->regs[REG_AF].hi,
		 cpu->regs[REG_DE].hi + ((FLAG(cpu) & BIT(FLAG_C)) << FLAG_C));
	
	return 4;
}

/* SBC A,E */
int
op_0x9B(gb_cpu *cpu)
{
	sub_byte(FLAG_P(cpu), &cpu->regs[REG_AF].hi,
		 cpu->regs[REG_DE].lo + ((FLAG(cpu) & BIT(FLAG_C)) << FLAG_C));
	
	return 4;
}

/* SBC A,H */
int
op_0x9C(gb_cpu *cpu)
{
	sub_byte(FLAG_P(cpu), &cpu->regs[REG_AF].hi,
		 cpu->regs[REG_HL].hi + ((FLAG(cpu) & BIT(FLAG_C)) << FLAG_C));
	
	return 4;
}

/* SBC A,L */
int
op_0x9D(gb_cpu *cpu)
{
	sub_byte(FLAG_P(cpu), &cpu->regs[REG_AF].hi,
		 cpu->regs[REG_HL].lo + ((FLAG(cpu) & BIT(FLAG_C)) << FLAG_C));
	
	return 4;
}

/* SBC A,(HL) */
int
op_0x9E(gb_cpu *cpu)
{
	sub_byte(FLAG_P(cpu), &cpu->regs[REG_AF].hi,
		 read_byte(cpu, cpu->regs[REG_HL].reg) + ((FLAG(cpu) & BIT(FLAG_C)) << FLAG_C));
	
	return 8;
}

/* SBC A,A */
int
op_0x9F(gb_cpu *cpu)
{
	sub_byte(FLAG_P(cpu), &cpu->regs[REG_AF].hi,
		 cpu->regs[REG_AF].hi + ((FLAG(cpu) & BIT(FLAG_C)) << FLAG_C));
	
	return 4;
}

/* AND B */
int
op_0xA0(gb_cpu *cpu)
{
	and_byte(FLAG_P(cpu), &cpu->regs[REG_AF].hi, cpu->regs[REG_BC].hi);
	
	return 4;
}

/* AND C */
int
op_0xA1(gb_cpu *cpu)
{
	and_byte(FLAG_P(cpu), &cpu->regs[REG_AF].hi, cpu->regs[REG_BC].lo);
	
	return 4;
}

/* AND D */
int
op_0xA2(gb_cpu *cpu)
{
	and_byte(FLAG_P(cpu), &cpu->regs[REG_AF].hi, cpu->regs[REG_DE].hi);
	
	return 4;
}

/* AND E */
int
op_0xA3(gb_cpu *cpu)
{
	and_byte(FLAG_P(cpu), &cpu->regs[REG_AF].hi, cpu->regs[REG_DE].lo);
	
	return 4;
}

/* AND H */
int
op_0xA4(gb_cpu *cpu)
{
	and_byte(FLAG_P(cpu), &cpu->regs[REG_AF].hi, cpu->regs[REG_HL].hi);
	
	return 4;
}

/* AND L */
int
op_0xA5(gb_cpu *cpu)
{
	and_byte(FLAG_P(cpu), &cpu->regs[REG_AF].hi, cpu->regs[REG_HL].lo);
	
	return 4;
}

/* AND (HL) */
int
op_0xA6(gb_cpu *cpu)
{
	and_byte(FLAG_P(cpu), &cpu->regs[REG_AF].hi,
		 read_byte(cpu, cpu->regs[REG_HL].reg));
	
	return 8;
}

/* AND A */
int
op_0xA7(gb_cpu *cpu)
{
	and_byte(FLAG_P(cpu), &cpu->regs[REG_AF].hi, cpu->regs[REG_AF].hi);
	
	return 4;
}

/* XOR B */
int
op_0xA8(gb_cpu *cpu)
{
	xor_byte(FLAG_P(cpu), &cpu->regs[REG_AF].hi, cpu->regs[REG_BC].hi);
	
	return 4;
}

/* XOR C */
int
op_0xA9(gb_cpu *cpu)
{
	xor_byte(FLAG_P(cpu), &cpu->regs[REG_AF].hi, cpu->regs[REG_BC].lo);
	
	return 4;
}

/* XOR D */
int
op_0xAA(gb_cpu *cpu)
{
	xor_byte(FLAG_P(cpu), &cpu->regs[REG_AF].hi, cpu->regs[REG_DE].hi);
	
	return 4;
}

/* XOR E */
int
op_0xAB(gb_cpu *cpu)
{
	xor_byte(FLAG_P(cpu), &cpu->regs[REG_AF].hi, cpu->regs[REG_DE].lo);
	
	return 4;
}

/* XOR H */
int
op_0xAC(gb_cpu *cpu)
{
	xor_byte(FLAG_P(cpu), &cpu->regs[REG_AF].hi, cpu->regs[REG_HL].hi);
	
	return 4;
}

/* XOR L */
int
op_0xAD(gb_cpu *cpu)
{
	xor_byte(FLAG_P(cpu), &cpu->regs[REG_AF].hi, cpu->regs[REG_HL].lo);
	
	return 4;
}

/* XOR (HL) */
int
op_0xAE(gb_cpu *cpu)
{
	xor_byte(FLAG_P(cpu), &cpu->regs[REG_AF].hi,
		 read_byte(cpu, cpu->regs[REG_HL].reg));
	
	return 8;
}

/* XOR A */
int
op_0xAF(gb_cpu *cpu)
{
	xor_byte(FLAG_P(cpu), &cpu->regs[REG_AF].hi, cpu->regs[REG_AF].hi);
	
	return 4;
}

/* OR B */
int
op_0xB0(gb_cpu *cpu)
{
	or_byte(FLAG_P(cpu), &cpu->regs[REG_AF].hi, cpu->regs[REG_BC].hi);
	
	return 4;
}

/* OR C */
int
op_0xB1(gb_cpu *cpu)
{
	or_byte(FLAG_P(cpu), &cpu->regs[REG_AF].hi, cpu->regs[REG_BC].lo);
	
	return 4;
}

/* OR D */
int
op_0xB2(gb_cpu *cpu)
{
	or_byte(FLAG_P(cpu), &cpu->regs[REG_AF].hi, cpu->regs[REG_DE].hi);
	
	return 4;
}

/* OR E */
int
op_0xB3(gb_cpu *cpu)
{
	or_byte(FLAG_P(cpu), &cpu->regs[REG_AF].hi, cpu->regs[REG_DE].lo);
	
	return 4;
}

/* OR H */
int
op_0xB4(gb_cpu *cpu)
{
	or_byte(FLAG_P(cpu), &cpu->regs[REG_AF].hi, cpu->regs[REG_HL].hi);
	
	return 4;
}

/* OR L */
int
op_0xB5(gb_cpu *cpu)
{
	or_byte(FLAG_P(cpu), &cpu->regs[REG_AF].hi, cpu->regs[REG_HL].lo);
	
	return 4;
}

/* OR (HL) */
int
op_0xB6(gb_cpu *cpu)
{
	or_byte(FLAG_P(cpu), &cpu->regs[REG_AF].hi,
		read_byte(cpu, cpu->regs[REG_HL].reg));
	
	return 8;
}

/* OR A */
int
op_0xB7(gb_cpu *cpu)
{
	or_byte(FLAG_P(cpu), &cpu->regs[REG_AF].hi, cpu->regs[REG_AF].hi);
	
	return 4;
}

/* CP B */
int
op_0xB8(gb_cpu *cpu)
{
	cp_byte(FLAG_P(cpu), cpu->regs[REG_AF].hi, cpu->regs[REG_BC].hi);
	
	return 4;
}

/* CP C */
int
op_0xB9(gb_cpu *cpu)
{
	cp_byte(FLAG_P(cpu), cpu->regs[REG_AF].hi, cpu->regs[REG_BC].lo);
	
	return 4;
}

/* CP D */
int
op_0xBA(gb_cpu *cpu)
{
	cp_byte(FLAG_P(cpu), cpu->regs[REG_AF].hi, cpu->regs[REG_DE].hi);
	
	return 4;
}

/* CP E */
int
op_0xBB(gb_cpu *cpu)
{
	cp_byte(FLAG_P(cpu), cpu->regs[REG_AF].hi, cpu->regs[REG_DE].lo);
	
	return 4;
}

/* CP H */
int
op_0xBC(gb_cpu *cpu)
{
	cp_byte(FLAG_P(cpu), cpu->regs[REG_AF].hi, cpu->regs[REG_HL].hi);
	
	return 4;
}

/* CP L */
int
op_0xBD(gb_cpu *cpu)
{
	cp_byte(FLAG_P(cpu), cpu->regs[REG_AF].hi, cpu->regs[REG_HL].lo);
	
	return 4;
}

/* CP (HL) */
int
op_0xBE(gb_cpu *cpu)
{
	cp_byte(FLAG_P(cpu), cpu->regs[REG_AF].hi,
		read_byte(cpu, cpu->regs[REG_HL].reg));
	
	return 8;
}

/* CP A */
int
op_0xBF(gb_cpu *cpu)
{
	cp_byte(FLAG_P(cpu), cpu->regs[REG_AF].hi, cpu->regs[REG_AF].hi);
	
	return 4;
}

/* RET NZ */
int
op_0xC0(gb_cpu *cpu)
{
	if (!(FLAG(cpu) & BIT(FLAG_Z))) {
		ret(cpu);
		return 20;
	}
	
	return 8;
}

/* POP BC */
int
op_0xC1(gb_cpu *cpu)
{
	cpu->regs[REG_BC].reg = pop(cpu);
	
	return 12;
}

/* JP NZ,a16 */
int
op_0xC2(gb_cpu *cpu, WORD a16)
{
	if (!(FLAG(cpu) & BIT(FLAG_Z))) {
		cpu->pc = a16;
		return 16;
	}
	
	return 12;
}

/* JP a16 */
int
op_0xC3(gb_cpu *cpu, WORD a16)
{
	cpu->pc = a16;
	
	return 16;
}

/* CALL NZ,a16 */
int
op_0xC4(gb_cpu *cpu, WORD a16)
{
	if (!(FLAG(cpu) & BIT(FLAG_Z))) {
		call(cpu, a16);
		return 24;
	}
	
	return 12;
}

/* PUSH BC */
int
op_0xC5(gb_cpu *cpu)
{
	push(cpu, cpu->regs[REG_BC].reg);
	
	return 16;
}

/* ADD A,d8 */
int
op_0xC6(gb_cpu *cpu, BYTE d8)
{
	add_byte(FLAG_P(cpu), &cpu->regs[REG_AF].hi, d8);
	
	return 8;
}

/* RST 00H */
int
op_0xC7(gb_cpu *cpu)
{
	call(cpu, 0x00);
	
	return 16;
}

/* RET Z */
int
op_0xC8(gb_cpu *cpu)
{
	if (FLAG(cpu) & BIT(FLAG_Z)) {
		cpu->pc = pop(cpu);
		return 20;
	}
	
	return 8;
}

/* RET */
int
op_0xC9(gb_cpu *cpu)
{
	cpu->pc = pop(cpu);
	
	return 16;
}

/* JP Z,a16 */
int
op_0xCA(gb_cpu *cpu, WORD a16)
{
	if (FLAG(cpu) & BIT(FLAG_Z)) {
		cpu->pc = a16;
		return 16;
	}
	
	return 12;
}

/* PREFIX CB */
int
op_0xCB(gb_cpu *cpu)
{
	const op *opcode;
	
	opcode = &ext_ops[cpu->memory[cpu->pc]];
	
	if (opcode->func == NULL) {
		printf("Missing implementation (CB)%s at $%04x\n", opcode->assembly, cpu->pc);
		exit(1);
	}
	
	return ((int(*)(gb_cpu*))opcode->func)(cpu);
}

/* CALL Z,a16 */
int
op_0xCC(gb_cpu *cpu, WORD a16)
{
	if (FLAG(cpu) & BIT(FLAG_Z)) {
		call(cpu, a16);
		return 24;
	}
	
	return 12;
}

/* CALL a16 */
int
op_0xCD(gb_cpu *cpu, WORD a16)
{
	call(cpu, a16);
	
	return 24;
}

/* ADC A,d8 */
int
op_0xCE(gb_cpu *cpu, BYTE d8)
{
	add_byte(FLAG_P(cpu), &cpu->regs[REG_AF].hi,
		 d8 + ((FLAG(cpu) & BIT(FLAG_C)) >> FLAG_C));
	
	return 8;
}

/* RST 08H */
int
op_0xCF(gb_cpu *cpu)
{
	call(cpu, 0x08);
	
	return 16;
}

/* RET NC */
int
op_0xD0(gb_cpu *cpu)
{
	if (!(FLAG(cpu) & BIT(FLAG_C))) {
		cpu->pc = pop(cpu);
		return 20;
	}
	
	return 8;
}

/* POP DE */
int
op_0xD1(gb_cpu *cpu)
{
	cpu->regs[REG_DE].reg = pop(cpu);
	
	return 12;
}

/* JP NC,a16 */
int
op_0xD2(gb_cpu *cpu, WORD a16)
{
	if (!(FLAG(cpu) & BIT(FLAG_C))) {
		cpu->pc = a16;
		return 16;
	}
	
	return 12;
}

/* CALL NC,a16 */
int
op_0xD4(gb_cpu *cpu, WORD a16)
{
	if (!(FLAG(cpu) & BIT(FLAG_C))) {
		call(cpu, a16);
		return 24;
	}
	
	return 12;
}

/* PUSH DE */
int
op_0xD5(gb_cpu *cpu)
{
	push(cpu, cpu->regs[REG_DE].reg);
	
	return 16;
}

/* SUB d8 */
int
op_0xD6(gb_cpu *cpu, BYTE d8)
{
	sub_byte(FLAG_P(cpu), &cpu->regs[REG_AF].hi, d8);
	
	return 8;
}

/* RST 10H */
int
op_0xD7(gb_cpu *cpu)
{
	call(cpu, 0x10);
	
	return 16;
}

/* RET C */
int
op_0xD8(gb_cpu *cpu)
{
	if (FLAG(cpu) & BIT(FLAG_C)) {
		ret(cpu);
		return 20;
	}
	
	return 8;
}

/* RETI */
int
op_0xD9(gb_cpu *cpu)
{
	cpu->ime = 1;
	ret(cpu);
	
	return 16;
}

/* JP C,a16 */
int
op_0xDA(gb_cpu *cpu, WORD a16)
{
	if (FLAG(cpu) & BIT(FLAG_C)) {
		cpu->pc = a16;
		return 16;
	}
	
	return 12;
}

/* CALL C,a16 */
int
op_0xDC(gb_cpu *cpu, WORD a16)
{
	if (FLAG(cpu) & BIT(FLAG_C)) {
		call(cpu, a16);
		return 24;
	}
	
	return 12;
}

/* SBC A,d8 */
int
op_0xDE(gb_cpu *cpu, BYTE d8)
{
	sub_byte(FLAG_P(cpu), &cpu->regs[REG_AF].hi,
		 d8 + ((FLAG(cpu) & BIT(FLAG_C)) >> FLAG_C));
	
	return 8;
}

/* RST 18H */
int
op_0xDF(gb_cpu *cpu)
{
	call(cpu, 0x18);
	
	return 16;
}

/* LDH (a8),A */
int
op_0xE0(gb_cpu *cpu, BYTE a8)
{
	write_byte(cpu, 0xFF00 + a8, cpu->regs[REG_AF].hi);
	
	return 12;
}

/* POP HL */
int
op_0xE1(gb_cpu *cpu)
{
	cpu->regs[REG_HL].reg = pop(cpu);
	
	return 12;
}

/* LD (C),A */
int
op_0xE2(gb_cpu *cpu)
{
	write_byte(cpu, 0xFF00 + cpu->regs[REG_BC].lo, cpu->regs[REG_AF].hi);
	
	return 8;
}

/* PUSH HL */
int
op_0xE5(gb_cpu *cpu)
{
	push(cpu, cpu->regs[REG_HL].reg);
	
	return 16;
}

/* AND d8 */
int
op_0xE6(gb_cpu *cpu, BYTE d8)
{
	and_byte(FLAG_P(cpu), &cpu->regs[REG_AF].hi, d8);
	
	return 8;
}

/* RST 20H */
int
op_0xE7(gb_cpu *cpu)
{
	call(cpu, 0x20);
	
	return 16;
}

/* ADD SP,r8 */
/* add to the top of the stack or the stack pointer itself? */
int
op_0xE8(gb_cpu *cpu, SIGNED_BYTE r8)
{
	add_word(FLAG_P(cpu), &cpu->stack->reg, r8);
	
	return 16;
}

/* JP (HL) */
int
op_0xE9(gb_cpu *cpu)
{
	cpu->pc = read_word(cpu, cpu->regs[REG_HL].reg);
	
	return 4;
}

/* LD (a16),A */
int
op_0xEA(gb_cpu *cpu, WORD a16)
{
	write_byte(cpu, a16, cpu->regs[REG_AF].hi);
	
	return 16;
}

/* XOR d8 */
int
op_0xEE(gb_cpu *cpu, BYTE d8)
{
	xor_byte(FLAG_P(cpu), &cpu->regs[REG_AF].hi, d8);
	
	return 8;
}

/* RST 28H */
int
op_0xEF(gb_cpu *cpu)
{
	call(cpu, 0x28);
	
	return 16;
}

/* LDH A,a8 */
int
op_0xF0(gb_cpu *cpu, BYTE a8)
{
	cpu->regs[REG_AF].hi = read_byte(cpu, 0xFF00 + a8);
	
	return 12;
}

/* POP AF */
int
op_0xF1(gb_cpu *cpu)
{
	cpu->regs[REG_AF].reg = pop(cpu);
	
	return 12;
}

/* LD A,(C) */
int
op_0xF2(gb_cpu *cpu)
{
	cpu->regs[REG_AF].hi = read_byte(cpu, 0xFF00 + cpu->regs[REG_BC].lo);
	
	return 8;
}

/* DI */
int
op_0xF3(gb_cpu *cpu)
{
	cpu->ime = 0;
	
	return 4;
}

/* PUSH AF */
int
op_0xF5(gb_cpu *cpu)
{
	push(cpu, cpu->regs[REG_AF].reg);
	
	return 16;
}

/* OR d8 */
int
op_0xF6(gb_cpu *cpu, BYTE d8)
{
	or_byte(FLAG_P(cpu), &cpu->regs[REG_AF].hi, d8);
	
	return 8;
}

/* RST 30H */
int
op_0xF7(gb_cpu *cpu)
{
	call(cpu, 0x30);
	
	return 16;
}

/* LD HL,SP+r8 */
int
op_0xF8(gb_cpu *cpu, BYTE r8)
{
	IMPLEMENT("LD HL,SP+r8");
	
	return 12;
}

/* LD SP,HL */
int
op_0xF9(gb_cpu *cpu)
{
	cpu->stack->reg = cpu->regs[REG_HL].reg;
	
	return 8;
}

/* LD A,(a16) */
int
op_0xFA(gb_cpu *cpu, WORD a16)
{
	cpu->regs[REG_AF].hi = read_byte(cpu, a16);
	
	return 16;
}

/* EI */
int
op_0xFB(gb_cpu *cpu)
{
	cpu->ime = 1;
	
	return 4;
}

/* CP d8 */
int
op_0xFE(gb_cpu *cpu, BYTE d8)
{
	cp_byte(FLAG_P(cpu), cpu->regs[REG_AF].hi, d8);
	
	return 8;
}

/* RST 38H */
int
op_0xFF(gb_cpu *cpu)
{
	call(cpu, 0x38);
	
	return 16;
}

/* ---=== EXTENDED OPCODES ===--- */

/* RLC B */
int
ext_op_0x00(gb_cpu *cpu)
{
	rot_byte(FLAG_P(cpu), &cpu->regs[REG_BC].hi, LEFT | CIRCULAR | BIT(FLAG_Z));
	
	return 8;
}

/* RLC C */
int
ext_op_0x01(gb_cpu *cpu)
{
	rot_byte(FLAG_P(cpu), &cpu->regs[REG_BC].lo, LEFT | CIRCULAR | BIT(FLAG_Z));
	
	return 8;
}

/* RLC D */
int
ext_op_0x02(gb_cpu *cpu)
{
	rot_byte(FLAG_P(cpu), &cpu->regs[REG_DE].hi, LEFT | CIRCULAR | BIT(FLAG_Z));
	
	return 8;
}

/* RLC E */
int
ext_op_0x03(gb_cpu *cpu)
{
	rot_byte(FLAG_P(cpu), &cpu->regs[REG_DE].lo, LEFT | CIRCULAR | BIT(FLAG_Z));
	
	return 8;
}

/* RLC H */
int
ext_op_0x04(gb_cpu *cpu)
{
	rot_byte(FLAG_P(cpu), &cpu->regs[REG_HL].hi, LEFT | CIRCULAR | BIT(FLAG_Z));
	
	return 8;
}

/* RLC L */
int
ext_op_0x05(gb_cpu *cpu)
{
	rot_byte(FLAG_P(cpu), &cpu->regs[REG_HL].lo, LEFT | CIRCULAR | BIT(FLAG_Z));
	
	return 8;
}

/* RLC (HL) */
int
ext_op_0x06(gb_cpu *cpu)
{
	rot_byte(FLAG_P(cpu),
		 &cpu->memory[cpu->regs[REG_HL].reg], LEFT | CIRCULAR | BIT(FLAG_Z));
	
	return 16;
}

/* RLC A */
int
ext_op_0x07(gb_cpu *cpu)
{
	rot_byte(FLAG_P(cpu), &cpu->regs[REG_AF].hi, LEFT | CIRCULAR | BIT(FLAG_Z));
	
	return 8;
}

/* RRC B */
int
ext_op_0x08(gb_cpu *cpu)
{
	rot_byte(FLAG_P(cpu), &cpu->regs[REG_BC].hi, RIGHT | CIRCULAR | BIT(FLAG_Z));
	
	return 8;
}

/* RRC C */
int
ext_op_0x09(gb_cpu *cpu)
{
	rot_byte(FLAG_P(cpu), &cpu->regs[REG_BC].lo, RIGHT | CIRCULAR | BIT(FLAG_Z));
	
	return 8;
}

/* RRC D */
int
ext_op_0x0A(gb_cpu *cpu)
{
	rot_byte(FLAG_P(cpu), &cpu->regs[REG_DE].hi, RIGHT | CIRCULAR | BIT(FLAG_Z));
	
	return 8;
}

/* RRC E */
int
ext_op_0x0B(gb_cpu *cpu)
{
	rot_byte(FLAG_P(cpu), &cpu->regs[REG_DE].lo, RIGHT | CIRCULAR | BIT(FLAG_Z));
	
	return 8;
}

/* RRC H */
int
ext_op_0x0C(gb_cpu *cpu)
{
	rot_byte(FLAG_P(cpu), &cpu->regs[REG_HL].hi, RIGHT | CIRCULAR | BIT(FLAG_Z));
	
	return 8;
}

/* RRC L */
int
ext_op_0x0D(gb_cpu *cpu)
{
	rot_byte(FLAG_P(cpu), &cpu->regs[REG_HL].lo, RIGHT | CIRCULAR | BIT(FLAG_Z));
	
	return 8;
}

/* RRC (HL) */
int
ext_op_0x0E(gb_cpu *cpu)
{
	rot_byte(FLAG_P(cpu),
		 &cpu->memory[cpu->regs[REG_HL].reg], RIGHT | CIRCULAR | BIT(FLAG_Z));
	
	return 8;
}

/* RRC A */
int
ext_op_0x0F(gb_cpu *cpu)
{
	rot_byte(FLAG_P(cpu), &cpu->regs[REG_AF].hi, RIGHT | CIRCULAR | BIT(FLAG_Z));
	
	return 8;
}

/* RL B */
int
ext_op_0x10(gb_cpu *cpu)
{
	rot_byte(FLAG_P(cpu), &cpu->regs[REG_BC].hi, LEFT | BIT(FLAG_Z));
	
	return 8;
}

/* RL C */
int
ext_op_0x11(gb_cpu *cpu)
{
	rot_byte(FLAG_P(cpu), &cpu->regs[REG_BC].lo, LEFT | BIT(FLAG_Z));
	
	return 8;
}

/* RL D */
int
ext_op_0x12(gb_cpu *cpu)
{
	rot_byte(FLAG_P(cpu), &cpu->regs[REG_DE].hi, LEFT | BIT(FLAG_Z));
	
	return 8;
}

/* RL E */
int
ext_op_0x13(gb_cpu *cpu)
{
	rot_byte(FLAG_P(cpu), &cpu->regs[REG_DE].lo, LEFT | BIT(FLAG_Z));
	
	return 8;
}

/* RL H */
int
ext_op_0x14(gb_cpu *cpu)
{
	rot_byte(FLAG_P(cpu), &cpu->regs[REG_HL].hi, LEFT | BIT(FLAG_Z));
	
	return 8;
}

/* RL L */
int
ext_op_0x15(gb_cpu *cpu)
{
	rot_byte(FLAG_P(cpu), &cpu->regs[REG_HL].lo, LEFT | BIT(FLAG_Z));
	
	return 8;
}

/* RL (HL) */
int
ext_op_0x16(gb_cpu *cpu)
{
	rot_byte(FLAG_P(cpu),
		 &cpu->memory[cpu->regs[REG_HL].reg], LEFT | BIT(FLAG_Z));
	
	return 8;
}

/* RL A */
int
ext_op_0x17(gb_cpu *cpu)
{
	rot_byte(FLAG_P(cpu), &cpu->regs[REG_AF].hi, LEFT | BIT(FLAG_Z));
	
	return 8;
}

/* RR B */
int
ext_op_0x18(gb_cpu *cpu)
{
	rot_byte(FLAG_P(cpu), &cpu->regs[REG_BC].hi, RIGHT | BIT(FLAG_Z));
	
	return 8;
}

/* RR C */
int
ext_op_0x19(gb_cpu *cpu)
{
	rot_byte(FLAG_P(cpu), &cpu->regs[REG_BC].lo, RIGHT | BIT(FLAG_Z));
	
	return 8;
}

/* RR D */
int
ext_op_0x1A(gb_cpu *cpu)
{
	rot_byte(FLAG_P(cpu), &cpu->regs[REG_DE].hi, RIGHT | BIT(FLAG_Z));
	
	return 8;
}

/* RR E */
int
ext_op_0x1B(gb_cpu *cpu)
{
	rot_byte(FLAG_P(cpu), &cpu->regs[REG_DE].lo, RIGHT | BIT(FLAG_Z));
	
	return 8;
}

/* RR H */
int
ext_op_0x1C(gb_cpu *cpu)
{
	rot_byte(FLAG_P(cpu), &cpu->regs[REG_HL].hi, RIGHT | BIT(FLAG_Z));
	
	return 8;
}

/* RR L */
int
ext_op_0x1D(gb_cpu *cpu)
{
	rot_byte(FLAG_P(cpu), &cpu->regs[REG_HL].lo, RIGHT | BIT(FLAG_Z));
	
	return 8;
}

/* RR (HL) */
int
ext_op_0x1E(gb_cpu *cpu)
{
	rot_byte(FLAG_P(cpu),
		 &cpu->memory[cpu->regs[REG_HL].reg], RIGHT | BIT(FLAG_Z));
	
	return 8;
}

/* RR A */
int
ext_op_0x1F(gb_cpu *cpu)
{
	rot_byte(FLAG_P(cpu), &cpu->regs[REG_AF].hi, RIGHT | BIT(FLAG_Z));
	
	return 8;
}

int ext_op_0x20(gb_cpu *cpu);
int ext_op_0x21(gb_cpu *cpu);
int ext_op_0x22(gb_cpu *cpu);
int ext_op_0x23(gb_cpu *cpu);
int ext_op_0x24(gb_cpu *cpu);
int ext_op_0x25(gb_cpu *cpu);
int ext_op_0x26(gb_cpu *cpu);
int ext_op_0x27(gb_cpu *cpu);
int ext_op_0x28(gb_cpu *cpu);
int ext_op_0x29(gb_cpu *cpu);
int ext_op_0x2A(gb_cpu *cpu);
int ext_op_0x2B(gb_cpu *cpu);
int ext_op_0x2C(gb_cpu *cpu);
int ext_op_0x2D(gb_cpu *cpu);
int ext_op_0x2E(gb_cpu *cpu);
int ext_op_0x2F(gb_cpu *cpu);

int ext_op_0x30(gb_cpu *cpu);
int ext_op_0x31(gb_cpu *cpu);
int ext_op_0x32(gb_cpu *cpu);
int ext_op_0x33(gb_cpu *cpu);
int ext_op_0x34(gb_cpu *cpu);
int ext_op_0x35(gb_cpu *cpu);
int ext_op_0x36(gb_cpu *cpu);

/* SWAP A */
int
ext_op_0x37(gb_cpu *cpu)
{
	swap_byte(FLAG_P(cpu), &cpu->regs[REG_AF].hi);
	
	return 8;
}

int ext_op_0x38(gb_cpu *cpu);
int ext_op_0x39(gb_cpu *cpu);
int ext_op_0x3A(gb_cpu *cpu);
int ext_op_0x3B(gb_cpu *cpu);
int ext_op_0x3C(gb_cpu *cpu);
int ext_op_0x3D(gb_cpu *cpu);
int ext_op_0x3E(gb_cpu *cpu);
int ext_op_0x3F(gb_cpu *cpu);

int ext_op_0x40(gb_cpu *cpu);
int ext_op_0x41(gb_cpu *cpu);
int ext_op_0x42(gb_cpu *cpu);
int ext_op_0x43(gb_cpu *cpu);
int ext_op_0x44(gb_cpu *cpu);
int ext_op_0x45(gb_cpu *cpu);
int ext_op_0x46(gb_cpu *cpu);
int ext_op_0x47(gb_cpu *cpu);
int ext_op_0x48(gb_cpu *cpu);
int ext_op_0x49(gb_cpu *cpu);
int ext_op_0x4A(gb_cpu *cpu);
int ext_op_0x4B(gb_cpu *cpu);
int ext_op_0x4C(gb_cpu *cpu);
int ext_op_0x4D(gb_cpu *cpu);
int ext_op_0x4E(gb_cpu *cpu);
int ext_op_0x4F(gb_cpu *cpu);

int ext_op_0x50(gb_cpu *cpu);
int ext_op_0x51(gb_cpu *cpu);
int ext_op_0x52(gb_cpu *cpu);
int ext_op_0x53(gb_cpu *cpu);
int ext_op_0x54(gb_cpu *cpu);
int ext_op_0x55(gb_cpu *cpu);
int ext_op_0x56(gb_cpu *cpu);
int ext_op_0x57(gb_cpu *cpu);
int ext_op_0x58(gb_cpu *cpu);
int ext_op_0x59(gb_cpu *cpu);
int ext_op_0x5A(gb_cpu *cpu);
int ext_op_0x5B(gb_cpu *cpu);
int ext_op_0x5C(gb_cpu *cpu);
int ext_op_0x5D(gb_cpu *cpu);
int ext_op_0x5E(gb_cpu *cpu);
int ext_op_0x5F(gb_cpu *cpu);

int ext_op_0x60(gb_cpu *cpu);
int ext_op_0x61(gb_cpu *cpu);
int ext_op_0x62(gb_cpu *cpu);
int ext_op_0x63(gb_cpu *cpu);
int ext_op_0x64(gb_cpu *cpu);
int ext_op_0x65(gb_cpu *cpu);
int ext_op_0x66(gb_cpu *cpu);
int ext_op_0x67(gb_cpu *cpu);
int ext_op_0x68(gb_cpu *cpu);
int ext_op_0x69(gb_cpu *cpu);
int ext_op_0x6A(gb_cpu *cpu);
int ext_op_0x6B(gb_cpu *cpu);
int ext_op_0x6C(gb_cpu *cpu);
int ext_op_0x6D(gb_cpu *cpu);
int ext_op_0x6E(gb_cpu *cpu);
int ext_op_0x6F(gb_cpu *cpu);

int ext_op_0x70(gb_cpu *cpu);
int ext_op_0x71(gb_cpu *cpu);
int ext_op_0x72(gb_cpu *cpu);
int ext_op_0x73(gb_cpu *cpu);
int ext_op_0x74(gb_cpu *cpu);
int ext_op_0x75(gb_cpu *cpu);
int ext_op_0x76(gb_cpu *cpu);
int ext_op_0x77(gb_cpu *cpu);
int ext_op_0x78(gb_cpu *cpu);
int ext_op_0x79(gb_cpu *cpu);
int ext_op_0x7A(gb_cpu *cpu);
int ext_op_0x7B(gb_cpu *cpu);
int ext_op_0x7C(gb_cpu *cpu);
int ext_op_0x7D(gb_cpu *cpu);
int ext_op_0x7E(gb_cpu *cpu);
int ext_op_0x7F(gb_cpu *cpu);

int ext_op_0x80(gb_cpu *cpu);
int ext_op_0x81(gb_cpu *cpu);
int ext_op_0x82(gb_cpu *cpu);
int ext_op_0x83(gb_cpu *cpu);
int ext_op_0x84(gb_cpu *cpu);
int ext_op_0x85(gb_cpu *cpu);
int ext_op_0x86(gb_cpu *cpu);
int ext_op_0x87(gb_cpu *cpu);
int ext_op_0x88(gb_cpu *cpu);
int ext_op_0x89(gb_cpu *cpu);
int ext_op_0x8A(gb_cpu *cpu);
int ext_op_0x8B(gb_cpu *cpu);
int ext_op_0x8C(gb_cpu *cpu);
int ext_op_0x8D(gb_cpu *cpu);
int ext_op_0x8E(gb_cpu *cpu);
int ext_op_0x8F(gb_cpu *cpu);

int ext_op_0x90(gb_cpu *cpu);
int ext_op_0x91(gb_cpu *cpu);
int ext_op_0x92(gb_cpu *cpu);
int ext_op_0x93(gb_cpu *cpu);
int ext_op_0x94(gb_cpu *cpu);
int ext_op_0x95(gb_cpu *cpu);
int ext_op_0x96(gb_cpu *cpu);
int ext_op_0x97(gb_cpu *cpu);
int ext_op_0x98(gb_cpu *cpu);
int ext_op_0x99(gb_cpu *cpu);
int ext_op_0x9A(gb_cpu *cpu);
int ext_op_0x9B(gb_cpu *cpu);
int ext_op_0x9C(gb_cpu *cpu);
int ext_op_0x9D(gb_cpu *cpu);
int ext_op_0x9E(gb_cpu *cpu);
int ext_op_0x9F(gb_cpu *cpu);

int ext_op_0xA0(gb_cpu *cpu);
int ext_op_0xA1(gb_cpu *cpu);
int ext_op_0xA2(gb_cpu *cpu);
int ext_op_0xA3(gb_cpu *cpu);
int ext_op_0xA4(gb_cpu *cpu);
int ext_op_0xA5(gb_cpu *cpu);
int ext_op_0xA6(gb_cpu *cpu);
int ext_op_0xA7(gb_cpu *cpu);
int ext_op_0xA8(gb_cpu *cpu);
int ext_op_0xA9(gb_cpu *cpu);
int ext_op_0xAA(gb_cpu *cpu);
int ext_op_0xAB(gb_cpu *cpu);
int ext_op_0xAC(gb_cpu *cpu);
int ext_op_0xAD(gb_cpu *cpu);
int ext_op_0xAE(gb_cpu *cpu);
int ext_op_0xAF(gb_cpu *cpu);

int ext_op_0xB0(gb_cpu *cpu);
int ext_op_0xB1(gb_cpu *cpu);
int ext_op_0xB2(gb_cpu *cpu);
int ext_op_0xB3(gb_cpu *cpu);
int ext_op_0xB4(gb_cpu *cpu);
int ext_op_0xB5(gb_cpu *cpu);
int ext_op_0xB6(gb_cpu *cpu);
int ext_op_0xB7(gb_cpu *cpu);
int ext_op_0xB8(gb_cpu *cpu);
int ext_op_0xB9(gb_cpu *cpu);
int ext_op_0xBA(gb_cpu *cpu);
int ext_op_0xBB(gb_cpu *cpu);
int ext_op_0xBC(gb_cpu *cpu);
int ext_op_0xBD(gb_cpu *cpu);
int ext_op_0xBE(gb_cpu *cpu);
int ext_op_0xBF(gb_cpu *cpu);

int ext_op_0xC0(gb_cpu *cpu);
int ext_op_0xC1(gb_cpu *cpu);
int ext_op_0xC2(gb_cpu *cpu);
int ext_op_0xC3(gb_cpu *cpu);
int ext_op_0xC4(gb_cpu *cpu);
int ext_op_0xC5(gb_cpu *cpu);
int ext_op_0xC6(gb_cpu *cpu);
int ext_op_0xC7(gb_cpu *cpu);
int ext_op_0xC8(gb_cpu *cpu);
int ext_op_0xC9(gb_cpu *cpu);
int ext_op_0xCA(gb_cpu *cpu);
int ext_op_0xCB(gb_cpu *cpu);
int ext_op_0xCC(gb_cpu *cpu);
int ext_op_0xCD(gb_cpu *cpu);
int ext_op_0xCE(gb_cpu *cpu);
int ext_op_0xCF(gb_cpu *cpu);

int ext_op_0xD0(gb_cpu *cpu);
int ext_op_0xD1(gb_cpu *cpu);
int ext_op_0xD2(gb_cpu *cpu);
int ext_op_0xD3(gb_cpu *cpu);
int ext_op_0xD4(gb_cpu *cpu);
int ext_op_0xD5(gb_cpu *cpu);
int ext_op_0xD6(gb_cpu *cpu);
int ext_op_0xD7(gb_cpu *cpu);
int ext_op_0xD8(gb_cpu *cpu);
int ext_op_0xD9(gb_cpu *cpu);
int ext_op_0xDA(gb_cpu *cpu);
int ext_op_0xDB(gb_cpu *cpu);
int ext_op_0xDC(gb_cpu *cpu);
int ext_op_0xDD(gb_cpu *cpu);
int ext_op_0xDE(gb_cpu *cpu);
int ext_op_0xDF(gb_cpu *cpu);

int ext_op_0xE0(gb_cpu *cpu);
int ext_op_0xE1(gb_cpu *cpu);
int ext_op_0xE2(gb_cpu *cpu);
int ext_op_0xE3(gb_cpu *cpu);
int ext_op_0xE4(gb_cpu *cpu);
int ext_op_0xE5(gb_cpu *cpu);
int ext_op_0xE6(gb_cpu *cpu);
int ext_op_0xE7(gb_cpu *cpu);
int ext_op_0xE8(gb_cpu *cpu);
int ext_op_0xE9(gb_cpu *cpu);
int ext_op_0xEA(gb_cpu *cpu);
int ext_op_0xEB(gb_cpu *cpu);
int ext_op_0xEC(gb_cpu *cpu);
int ext_op_0xED(gb_cpu *cpu);
int ext_op_0xEE(gb_cpu *cpu);
int ext_op_0xEF(gb_cpu *cpu);

int ext_op_0xF0(gb_cpu *cpu);
int ext_op_0xF1(gb_cpu *cpu);
int ext_op_0xF2(gb_cpu *cpu);
int ext_op_0xF3(gb_cpu *cpu);
int ext_op_0xF4(gb_cpu *cpu);
int ext_op_0xF5(gb_cpu *cpu);
int ext_op_0xF6(gb_cpu *cpu);
int ext_op_0xF7(gb_cpu *cpu);
int ext_op_0xF8(gb_cpu *cpu);
int ext_op_0xF9(gb_cpu *cpu);
int ext_op_0xFA(gb_cpu *cpu);
int ext_op_0xFB(gb_cpu *cpu);
int ext_op_0xFC(gb_cpu *cpu);
int ext_op_0xFD(gb_cpu *cpu);
int ext_op_0xFE(gb_cpu *cpu);
int ext_op_0xFF(gb_cpu *cpu);

