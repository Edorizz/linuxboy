/*
 * Linuxboy - Nintendo Game Boy Emulator
 * Copyright (C) 2017  Edgar Mendoza

 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

/* Header file */
#include "opcodes.h"
/* C Library */
#include <stdlib.h>
#include <string.h>
/* Linuxboy */
#include "cpu.h"
#include "utils.h"
#include "timers.h"

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
			{ "DAA", 1, op_0x27 },
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
			{ "PREFIX CB", 2, op_0xCB },
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
			{ "ADD SP,%02x", 2 | SIGNED, op_0xE8 },
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
			{ "LD HL,SP+%02x", 2 | SIGNED, op_0xF8 },
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
			    
			    { "RL B", 2, ext_op_0x10 },
			    { "RL C", 2, ext_op_0x11 },
			    { "RL D", 2, ext_op_0x12 },
			    { "RL E", 2, ext_op_0x13 },
			    { "RL H", 2, ext_op_0x14 },
			    { "RL L", 2, ext_op_0x15 },
			    { "RL (HL)", 2, ext_op_0x16 },
			    { "RL A", 2, ext_op_0x17 },
			    { "RR B", 2, ext_op_0x18 },
			    { "RR C", 2, ext_op_0x19 },
			    { "RR D", 2, ext_op_0x1A },
			    { "RR E", 2, ext_op_0x1B },
			    { "RR H", 2, ext_op_0x1C },
			    { "RR L", 2, ext_op_0x1D },
			    { "RR (HL)", 2, ext_op_0x1E },
			    { "RR A", 2, ext_op_0x1F },
			    
			    { "SLA B", 2, ext_op_0x20 },
			    { "SLA C", 2, ext_op_0x21 },
			    { "SLA D", 2, ext_op_0x22 },
			    { "SLA E", 2, ext_op_0x23 },
			    { "SLA H", 2, ext_op_0x24 },
			    { "SLA L", 2, ext_op_0x25 },
			    { "SLA (HL)", 2, ext_op_0x26 },
			    { "SLA A", 2, ext_op_0x27 },
			    { "SRA B", 2, ext_op_0x28 },
			    { "SRA C", 2, ext_op_0x29 },
			    { "SRA D", 2, ext_op_0x2A },
			    { "SRA E", 2, ext_op_0x2B },
			    { "SRA H", 2, ext_op_0x2C },
			    { "SRA L", 2, ext_op_0x2D },
			    { "SRA (HL)", 2, ext_op_0x2E },
			    { "SRA A", 2, ext_op_0x2F },
			    
			    { "SWAP B", 2, ext_op_0x30 },
			    { "SWAP C", 2, ext_op_0x31 },
			    { "SWAP D", 2, ext_op_0x32 },
			    { "SWAP E", 2, ext_op_0x33 },
			    { "SWAP H", 2, ext_op_0x34 },
			    { "SWAP L", 2, ext_op_0x35 },
			    { "SWAP (HL)", 2, ext_op_0x36 },
			    { "SWAP A", 2, ext_op_0x37 },
			    { "SRL B", 2, ext_op_0x38 },
			    { "SRL C", 2, ext_op_0x39 },
			    { "SRL D", 2, ext_op_0x3A },
			    { "SRL E", 2, ext_op_0x3B },
			    { "SRL H", 2, ext_op_0x3C },
			    { "SRL L", 2, ext_op_0x3D },
			    { "SRL (HL)", 2, ext_op_0x3E },
			    { "SRL A", 2, ext_op_0x3F },
			    
			    { "BIT 0,B", 2, ext_op_0x40 },
			    { "BIT 0,C", 2, ext_op_0x41 },
			    { "BIT 0,D", 2, ext_op_0x42 },
			    { "BIT 0,E", 2, ext_op_0x43 },
			    { "BIT 0,H", 2, ext_op_0x44 },
			    { "BIT 0,L", 2, ext_op_0x45 },
			    { "BIT 0,(HL)", 2, ext_op_0x46 },
			    { "BIT 0,A", 2, ext_op_0x47 },
			    { "BIT 1,B", 2, ext_op_0x48 },
			    { "BIT 1,C", 2, ext_op_0x49 },
			    { "BIT 1,D", 2, ext_op_0x4A },
			    { "BIT 1,E", 2, ext_op_0x4B },
			    { "BIT 1,H", 2, ext_op_0x4C },
			    { "BIT 1,L", 2, ext_op_0x4D },
			    { "BIT 1,(HL)", 2, ext_op_0x4E },
			    { "BIT 1,A", 2, ext_op_0x4F },
			    
			    { "BIT 2,B", 2, ext_op_0x50 },
			    { "BIT 2,C", 2, ext_op_0x51 },
			    { "BIT 2,D", 2, ext_op_0x52 },
			    { "BIT 2,E", 2, ext_op_0x53 },
			    { "BIT 2,H", 2, ext_op_0x54 },
			    { "BIT 2,L", 2, ext_op_0x55 },
			    { "BIT 2,(HL)", 2, ext_op_0x56 },
			    { "BIT 2,A", 2, ext_op_0x57 },
			    { "BIT 3,B", 2, ext_op_0x58 },
			    { "BIT 3,C", 2, ext_op_0x59 },
			    { "BIT 3,D", 2, ext_op_0x5A },
			    { "BIT 3,E", 2, ext_op_0x5B },
			    { "BIT 3,H", 2, ext_op_0x5C },
			    { "BIT 3,L", 2, ext_op_0x5D },
			    { "BIT 3,(HL)", 2, ext_op_0x5E },
			    { "BIT 3,A", 2, ext_op_0x5F },
			    
			    { "BIT 4,B", 2, ext_op_0x60 },
			    { "BIT 4,C", 2, ext_op_0x61 },
			    { "BIT 4,D", 2, ext_op_0x62 },
			    { "BIT 4,E", 2, ext_op_0x63 },
			    { "BIT 4,H", 2, ext_op_0x64 },
			    { "BIT 4,L", 2, ext_op_0x65 },
			    { "BIT 4,(HL)", 2, ext_op_0x66 },
			    { "BIT 4,A", 2, ext_op_0x67 },
			    { "BIT 5,B", 2, ext_op_0x68 },
			    { "BIT 5,C", 2, ext_op_0x69 },
			    { "BIT 5,D", 2, ext_op_0x6A },
			    { "BIT 5,E", 2, ext_op_0x6B },
			    { "BIT 5,H", 2, ext_op_0x6C },
			    { "BIT 5,L", 2, ext_op_0x6D },
			    { "BIT 5,(HL)", 2, ext_op_0x6E },
			    { "BIT 5,A", 2, ext_op_0x6F },
			    
			    { "BIT 6,B", 2, ext_op_0x70 },
			    { "BIT 6,C", 2, ext_op_0x71 },
			    { "BIT 6,D", 2, ext_op_0x72 },
			    { "BIT 6,E", 2, ext_op_0x73 },
			    { "BIT 6,H", 2, ext_op_0x74 },
			    { "BIT 6,L", 2, ext_op_0x75 },
			    { "BIT 6,(HL)", 2, ext_op_0x76 },
			    { "BIT 6,A", 2, ext_op_0x77 },
			    { "BIT 7,B", 2, ext_op_0x78 },
			    { "BIT 7,C", 2, ext_op_0x79 },
			    { "BIT 7,D", 2, ext_op_0x7A },
			    { "BIT 7,E", 2, ext_op_0x7B },
			    { "BIT 7,H", 2, ext_op_0x7C },
			    { "BIT 7,L", 2, ext_op_0x7D },
			    { "BIT 7,(HL)", 2, ext_op_0x7E },
			    { "BIT 7,A", 2, ext_op_0x7F },
			    
			    { "RES 0,B", 2, ext_op_0x80 },
			    { "RES 0,C", 2, ext_op_0x81 },
			    { "RES 0,D", 2, ext_op_0x82 },
			    { "RES 0,E", 2, ext_op_0x83 },
			    { "RES 0,H", 2, ext_op_0x84 },
			    { "RES 0,L", 2, ext_op_0x85 },
			    { "RES 0,(HL)", 2, ext_op_0x86 },
			    { "RES 0,A", 2, ext_op_0x87 },
			    { "RES 1,B", 2, ext_op_0x88 },
			    { "RES 1,C", 2, ext_op_0x89 },
			    { "RES 1,D", 2, ext_op_0x8A },
			    { "RES 1,E", 2, ext_op_0x8B },
			    { "RES 1,H", 2, ext_op_0x8C },
			    { "RES 1,L", 2, ext_op_0x8D },
			    { "RES 1,(HL)", 2, ext_op_0x8E },
			    { "RES 1,A", 2, ext_op_0x8F },
			    
			    { "RES 2,B", 2, ext_op_0x90 },
			    { "RES 2,C", 2, ext_op_0x91 },
			    { "RES 2,D", 2, ext_op_0x92 },
			    { "RES 2,E", 2, ext_op_0x93 },
			    { "RES 2,H", 2, ext_op_0x94 },
			    { "RES 2,L", 2, ext_op_0x95 },
			    { "RES 2,(HL)", 2, ext_op_0x96 },
			    { "RES 2,A", 2, ext_op_0x97 },
			    { "RES 3,B", 2, ext_op_0x98 },
			    { "RES 3,C", 2, ext_op_0x99 },
			    { "RES 3,D", 2, ext_op_0x9A },
			    { "RES 3,E", 2, ext_op_0x9B },
			    { "RES 3,H", 2, ext_op_0x9C },
			    { "RES 3,L", 2, ext_op_0x9D },
			    { "RES 3,(HL)", 2, ext_op_0x9E },
			    { "RES 3,A", 2, ext_op_0x9F },
			    
			    { "RES 4,B", 2, ext_op_0xA0 },
			    { "RES 4,C", 2, ext_op_0xA1 },
			    { "RES 4,D", 2, ext_op_0xA2 },
			    { "RES 4,E", 2, ext_op_0xA3 },
			    { "RES 4,H", 2, ext_op_0xA4 },
			    { "RES 4,L", 2, ext_op_0xA5 },
			    { "RES 4,(HL)", 2, ext_op_0xA6 },
			    { "RES 4,A", 2, ext_op_0xA7 },
			    { "RES 5,B", 2, ext_op_0xA8 },
			    { "RES 5,C", 2, ext_op_0xA9 },
			    { "RES 5,D", 2, ext_op_0xAA },
			    { "RES 5,E", 2, ext_op_0xAB },
			    { "RES 5,H", 2, ext_op_0xAC },
			    { "RES 5,L", 2, ext_op_0xAD },
			    { "RES 5,(HL)", 2, ext_op_0xAE },
			    { "RES 5,A", 2, ext_op_0xAF },
			    
			    { "RES 6,B", 2, ext_op_0xB0 },
			    { "RES 6,C", 2, ext_op_0xB1 },
			    { "RES 6,D", 2, ext_op_0xB2 },
			    { "RES 6,E", 2, ext_op_0xB3 },
			    { "RES 6,H", 2, ext_op_0xB4 },
			    { "RES 6,L", 2, ext_op_0xB5 },
			    { "RES 6,(HL)", 2, ext_op_0xB6 },
			    { "RES 6,A", 2, ext_op_0xB7 },
			    { "RES 7,B", 2, ext_op_0xB8 },
			    { "RES 7,C", 2, ext_op_0xB9 },
			    { "RES 7,D", 2, ext_op_0xBA },
			    { "RES 7,E", 2, ext_op_0xBB },
			    { "RES 7,H", 2, ext_op_0xBC },
			    { "RES 7,L", 2, ext_op_0xBD },
			    { "RES 7,(HL)", 2, ext_op_0xBE },
			    { "RES 7,A", 2, ext_op_0xBF },
			    
			    { "SET 0,B", 2, ext_op_0xC0 },
			    { "SET 0,C", 2, ext_op_0xC1 },
			    { "SET 0,D", 2, ext_op_0xC2 },
			    { "SET 0,E", 2, ext_op_0xC3 },
			    { "SET 0,H", 2, ext_op_0xC4 },
			    { "SET 0,L", 2, ext_op_0xC5 },
			    { "SET 0,(HL)", 2, ext_op_0xC6 },
			    { "SET 0,A", 2, ext_op_0xC7 },
			    { "SET 1,B", 2, ext_op_0xC8 },
			    { "SET 1,C", 2, ext_op_0xC9 },
			    { "SET 1,D", 2, ext_op_0xCA },
			    { "SET 1,E", 2, ext_op_0xCB },
			    { "SET 1,H", 2, ext_op_0xCC },
			    { "SET 1,L", 2, ext_op_0xCD },
			    { "SET 1,(HL)", 2, ext_op_0xCE },
			    { "SET 1,A", 2, ext_op_0xCF },
			    
			    { "SET 2,B", 2, ext_op_0xD0 },
			    { "SET 2,C", 2, ext_op_0xD1 },
			    { "SET 2,D", 2, ext_op_0xD2 },
			    { "SET 2,E", 2, ext_op_0xD3 },
			    { "SET 2,H", 2, ext_op_0xD4 },
			    { "SET 2,L", 2, ext_op_0xD5 },
			    { "SET 2,(HL)", 2, ext_op_0xD6 },
			    { "SET 2,A", 2, ext_op_0xD7 },
			    { "SET 3,B", 2, ext_op_0xD8 },
			    { "SET 3,C", 2, ext_op_0xD9 },
			    { "SET 3,D", 2, ext_op_0xDA },
			    { "SET 3,E", 2, ext_op_0xDB },
			    { "SET 3,H", 2, ext_op_0xDC },
			    { "SET 3,L", 2, ext_op_0xDD },
			    { "SET 3,(HL)", 2, ext_op_0xDE },
			    { "SET 3,A", 2, ext_op_0xDF },
			    
			    { "SET 4,B", 2, ext_op_0xE0 },
			    { "SET 4,C", 2, ext_op_0xE1 },
			    { "SET 4,D", 2, ext_op_0xE2 },
			    { "SET 4,E", 2, ext_op_0xE3 },
			    { "SET 4,H", 2, ext_op_0xE4 },
			    { "SET 4,L", 2, ext_op_0xE5 },
			    { "SET 4,(HL)", 2, ext_op_0xE6 },
			    { "SET 4,A", 2, ext_op_0xE7 },
			    { "SET 5,B", 2, ext_op_0xE8 },
			    { "SET 5,C", 2, ext_op_0xE9 },
			    { "SET 5,D", 2, ext_op_0xEA },
			    { "SET 5,E", 2, ext_op_0xEB },
			    { "SET 5,H", 2, ext_op_0xEC },
			    { "SET 5,L", 2, ext_op_0xED },
			    { "SET 5,(HL)", 2, ext_op_0xEE },
			    { "SET 5,A", 2, ext_op_0xEF },
			    
			    { "SET 6,B", 2, ext_op_0xF0 },
			    { "SET 6,C", 2, ext_op_0xF1 },
			    { "SET 6,D", 2, ext_op_0xF2 },
			    { "SET 6,E", 2, ext_op_0xF3 },
			    { "SET 6,H", 2, ext_op_0xF4 },
			    { "SET 6,L", 2, ext_op_0xF5 },
			    { "SET 6,(HL)", 2, ext_op_0xF6 },
			    { "SET 6,A", 2, ext_op_0xF7 },
			    { "SET 7,B", 2, ext_op_0xF8 },
			    { "SET 7,C", 2, ext_op_0xF9 },
			    { "SET 7,D", 2, ext_op_0xFA },
			    { "SET 7,E", 2, ext_op_0xFB },
			    { "SET 7,H", 2, ext_op_0xFC },
			    { "SET 7,L", 2, ext_op_0xFD },
			    { "SET 7,(HL)", 2, ext_op_0xFE },
			    { "SET 7,A", 2, ext_op_0xFF } };

/*
 * -==+ OPCODES +==-
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
	write_byte(cpu, a16, cpu->stack & 0xFF);
	write_byte(cpu, a16 + 1, cpu->stack >> 8);
	
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
	cpu->status = BIT(STOP);
	
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
	cpu->regs[REG_HL].hi = dec_byte(FLAG_P(cpu), cpu->regs[REG_HL].hi);
	
	return 4;
}

/* LD H,d8 */
int
op_0x26(gb_cpu *cpu, BYTE d8)
{
	cpu->regs[REG_HL].hi = d8;
	
	return 8;
}

/* DAA */
int
op_0x27(gb_cpu *cpu)
{
	WORD a = cpu->regs[REG_AF].hi;

	if (FLAG(cpu) & BIT(FLAG_N)) {
		if (FLAG(cpu) & BIT(FLAG_H)) {
			a = (a - 0x06) & 0xFF;
		}

		if (FLAG(cpu) & BIT(FLAG_C)) {
			a -= 0x60;
		}
	} else {
		if (FLAG(cpu) & BIT(FLAG_H) || (a & 0x0F) > 9) {
			a += 0x06;
		}

		if (FLAG(cpu) & BIT(FLAG_C) || a > 0x9F) {
			a += 0x60;
		}
	}

	cpu->regs[REG_AF].hi = a;
	RESET_FLAGS(FLAG(cpu), BIT(FLAG_H) | BIT(FLAG_Z));

	if (cpu->regs[REG_AF].hi == 0) {
		FLAG(cpu) |= BIT(FLAG_Z);
	}

	if (a > 0xFF) {
		FLAG(cpu) |= BIT(FLAG_C);
	}
	
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
	if (!(FLAG(cpu) & BIT(FLAG_C))) {
		cpu->pc += r8;
		return 12;
	}
	
	return 8;
}

/* LD SP,d16 */
int
op_0x31(gb_cpu *cpu, WORD d16)
{
	cpu->stack = d16;
	
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
	cpu->stack += 2;
	
	return 8;
}

/* INC (HL) */
int
op_0x34(gb_cpu *cpu)
{
	write_byte(cpu, cpu->regs[REG_HL].reg,
		   inc_byte(FLAG_P(cpu), read_byte(cpu, cpu->regs[REG_HL].reg)));
	
	return 12;
}

/* DEC (HL) */
int
op_0x35(gb_cpu *cpu)
{
	write_byte(cpu, cpu->regs[REG_HL].reg,
		   dec_byte(FLAG_P(cpu), read_byte(cpu, cpu->regs[REG_HL].reg)));
	
	return 12;
}

/* LD (HL),d8 */
int
op_0x36(gb_cpu *cpu, BYTE d8)
{
	write_byte(cpu, cpu->regs[REG_HL].reg, d8);
	
	return 12;
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
	add_word(FLAG_P(cpu), &cpu->regs[REG_HL].reg, *(WORD*)&cpu->memory[cpu->stack]);
	
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
	cpu->stack -= 2;
	
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
	cpu->status = BIT(HALT);
	
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
	add_byte(FLAG_P(cpu), &cpu->regs[REG_AF].hi, cpu->regs[REG_BC].hi);
	
	return 4;
}

/* ADD A,C */
int
op_0x81(gb_cpu *cpu)
{
	add_byte(FLAG_P(cpu), &cpu->regs[REG_AF].hi, cpu->regs[REG_BC].lo);
	
	return 4;
}

/* ADD A,D */
int
op_0x82(gb_cpu *cpu)
{
	add_byte(FLAG_P(cpu), &cpu->regs[REG_AF].hi, cpu->regs[REG_DE].hi);
	
	return 4;
}

/* ADD A,E */
int
op_0x83(gb_cpu *cpu)
{
	add_byte(FLAG_P(cpu), &cpu->regs[REG_AF].hi, cpu->regs[REG_DE].lo);
	
	return 4;
}

/* ADD A,H */
int
op_0x84(gb_cpu *cpu)
{
	add_byte(FLAG_P(cpu), &cpu->regs[REG_AF].hi, cpu->regs[REG_HL].hi);
	
	return 4;
}

/* ADD A,L */
int
op_0x85(gb_cpu *cpu)
{
	add_byte(FLAG_P(cpu), &cpu->regs[REG_AF].hi, cpu->regs[REG_HL].lo);
	
	return 4;
}

/* ADD A,(HL) */
int
op_0x86(gb_cpu *cpu)
{
	add_byte(FLAG_P(cpu), &cpu->regs[REG_AF].hi,
		 read_byte(cpu, cpu->regs[REG_HL].reg));
	
	return 8;
}

/* ADD A,A */
int
op_0x87(gb_cpu *cpu)
{
	add_byte(FLAG_P(cpu), &cpu->regs[REG_AF].hi, cpu->regs[REG_AF].hi);
	
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
		 cpu->regs[REG_BC].hi + ((FLAG(cpu) & BIT(FLAG_C)) >> FLAG_C));
	
	return 4;
}

/* SBC A,C */
int
op_0x99(gb_cpu *cpu)
{
	sub_byte(FLAG_P(cpu), &cpu->regs[REG_AF].hi,
		 cpu->regs[REG_BC].lo + ((FLAG(cpu) & BIT(FLAG_C)) >> FLAG_C));
	
	return 4;
}

/* SBC A,D */
int
op_0x9A(gb_cpu *cpu)
{
	sub_byte(FLAG_P(cpu), &cpu->regs[REG_AF].hi,
		 cpu->regs[REG_DE].hi + ((FLAG(cpu) & BIT(FLAG_C)) >> FLAG_C));
	
	return 4;
}

/* SBC A,E */
int
op_0x9B(gb_cpu *cpu)
{
	sub_byte(FLAG_P(cpu), &cpu->regs[REG_AF].hi,
		 cpu->regs[REG_DE].lo + ((FLAG(cpu) & BIT(FLAG_C)) >> FLAG_C));
	
	return 4;
}

/* SBC A,H */
int
op_0x9C(gb_cpu *cpu)
{
	sub_byte(FLAG_P(cpu), &cpu->regs[REG_AF].hi,
		 cpu->regs[REG_HL].hi + ((FLAG(cpu) & BIT(FLAG_C)) >> FLAG_C));
	
	return 4;
}

/* SBC A,L */
int
op_0x9D(gb_cpu *cpu)
{
	sub_byte(FLAG_P(cpu), &cpu->regs[REG_AF].hi,
		 cpu->regs[REG_HL].lo + ((FLAG(cpu) & BIT(FLAG_C)) >> FLAG_C));
	
	return 4;
}

/* SBC A,(HL) */
int
op_0x9E(gb_cpu *cpu)
{
	sub_byte(FLAG_P(cpu), &cpu->regs[REG_AF].hi,
		 read_byte(cpu, cpu->regs[REG_HL].reg) + ((FLAG(cpu) & BIT(FLAG_C)) >> FLAG_C));
	
	return 8;
}

/* SBC A,A */
int
op_0x9F(gb_cpu *cpu)
{
	sub_byte(FLAG_P(cpu), &cpu->regs[REG_AF].hi,
		 cpu->regs[REG_AF].hi + ((FLAG(cpu) & BIT(FLAG_C)) >> FLAG_C));
	
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

	opcode = &ext_ops[cpu->memory[cpu->pc - 1]];
	
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
	WORD result = cpu->stack + r8;
	
	RESET_FLAGS(FLAG(cpu), BIT(FLAG_Z) | BIT(FLAG_N) | BIT(FLAG_H) | BIT(FLAG_C));

	if (((cpu->stack ^ r8 ^ result) & 0x100) == 0x100) {
		FLAG(cpu) |= BIT(FLAG_C);
	}

	if (((cpu->stack ^ r8 ^ result) & 0x10) == 0x10) {
		FLAG(cpu) |= BIT(FLAG_H);
	}

	cpu->stack = result;
	
	return 16;
}

/* JP (HL) */
int
op_0xE9(gb_cpu *cpu)
{
	cpu->pc = cpu->regs[REG_HL].reg;
	
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
	cpu->regs[REG_AF].lo &= 0xF0;
	
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
op_0xF8(gb_cpu *cpu, SIGNED_BYTE r8)
{
	WORD result = cpu->stack + r8;
	
	RESET_FLAGS(FLAG(cpu), BIT(FLAG_Z) | BIT(FLAG_N) | BIT(FLAG_H) | BIT(FLAG_C));

	if (((cpu->stack ^ r8 ^ result) & 0x100) == 0x100) {
		FLAG(cpu) |= BIT(FLAG_C);
	}

	if (((cpu->stack ^ r8 ^ result) & 0x10) == 0x10) {
		FLAG(cpu) |= BIT(FLAG_H);
	}

	cpu->regs[REG_HL].reg = result;

	return 12;
}

/* LD SP,HL */
int
op_0xF9(gb_cpu *cpu)
{
	/*cpu->stack->reg = cpu->regs[REG_HL].reg;*/
	cpu->stack = cpu->regs[REG_HL].reg;
	
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

/*
 * -==+ EXTENDED OPCODES +==-
 */

/* | BIT(FLAG_Z) ? */
/* RLC B */
int
ext_op_0x00(gb_cpu *cpu)
{
	rot_byte(FLAG_P(cpu), &cpu->regs[REG_BC].hi, LEFT | CIRCULAR);
	
	return 8;
}

/* RLC C */
int
ext_op_0x01(gb_cpu *cpu)
{
	rot_byte(FLAG_P(cpu), &cpu->regs[REG_BC].lo, LEFT | CIRCULAR);
	
	return 8;
}

/* RLC D */
int
ext_op_0x02(gb_cpu *cpu)
{
	rot_byte(FLAG_P(cpu), &cpu->regs[REG_DE].hi, LEFT | CIRCULAR);
	
	return 8;
}

/* RLC E */
int
ext_op_0x03(gb_cpu *cpu)
{
	rot_byte(FLAG_P(cpu), &cpu->regs[REG_DE].lo, LEFT | CIRCULAR);
	
	return 8;
}

/* RLC H */
int
ext_op_0x04(gb_cpu *cpu)
{
	rot_byte(FLAG_P(cpu), &cpu->regs[REG_HL].hi, LEFT | CIRCULAR);
	
	return 8;
}

/* RLC L */
int
ext_op_0x05(gb_cpu *cpu)
{
	rot_byte(FLAG_P(cpu), &cpu->regs[REG_HL].lo, LEFT | CIRCULAR);
	
	return 8;
}

/* RLC (HL) */
int
ext_op_0x06(gb_cpu *cpu)
{
	rot_byte(FLAG_P(cpu),
		 &cpu->memory[cpu->regs[REG_HL].reg], LEFT | CIRCULAR);
	
	return 16;
}

/* RLC A */
int
ext_op_0x07(gb_cpu *cpu)
{
	rot_byte(FLAG_P(cpu), &cpu->regs[REG_AF].hi, LEFT | CIRCULAR);
	
	return 8;
}

/* RRC B */
int
ext_op_0x08(gb_cpu *cpu)
{
	rot_byte(FLAG_P(cpu), &cpu->regs[REG_BC].hi, RIGHT | CIRCULAR);
	
	return 8;
}

/* RRC C */
int
ext_op_0x09(gb_cpu *cpu)
{
	rot_byte(FLAG_P(cpu), &cpu->regs[REG_BC].lo, RIGHT | CIRCULAR);
	
	return 8;
}

/* RRC D */
int
ext_op_0x0A(gb_cpu *cpu)
{
	rot_byte(FLAG_P(cpu), &cpu->regs[REG_DE].hi, RIGHT | CIRCULAR);
	
	return 8;
}

/* RRC E */
int
ext_op_0x0B(gb_cpu *cpu)
{
	rot_byte(FLAG_P(cpu), &cpu->regs[REG_DE].lo, RIGHT | CIRCULAR);
	
	return 8;
}

/* RRC H */
int
ext_op_0x0C(gb_cpu *cpu)
{
	rot_byte(FLAG_P(cpu), &cpu->regs[REG_HL].hi, RIGHT | CIRCULAR);
	
	return 8;
}

/* RRC L */
int
ext_op_0x0D(gb_cpu *cpu)
{
	rot_byte(FLAG_P(cpu), &cpu->regs[REG_HL].lo, RIGHT | CIRCULAR);
	
	return 8;
}

/* RRC (HL) */
int
ext_op_0x0E(gb_cpu *cpu)
{
	rot_byte(FLAG_P(cpu),
		 &cpu->memory[cpu->regs[REG_HL].reg], RIGHT | CIRCULAR);
	
	return 16;
}

/* RRC A */
int
ext_op_0x0F(gb_cpu *cpu)
{
	rot_byte(FLAG_P(cpu), &cpu->regs[REG_AF].hi, RIGHT | CIRCULAR);
	
	return 8;
}

/* RL B */
int
ext_op_0x10(gb_cpu *cpu)
{
	rot_byte(FLAG_P(cpu), &cpu->regs[REG_BC].hi, LEFT);
	
	return 8;
}

/* RL C */
int
ext_op_0x11(gb_cpu *cpu)
{
	rot_byte(FLAG_P(cpu), &cpu->regs[REG_BC].lo, LEFT);
	
	return 8;
}

/* RL D */
int
ext_op_0x12(gb_cpu *cpu)
{
	rot_byte(FLAG_P(cpu), &cpu->regs[REG_DE].hi, LEFT);
	
	return 8;
}

/* RL E */
int
ext_op_0x13(gb_cpu *cpu)
{
	rot_byte(FLAG_P(cpu), &cpu->regs[REG_DE].lo, LEFT);
	
	return 8;
}

/* RL H */
int
ext_op_0x14(gb_cpu *cpu)
{
	rot_byte(FLAG_P(cpu), &cpu->regs[REG_HL].hi, LEFT);
	
	return 8;
}

/* RL L */
int
ext_op_0x15(gb_cpu *cpu)
{
	rot_byte(FLAG_P(cpu), &cpu->regs[REG_HL].lo, LEFT);
	
	return 8;
}

/* RL (HL) */
int
ext_op_0x16(gb_cpu *cpu)
{
	rot_byte(FLAG_P(cpu),
		 &cpu->memory[cpu->regs[REG_HL].reg], LEFT);
	
	return 16;
}

/* RL A */
int
ext_op_0x17(gb_cpu *cpu)
{
	rot_byte(FLAG_P(cpu), &cpu->regs[REG_AF].hi, LEFT);
	
	return 8;
}

/* RR B */
int
ext_op_0x18(gb_cpu *cpu)
{
	rot_byte(FLAG_P(cpu), &cpu->regs[REG_BC].hi, RIGHT);
	
	return 8;
}

/* RR C */
int
ext_op_0x19(gb_cpu *cpu)
{
	rot_byte(FLAG_P(cpu), &cpu->regs[REG_BC].lo, RIGHT);
	
	return 8;
}

/* RR D */
int
ext_op_0x1A(gb_cpu *cpu)
{
	rot_byte(FLAG_P(cpu), &cpu->regs[REG_DE].hi, RIGHT);
	
	return 8;
}

/* RR E */
int
ext_op_0x1B(gb_cpu *cpu)
{
	rot_byte(FLAG_P(cpu), &cpu->regs[REG_DE].lo, RIGHT);
	
	return 8;
}

/* RR H */
int
ext_op_0x1C(gb_cpu *cpu)
{
	rot_byte(FLAG_P(cpu), &cpu->regs[REG_HL].hi, RIGHT);
	
	return 8;
}

/* RR L */
int
ext_op_0x1D(gb_cpu *cpu)
{
	rot_byte(FLAG_P(cpu), &cpu->regs[REG_HL].lo, RIGHT);
	
	return 8;
}

/* RR (HL) */
int
ext_op_0x1E(gb_cpu *cpu)
{
	rot_byte(FLAG_P(cpu),
		 &cpu->memory[cpu->regs[REG_HL].reg], RIGHT);
	
	return 16;
}

/* RR A */
int
ext_op_0x1F(gb_cpu *cpu)
{
	rot_byte(FLAG_P(cpu), &cpu->regs[REG_AF].hi, RIGHT);
	
	return 8;
}

/* SLA B */
int
ext_op_0x20(gb_cpu *cpu)
{
	shift_byte(FLAG_P(cpu), &cpu->regs[REG_BC].hi, BIT(LEFT) | BIT(ARITHMETIC));

	return 8;
}

/* SLA C */
int
ext_op_0x21(gb_cpu *cpu)
{
	shift_byte(FLAG_P(cpu), &cpu->regs[REG_BC].lo, BIT(LEFT) | BIT(ARITHMETIC));

	return 8;
}

/* SLA D */
int
ext_op_0x22(gb_cpu *cpu)
{
	shift_byte(FLAG_P(cpu), &cpu->regs[REG_DE].hi, BIT(LEFT) | BIT(ARITHMETIC));

	return 8;
}
	
/* SLA E */
int
ext_op_0x23(gb_cpu *cpu)
{
	shift_byte(FLAG_P(cpu), &cpu->regs[REG_DE].lo, BIT(LEFT) | BIT(ARITHMETIC));

	return 8;
}

/* SLA H */
int
ext_op_0x24(gb_cpu *cpu)
{
	shift_byte(FLAG_P(cpu), &cpu->regs[REG_HL].hi, BIT(LEFT) | BIT(ARITHMETIC));

	return 8;
}

/* SLA L */
int
ext_op_0x25(gb_cpu *cpu)
{
	shift_byte(FLAG_P(cpu), &cpu->regs[REG_HL].lo, BIT(LEFT) | BIT(ARITHMETIC));

	return 8;
}

/* SLA (HL) */
int
ext_op_0x26(gb_cpu *cpu)
{
	shift_byte(FLAG_P(cpu),
		   &cpu->memory[cpu->regs[REG_HL].reg], BIT(LEFT) | BIT(ARITHMETIC));

	return 16;
}

/* SLA A */
int
ext_op_0x27(gb_cpu *cpu)
{
	shift_byte(FLAG_P(cpu), &cpu->regs[REG_AF].hi, BIT(LEFT) | BIT(ARITHMETIC));

	return 8;
}

/* SRA B */
int
ext_op_0x28(gb_cpu *cpu)
{
	shift_byte(FLAG_P(cpu), &cpu->regs[REG_BC].hi, BIT(RIGHT) | BIT(ARITHMETIC));

	return 8;
}

/* SRA C */
int
ext_op_0x29(gb_cpu *cpu)
{
	shift_byte(FLAG_P(cpu), &cpu->regs[REG_BC].lo, BIT(RIGHT) | BIT(ARITHMETIC));

	return 8;
}

/* SRA D */
int
ext_op_0x2A(gb_cpu *cpu)
{
	shift_byte(FLAG_P(cpu), &cpu->regs[REG_DE].hi, BIT(RIGHT) | BIT(ARITHMETIC));

	return 8;
}

/* SRA E */
int
ext_op_0x2B(gb_cpu *cpu)
{
	shift_byte(FLAG_P(cpu), &cpu->regs[REG_DE].lo, BIT(RIGHT) | BIT(ARITHMETIC));

	return 8;
}

/* SRA H */
int
ext_op_0x2C(gb_cpu *cpu)
{
	shift_byte(FLAG_P(cpu), &cpu->regs[REG_HL].hi, BIT(RIGHT) | BIT(ARITHMETIC));

	return 8;
}

/* SRA L */
int
ext_op_0x2D(gb_cpu *cpu)
{
	shift_byte(FLAG_P(cpu), &cpu->regs[REG_HL].lo, BIT(RIGHT) | BIT(ARITHMETIC));

	return 8;
}

/* SRA (HL) */
int
ext_op_0x2E(gb_cpu *cpu)
{
	shift_byte(FLAG_P(cpu),
		   &cpu->memory[cpu->regs[REG_HL].reg], BIT(RIGHT) | BIT(ARITHMETIC));

	return 16;
}

/* SRA A */
int
ext_op_0x2F(gb_cpu *cpu)
{
	shift_byte(FLAG_P(cpu), &cpu->regs[REG_AF].hi, BIT(RIGHT) | BIT(ARITHMETIC));

	return 8;
}

/* SWAP B */
int
ext_op_0x30(gb_cpu *cpu)
{
	swap_byte(FLAG_P(cpu), &cpu->regs[REG_BC].hi);
	
	return 8;
}

/* SWAP C */
int
ext_op_0x31(gb_cpu *cpu)
{
	swap_byte(FLAG_P(cpu), &cpu->regs[REG_BC].lo);
	
	return 8;
}

/* SWAP D */
int
ext_op_0x32(gb_cpu *cpu)
{
	swap_byte(FLAG_P(cpu), &cpu->regs[REG_DE].hi);
	
	return 8;
}

/* SWAP E */
int
ext_op_0x33(gb_cpu *cpu)
{
	swap_byte(FLAG_P(cpu), &cpu->regs[REG_DE].lo);
	
	return 8;
}

/* SWAP H */
int
ext_op_0x34(gb_cpu *cpu)
{
	swap_byte(FLAG_P(cpu), &cpu->regs[REG_HL].hi);
	
	return 8;
}

/* SWAP L */
int
ext_op_0x35(gb_cpu *cpu)
{
	swap_byte(FLAG_P(cpu), &cpu->regs[REG_HL].lo);
	
	return 8;
}

/* SWAP (HL) */
int
ext_op_0x36(gb_cpu *cpu)
{
	swap_byte(FLAG_P(cpu),
		  &cpu->memory[cpu->regs[REG_HL].reg]);
	
	return 16;
}

/* SWAP A */
int
ext_op_0x37(gb_cpu *cpu)
{
	swap_byte(FLAG_P(cpu), &cpu->regs[REG_AF].hi);
	
	return 8;
}

/* SRL B */
int
ext_op_0x38(gb_cpu *cpu)
{
	shift_byte(FLAG_P(cpu), &cpu->regs[REG_BC].hi, BIT(RIGHT));

	return 8;
}

/* SRL C */
int
ext_op_0x39(gb_cpu *cpu)
{
	shift_byte(FLAG_P(cpu), &cpu->regs[REG_BC].lo, BIT(RIGHT));

	return 8;
}

/* SRL D */
int
ext_op_0x3A(gb_cpu *cpu)
{
	shift_byte(FLAG_P(cpu), &cpu->regs[REG_DE].hi, BIT(RIGHT));

	return 8;
}

/* SRL E */
int
ext_op_0x3B(gb_cpu *cpu)
{
	shift_byte(FLAG_P(cpu), &cpu->regs[REG_DE].lo, BIT(RIGHT));

	return 8;
}

/* SRL H */
int
ext_op_0x3C(gb_cpu *cpu)
{
	shift_byte(FLAG_P(cpu), &cpu->regs[REG_HL].hi, BIT(RIGHT));

	return 8;
}

/* SRL L */
int
ext_op_0x3D(gb_cpu *cpu)
{
	shift_byte(FLAG_P(cpu), &cpu->regs[REG_HL].lo, BIT(RIGHT));

	return 8;
}

/* SRL (HL) */
int
ext_op_0x3E(gb_cpu *cpu)
{
	shift_byte(FLAG_P(cpu),
		   &cpu->memory[cpu->regs[REG_HL].reg], BIT(RIGHT));

	return 16;
}

/* SRL A */
int
ext_op_0x3F(gb_cpu *cpu)
{
	shift_byte(FLAG_P(cpu), &cpu->regs[REG_AF].hi, BIT(RIGHT));

	return 8;
}

/* BIT 0,B */
int
ext_op_0x40(gb_cpu *cpu)
{
	test_bit(FLAG_P(cpu), cpu->regs[REG_BC].hi, 0);

	return 8;
}

/* BIT 0,C */
int
ext_op_0x41(gb_cpu *cpu)
{
	test_bit(FLAG_P(cpu), cpu->regs[REG_BC].lo, 0);

	return 8;
}

/* BIT 0,D */
int
ext_op_0x42(gb_cpu *cpu)
{
	test_bit(FLAG_P(cpu), cpu->regs[REG_DE].hi, 0);

	return 8;
}

/* BIT 0,E */
int
ext_op_0x43(gb_cpu *cpu)
{
	test_bit(FLAG_P(cpu), cpu->regs[REG_DE].lo, 0);

	return 8;
}

/* BIT 0,H */
int
ext_op_0x44(gb_cpu *cpu)
{
	test_bit(FLAG_P(cpu), cpu->regs[REG_HL].hi, 0);

	return 8;
}

/* BIT 0,L */
int
ext_op_0x45(gb_cpu *cpu)
{
	test_bit(FLAG_P(cpu), cpu->regs[REG_HL].lo, 0);

	return 8;
}

/* BIT 0,(HL) */
int
ext_op_0x46(gb_cpu *cpu)
{
	test_bit(FLAG_P(cpu),
		 cpu->memory[cpu->regs[REG_HL].reg], 0);

	return 12;
}

/* BIT 0,A */
int
ext_op_0x47(gb_cpu *cpu)
{
	test_bit(FLAG_P(cpu), cpu->regs[REG_AF].hi, 0);

	return 8;
}

/* BIT 1,B */
int
ext_op_0x48(gb_cpu *cpu)
{
	test_bit(FLAG_P(cpu), cpu->regs[REG_BC].hi, 1);

	return 8;
}

/* BIT 1,C */
int
ext_op_0x49(gb_cpu *cpu)
{
	test_bit(FLAG_P(cpu), cpu->regs[REG_BC].lo, 1);

	return 8;
}

/* BIT 1,D */
int
ext_op_0x4A(gb_cpu *cpu)
{
	test_bit(FLAG_P(cpu), cpu->regs[REG_DE].hi, 1);

	return 8;
}

/* BIT 1,E */
int
ext_op_0x4B(gb_cpu *cpu)
{
	test_bit(FLAG_P(cpu), cpu->regs[REG_DE].lo, 1);

	return 8;
}

/* BIT 1,H */
int
ext_op_0x4C(gb_cpu *cpu)
{
	test_bit(FLAG_P(cpu), cpu->regs[REG_HL].hi, 1);

	return 8;
}

/* BIT 1,L */
int
ext_op_0x4D(gb_cpu *cpu)
{
	test_bit(FLAG_P(cpu), cpu->regs[REG_HL].lo, 1);

	return 8;
}

/* BIT 1,(HL) */
int
ext_op_0x4E(gb_cpu *cpu)
{
	test_bit(FLAG_P(cpu),
		 cpu->memory[cpu->regs[REG_HL].reg], 1);

	return 12;
}

/* BIT 1,A */
int
ext_op_0x4F(gb_cpu *cpu)
{
	test_bit(FLAG_P(cpu), cpu->regs[REG_AF].hi, 1);

	return 8;
}

/* BIT 2,B */
int
ext_op_0x50(gb_cpu *cpu)
{
	test_bit(FLAG_P(cpu), cpu->regs[REG_BC].hi, 2);

	return 8;
}

/* BIT 2,C */
int
ext_op_0x51(gb_cpu *cpu)
{
	test_bit(FLAG_P(cpu), cpu->regs[REG_BC].lo, 2);

	return 8;
}

/* BIT 2,D */
int
ext_op_0x52(gb_cpu *cpu)
{
	test_bit(FLAG_P(cpu), cpu->regs[REG_DE].hi, 2);

	return 8;
}

/* BIT 2,E */
int
ext_op_0x53(gb_cpu *cpu)
{
	test_bit(FLAG_P(cpu), cpu->regs[REG_DE].lo, 2);

	return 8;
}

/* BIT 2,H */
int
ext_op_0x54(gb_cpu *cpu)
{
	test_bit(FLAG_P(cpu), cpu->regs[REG_HL].hi, 2);

	return 8;
}

/* BIT 2,L */
int
ext_op_0x55(gb_cpu *cpu)
{
	test_bit(FLAG_P(cpu), cpu->regs[REG_HL].lo, 2);

	return 8;
}

/* BIT 2,(HL) */
int
ext_op_0x56(gb_cpu *cpu)
{
	test_bit(FLAG_P(cpu),
		 cpu->memory[cpu->regs[REG_HL].reg], 2);

	return 12;
}

/* BIT 2,A */
int
ext_op_0x57(gb_cpu *cpu)
{
	test_bit(FLAG_P(cpu), cpu->regs[REG_AF].hi, 2);

	return 8;
}

/* BIT 3,B */
int
ext_op_0x58(gb_cpu *cpu)
{
	test_bit(FLAG_P(cpu), cpu->regs[REG_BC].hi, 3);

	return 8;
}

/* BIT 3,C */
int
ext_op_0x59(gb_cpu *cpu)
{
	test_bit(FLAG_P(cpu), cpu->regs[REG_BC].lo, 3);

	return 8;
}

/* BIT 3,D */
int
ext_op_0x5A(gb_cpu *cpu)
{
	test_bit(FLAG_P(cpu), cpu->regs[REG_DE].hi, 3);

	return 8;
}

/* BIT 3,E */
int
ext_op_0x5B(gb_cpu *cpu)
{
	test_bit(FLAG_P(cpu), cpu->regs[REG_DE].lo, 3);

	return 8;
}

/* BIT 3,H */
int
ext_op_0x5C(gb_cpu *cpu)
{
	test_bit(FLAG_P(cpu), cpu->regs[REG_HL].hi, 3);

	return 8;
}

/* BIT 3,L */
int
ext_op_0x5D(gb_cpu *cpu)
{
	test_bit(FLAG_P(cpu), cpu->regs[REG_HL].lo, 3);

	return 8;
}

/* BIT 3,(HL) */
int
ext_op_0x5E(gb_cpu *cpu)
{
	test_bit(FLAG_P(cpu),
		 cpu->memory[cpu->regs[REG_HL].reg], 3);

	return 12;
}

/* BIT 3,A */
int
ext_op_0x5F(gb_cpu *cpu)
{
	test_bit(FLAG_P(cpu), cpu->regs[REG_AF].hi, 3);

	return 8;
}

/* BIT 4,B */
int
ext_op_0x60(gb_cpu *cpu)
{
	test_bit(FLAG_P(cpu), cpu->regs[REG_BC].hi, 4);

	return 8;
}

/* BIT 4,C */
int
ext_op_0x61(gb_cpu *cpu)
{
	test_bit(FLAG_P(cpu), cpu->regs[REG_BC].lo, 4);

	return 8;
}

/* BIT 4,D */
int
ext_op_0x62(gb_cpu *cpu)
{
	test_bit(FLAG_P(cpu), cpu->regs[REG_DE].hi, 4);

	return 8;
}

/* BIT 4,E */
int
ext_op_0x63(gb_cpu *cpu)
{
	test_bit(FLAG_P(cpu), cpu->regs[REG_DE].lo, 4);

	return 8;
}

/* BIT 4,H */
int
ext_op_0x64(gb_cpu *cpu)
{
	test_bit(FLAG_P(cpu), cpu->regs[REG_HL].hi, 4);

	return 8;
}

/* BIT 4,L */
int
ext_op_0x65(gb_cpu *cpu)
{
	test_bit(FLAG_P(cpu), cpu->regs[REG_HL].lo, 4);

	return 8;
}

/* BIT 4,(HL) */
int
ext_op_0x66(gb_cpu *cpu)
{
	test_bit(FLAG_P(cpu),
		 cpu->memory[cpu->regs[REG_HL].reg], 4);

	return 12;
}

/* BIT 4,A */
int
ext_op_0x67(gb_cpu *cpu)
{
	test_bit(FLAG_P(cpu), cpu->regs[REG_AF].hi, 4);

	return 8;
}

/* BIT 5,B */
int
ext_op_0x68(gb_cpu *cpu)
{
	test_bit(FLAG_P(cpu), cpu->regs[REG_BC].hi, 5);

	return 8;
}

/* BIT 5,C */
int
ext_op_0x69(gb_cpu *cpu)
{
	test_bit(FLAG_P(cpu), cpu->regs[REG_BC].lo, 5);

	return 8;
}

/* BIT 5,D */
int
ext_op_0x6A(gb_cpu *cpu)
{
	test_bit(FLAG_P(cpu), cpu->regs[REG_DE].hi, 5);

	return 8;
}

/* BIT 5,E */
int
ext_op_0x6B(gb_cpu *cpu)
{
	test_bit(FLAG_P(cpu), cpu->regs[REG_DE].lo, 5);

	return 8;
}

/* BIT 5,H */
int
ext_op_0x6C(gb_cpu *cpu)
{
	test_bit(FLAG_P(cpu), cpu->regs[REG_HL].hi, 5);

	return 8;
}

/* BIT 5,L */
int
ext_op_0x6D(gb_cpu *cpu)
{
	test_bit(FLAG_P(cpu), cpu->regs[REG_HL].lo, 5);

	return 8;
}

/* BIT 5,(HL) */
int
ext_op_0x6E(gb_cpu *cpu)
{
	test_bit(FLAG_P(cpu),
		 cpu->memory[cpu->regs[REG_HL].reg], 5);

	return 12;
}

/* BIT 5,A */
int
ext_op_0x6F(gb_cpu *cpu)
{
	test_bit(FLAG_P(cpu), cpu->regs[REG_AF].hi, 5);

	return 8;
}

/* BIT 6,B */
int
ext_op_0x70(gb_cpu *cpu)
{
	test_bit(FLAG_P(cpu), cpu->regs[REG_BC].hi, 6);

	return 8;
}

/* BIT 6,C */
int
ext_op_0x71(gb_cpu *cpu)
{
	test_bit(FLAG_P(cpu), cpu->regs[REG_BC].lo, 6);

	return 8;
}

/* BIT 6,D */
int
ext_op_0x72(gb_cpu *cpu)
{
	test_bit(FLAG_P(cpu), cpu->regs[REG_DE].hi, 6);

	return 8;
}

/* BIT 6,E */
int
ext_op_0x73(gb_cpu *cpu)
{
	test_bit(FLAG_P(cpu), cpu->regs[REG_DE].lo, 6);

	return 8;
}

/* BIT 6,H */
int
ext_op_0x74(gb_cpu *cpu)
{
	test_bit(FLAG_P(cpu), cpu->regs[REG_HL].hi, 6);

	return 8;
}

/* BIT 6,L */
int
ext_op_0x75(gb_cpu *cpu)
{
	test_bit(FLAG_P(cpu), cpu->regs[REG_HL].lo, 6);

	return 8;
}

/* BIT 6,(HL) */
int
ext_op_0x76(gb_cpu *cpu)
{
	test_bit(FLAG_P(cpu),
		 cpu->memory[cpu->regs[REG_HL].reg], 6);

	return 12;
}

/* BIT 6,A */
int
ext_op_0x77(gb_cpu *cpu)
{
	test_bit(FLAG_P(cpu), cpu->regs[REG_AF].hi, 6);

	return 8;
}

/* BIT 7,B */
int
ext_op_0x78(gb_cpu *cpu)
{
	test_bit(FLAG_P(cpu), cpu->regs[REG_BC].hi, 7);

	return 8;
}

/* BIT 7,C */
int
ext_op_0x79(gb_cpu *cpu)
{
	test_bit(FLAG_P(cpu), cpu->regs[REG_BC].lo, 7);

	return 8;
}

/* BIT 7,D */
int
ext_op_0x7A(gb_cpu *cpu)
{
	test_bit(FLAG_P(cpu), cpu->regs[REG_DE].hi, 7);

	return 8;
}

/* BIT 7,E */
int
ext_op_0x7B(gb_cpu *cpu)
{
	test_bit(FLAG_P(cpu), cpu->regs[REG_DE].lo, 7);

	return 8;
}

/* BIT 7,H */
int
ext_op_0x7C(gb_cpu *cpu)
{
	test_bit(FLAG_P(cpu), cpu->regs[REG_HL].hi, 7);

	return 8;
}

/* BIT 7,L */
int
ext_op_0x7D(gb_cpu *cpu)
{
	test_bit(FLAG_P(cpu), cpu->regs[REG_HL].lo, 7);

	return 8;
}

/* BIT 7,(HL) */
int
ext_op_0x7E(gb_cpu *cpu)
{
	test_bit(FLAG_P(cpu),
		 cpu->memory[cpu->regs[REG_HL].reg], 7);

	return 12;
}

/* BIT 7,A */
int
ext_op_0x7F(gb_cpu *cpu)
{
	test_bit(FLAG_P(cpu), cpu->regs[REG_AF].hi, 7);

	return 8;
}

/* RES 0,B */
int
ext_op_0x80(gb_cpu *cpu)
{
	cpu->regs[REG_BC].hi &= ~BIT(0);

	return 8;
}

/* RES 0,C */
int
ext_op_0x81(gb_cpu *cpu)
{
	cpu->regs[REG_BC].lo &= ~BIT(0);

	return 8;
}

/* RES 0,D */
int
ext_op_0x82(gb_cpu *cpu)
{
	cpu->regs[REG_DE].hi &= ~BIT(0);

	return 8;
}

/* RES 0,E */
int
ext_op_0x83(gb_cpu *cpu)
{
	cpu->regs[REG_DE].lo &= ~BIT(0);

	return 8;
}

/* RES 0,H */
int
ext_op_0x84(gb_cpu *cpu)
{
	cpu->regs[REG_HL].hi &= ~BIT(0);

	return 8;
}

/* RES 0,L */
int
ext_op_0x85(gb_cpu *cpu)
{
	cpu->regs[REG_HL].lo &= ~BIT(0);

	return 8;
}

/* RES 0,(HL) */
int
ext_op_0x86(gb_cpu *cpu)
{
	cpu->memory[cpu->regs[REG_HL].reg] &= ~BIT(0);

	return 16;
}

/* RES 0,A */
int
ext_op_0x87(gb_cpu *cpu)
{
	cpu->regs[REG_AF].hi &= ~BIT(0);

	return 8;
}

/* RES 1,B */
int
ext_op_0x88(gb_cpu *cpu)
{
	cpu->regs[REG_BC].hi &= ~BIT(1);

	return 8;
}

/* RES 1,C */
int
ext_op_0x89(gb_cpu *cpu)
{
	cpu->regs[REG_BC].lo &= ~BIT(1);

	return 8;
}

/* RES 1,D */
int
ext_op_0x8A(gb_cpu *cpu)
{
	cpu->regs[REG_DE].hi &= ~BIT(1);

	return 8;
}

/* RES 1,E */
int
ext_op_0x8B(gb_cpu *cpu)
{
	cpu->regs[REG_DE].lo &= ~BIT(1);

	return 8;
}

/* RES 1,H */
int
ext_op_0x8C(gb_cpu *cpu)
{
	cpu->regs[REG_HL].hi &= ~BIT(1);

	return 8;
}

/* RES 1,L */
int
ext_op_0x8D(gb_cpu *cpu)
{
	cpu->regs[REG_HL].lo &= ~BIT(1);

	return 8;
}

/* RES 1,(HL) */
int
ext_op_0x8E(gb_cpu *cpu)
{
	cpu->memory[cpu->regs[REG_HL].reg] &= ~BIT(1);

	return 16;
}

/* RES 1,A */
int
ext_op_0x8F(gb_cpu *cpu)
{
	cpu->regs[REG_AF].hi &= ~BIT(1);

	return 8;
}

/* RES 2,B */
int
ext_op_0x90(gb_cpu *cpu)
{
	cpu->regs[REG_BC].hi &= ~BIT(2);

	return 8;
}

/* RES 2,C */
int
ext_op_0x91(gb_cpu *cpu)
{
	cpu->regs[REG_BC].lo &= ~BIT(2);

	return 8;
}

/* RES 2,D */
int
ext_op_0x92(gb_cpu *cpu)
{
	cpu->regs[REG_DE].hi &= ~BIT(2);

	return 8;
}

/* RES 2,E */
int
ext_op_0x93(gb_cpu *cpu)
{
	cpu->regs[REG_DE].lo &= ~BIT(2);

	return 8;
}

/* RES 2,H */
int
ext_op_0x94(gb_cpu *cpu)
{
	cpu->regs[REG_HL].hi &= ~BIT(2);

	return 8;
}

/* RES 2,L */
int
ext_op_0x95(gb_cpu *cpu)
{
	cpu->regs[REG_HL].lo &= ~BIT(2);

	return 8;
}

/* RES 2,(HL) */
int
ext_op_0x96(gb_cpu *cpu)
{
	cpu->memory[cpu->regs[REG_HL].reg] &= ~BIT(2);

	return 16;
}

/* RES 2,A */
int
ext_op_0x97(gb_cpu *cpu)
{
	cpu->regs[REG_AF].hi &= ~BIT(2);

	return 8;
}

/* RES 3,B */
int
ext_op_0x98(gb_cpu *cpu)
{
	cpu->regs[REG_BC].hi &= ~BIT(3);

	return 8;
}

/* RES 3,C */
int
ext_op_0x99(gb_cpu *cpu)
{
	cpu->regs[REG_BC].lo &= ~BIT(3);

	return 8;
}

/* RES 3,D */
int
ext_op_0x9A(gb_cpu *cpu)
{
	cpu->regs[REG_DE].hi &= ~BIT(3);

	return 8;
}

/* RES 3,E */
int
ext_op_0x9B(gb_cpu *cpu)
{
	cpu->regs[REG_DE].lo &= ~BIT(3);

	return 8;
}

/* RES 3,H */
int
ext_op_0x9C(gb_cpu *cpu)
{
	cpu->regs[REG_HL].hi &= ~BIT(3);

	return 8;
}

/* RES 3,L */
int
ext_op_0x9D(gb_cpu *cpu)
{
	cpu->regs[REG_HL].lo &= ~BIT(3);

	return 8;
}

/* RES 3,(HL) */
int
ext_op_0x9E(gb_cpu *cpu)
{
	cpu->memory[cpu->regs[REG_HL].reg] &= ~BIT(3);

	return 16;
}

/* RES 3,A */
int
ext_op_0x9F(gb_cpu *cpu)
{
	cpu->regs[REG_AF].hi &= ~BIT(3);

	return 8;
}

/* RES 4,B */
int
ext_op_0xA0(gb_cpu *cpu)
{
	cpu->regs[REG_BC].hi &= ~BIT(4);

	return 8;
}

/* RES 4,C */
int
ext_op_0xA1(gb_cpu *cpu)
{
	cpu->regs[REG_BC].lo &= ~BIT(4);

	return 8;
}

/* RES 4,D */
int
ext_op_0xA2(gb_cpu *cpu)
{
	cpu->regs[REG_DE].hi &= ~BIT(4);

	return 8;
}

/* RES 4,E */
int
ext_op_0xA3(gb_cpu *cpu)
{
	cpu->regs[REG_DE].lo &= ~BIT(4);

	return 8;
}

/* RES 4,H */
int
ext_op_0xA4(gb_cpu *cpu)
{
	cpu->regs[REG_HL].hi &= ~BIT(4);

	return 8;
}

/* RES 4,L */
int
ext_op_0xA5(gb_cpu *cpu)
{
	cpu->regs[REG_HL].lo &= ~BIT(4);

	return 8;
}

/* RES 4,(HL) */
int
ext_op_0xA6(gb_cpu *cpu)
{
	cpu->memory[cpu->regs[REG_HL].reg] &= ~BIT(4);

	return 16;
}

/* RES 4,A */
int
ext_op_0xA7(gb_cpu *cpu)
{
	cpu->regs[REG_AF].hi &= ~BIT(4);

	return 8;
}

/* RES 5,B */
int
ext_op_0xA8(gb_cpu *cpu)
{
	cpu->regs[REG_BC].hi &= ~BIT(5);

	return 8;
}

/* RES 5,C */
int
ext_op_0xA9(gb_cpu *cpu)
{
	cpu->regs[REG_BC].lo &= ~BIT(5);

	return 8;
}

/* RES 5,D */
int
ext_op_0xAA(gb_cpu *cpu)
{
	cpu->regs[REG_DE].hi &= ~BIT(5);

	return 8;
}

/* RES 5,E */
int
ext_op_0xAB(gb_cpu *cpu)
{
	cpu->regs[REG_DE].lo &= ~BIT(5);

	return 8;
}

/* RES 5,H */
int
ext_op_0xAC(gb_cpu *cpu)
{
	cpu->regs[REG_HL].hi &= ~BIT(5);

	return 8;
}

/* RES 5,L */
int
ext_op_0xAD(gb_cpu *cpu)
{
	cpu->regs[REG_HL].lo &= ~BIT(5);

	return 8;
}

/* RES 5,(HL) */
int
ext_op_0xAE(gb_cpu *cpu)
{
	cpu->memory[cpu->regs[REG_HL].reg] &= ~BIT(5);

	return 16;
}

/* RES 5,A */
int
ext_op_0xAF(gb_cpu *cpu)
{
	cpu->regs[REG_AF].hi &= ~BIT(5);

	return 8;
}

/* RES 6,B */
int
ext_op_0xB0(gb_cpu *cpu)
{
	cpu->regs[REG_BC].hi &= ~BIT(6);

	return 8;
}

/* RES 6,C */
int
ext_op_0xB1(gb_cpu *cpu)
{
	cpu->regs[REG_BC].lo &= ~BIT(6);

	return 8;
}

/* RES 6,D */
int
ext_op_0xB2(gb_cpu *cpu)
{
	cpu->regs[REG_DE].hi &= ~BIT(6);

	return 8;
}

/* RES 6,E */
int
ext_op_0xB3(gb_cpu *cpu)
{
	cpu->regs[REG_DE].lo &= ~BIT(6);

	return 8;
}

/* RES 6,H */
int
ext_op_0xB4(gb_cpu *cpu)
{
	cpu->regs[REG_HL].hi &= ~BIT(6);

	return 8;
}

/* RES 6,L */
int
ext_op_0xB5(gb_cpu *cpu)
{
	cpu->regs[REG_HL].lo &= ~BIT(6);

	return 8;
}

/* RES 6,(HL) */
int
ext_op_0xB6(gb_cpu *cpu)
{
	cpu->memory[cpu->regs[REG_HL].reg] &= ~BIT(6);

	return 16;
}

/* RES 6,A */
int
ext_op_0xB7(gb_cpu *cpu)
{
	cpu->regs[REG_AF].hi &= ~BIT(6);

	return 8;
}

/* RES 7,B */
int
ext_op_0xB8(gb_cpu *cpu)
{
	cpu->regs[REG_BC].hi &= ~BIT(7);

	return 8;
}

/* RES 7,C */
int
ext_op_0xB9(gb_cpu *cpu)
{
	cpu->regs[REG_BC].lo &= ~BIT(7);

	return 8;
}

/* RES 7,D */
int
ext_op_0xBA(gb_cpu *cpu)
{
	cpu->regs[REG_DE].hi &= ~BIT(7);

	return 8;
}

/* RES 7,E */
int
ext_op_0xBB(gb_cpu *cpu)
{
	cpu->regs[REG_DE].lo &= ~BIT(7);

	return 8;
}

/* RES 7,H */
int
ext_op_0xBC(gb_cpu *cpu)
{
	cpu->regs[REG_HL].hi &= ~BIT(7);

	return 8;
}

/* RES 7,L */
int
ext_op_0xBD(gb_cpu *cpu)
{
	cpu->regs[REG_HL].lo &= ~BIT(7);

	return 8;
}

/* RES 7,(HL) */
int
ext_op_0xBE(gb_cpu *cpu)
{
	cpu->memory[cpu->regs[REG_HL].reg] &= ~BIT(7);

	return 16;
}

/* RES 7,A */
int
ext_op_0xBF(gb_cpu *cpu)
{
	cpu->regs[REG_AF].hi &= ~BIT(7);

	return 8;
}

/* SET 0,B */
int
ext_op_0xC0(gb_cpu *cpu)
{
	cpu->regs[REG_BC].hi |= BIT(0);

	return 8;
}

/* SET 0,C */
int
ext_op_0xC1(gb_cpu *cpu)
{
	cpu->regs[REG_BC].lo |= BIT(0);

	return 8;
}

/* SET 0,D */
int
ext_op_0xC2(gb_cpu *cpu)
{
	cpu->regs[REG_DE].hi |= BIT(0);

	return 8;
}

/* SET 0,E */
int
ext_op_0xC3(gb_cpu *cpu)
{
	cpu->regs[REG_DE].lo |= BIT(0);

	return 8;
}

/* SET 0,H */
int
ext_op_0xC4(gb_cpu *cpu)
{
	cpu->regs[REG_HL].hi |= BIT(0);

	return 8;
}

/* SET 0,L */
int
ext_op_0xC5(gb_cpu *cpu)
{
	cpu->regs[REG_HL].lo |= BIT(0);

	return 8;
}

/* SET 0,(HL) */
int
ext_op_0xC6(gb_cpu *cpu)
{
	cpu->memory[cpu->regs[REG_HL].reg] |= BIT(0);

	return 16;
}

/* SET 0,A */
int
ext_op_0xC7(gb_cpu *cpu)
{
	cpu->regs[REG_AF].hi |= BIT(0);

	return 8;
}

/* SET 1,B */
int
ext_op_0xC8(gb_cpu *cpu)
{
	cpu->regs[REG_BC].hi |= BIT(1);

	return 8;
}

/* SET 1,C */
int
ext_op_0xC9(gb_cpu *cpu)
{
	cpu->regs[REG_BC].lo |= BIT(1);

	return 8;
}

/* SET 1,D */
int
ext_op_0xCA(gb_cpu *cpu)
{
	cpu->regs[REG_DE].hi |= BIT(1);

	return 8;
}

/* SET 1,E */
int
ext_op_0xCB(gb_cpu *cpu)
{
	cpu->regs[REG_DE].lo |= BIT(1);

	return 8;
}

/* SET 1,H */
int
ext_op_0xCC(gb_cpu *cpu)
{
	cpu->regs[REG_HL].hi |= BIT(1);

	return 8;
}

/* SET 1,L */
int
ext_op_0xCD(gb_cpu *cpu)
{
	cpu->regs[REG_HL].lo |= BIT(1);

	return 8;
}

/* SET 1,(HL) */
int
ext_op_0xCE(gb_cpu *cpu)
{
	cpu->memory[cpu->regs[REG_HL].reg] |= BIT(1);

	return 16;
}

/* SET 1,A */
int
ext_op_0xCF(gb_cpu *cpu)
{
	cpu->regs[REG_AF].hi |= BIT(1);

	return 8;
}

/* SET 2,B */
int
ext_op_0xD0(gb_cpu *cpu)
{
	cpu->regs[REG_BC].hi |= BIT(2);

	return 8;
}

/* SET 2,C */
int
ext_op_0xD1(gb_cpu *cpu)
{
	cpu->regs[REG_BC].lo |= BIT(2);

	return 8;
}

/* SET 2,D */
int
ext_op_0xD2(gb_cpu *cpu)
{
	cpu->regs[REG_DE].hi |= BIT(2);

	return 8;
}

/* SET 2,E */
int
ext_op_0xD3(gb_cpu *cpu)
{
	cpu->regs[REG_DE].lo |= BIT(2);

	return 8;
}

/* SET 2,H */
int
ext_op_0xD4(gb_cpu *cpu)
{
	cpu->regs[REG_HL].hi |= BIT(2);

	return 8;
}

/* SET 2,L */
int
ext_op_0xD5(gb_cpu *cpu)
{
	cpu->regs[REG_HL].lo |= BIT(2);

	return 8;
}

/* SET 2,(HL) */
int
ext_op_0xD6(gb_cpu *cpu)
{
	cpu->memory[cpu->regs[REG_HL].reg] |= BIT(2);

	return 16;
}

/* SET 2,A */
int
ext_op_0xD7(gb_cpu *cpu)
{
	cpu->regs[REG_AF].hi |= BIT(2);

	return 8;
}

/* SET 3,B */
int
ext_op_0xD8(gb_cpu *cpu)
{
	cpu->regs[REG_BC].hi |= BIT(3);

	return 8;
}

/* SET 3,C */
int
ext_op_0xD9(gb_cpu *cpu)
{
	cpu->regs[REG_BC].lo |= BIT(3);

	return 8;
}

/* SET 3,D */
int
ext_op_0xDA(gb_cpu *cpu)
{
	cpu->regs[REG_DE].hi |= BIT(3);

	return 8;
}

/* SET 3,E */
int
ext_op_0xDB(gb_cpu *cpu)
{
	cpu->regs[REG_DE].lo |= BIT(3);

	return 8;
}

/* SET 3,H */
int
ext_op_0xDC(gb_cpu *cpu)
{
	cpu->regs[REG_HL].hi |= BIT(3);

	return 8;
}

/* SET 3,L */
int
ext_op_0xDD(gb_cpu *cpu)
{
	cpu->regs[REG_HL].lo |= BIT(3);

	return 8;
}

/* SET 3,(HL) */
int
ext_op_0xDE(gb_cpu *cpu)
{
	cpu->memory[cpu->regs[REG_HL].reg] |= BIT(3);

	return 16;
}

/* SET 3,A */
int
ext_op_0xDF(gb_cpu *cpu)
{
	cpu->regs[REG_AF].hi |= BIT(3);

	return 8;
}

/* SET 4,B */
int
ext_op_0xE0(gb_cpu *cpu)
{
	cpu->regs[REG_BC].hi |= BIT(4);

	return 8;
}

/* SET 4,C */
int
ext_op_0xE1(gb_cpu *cpu)
{
	cpu->regs[REG_BC].lo |= BIT(4);

	return 8;
}

/* SET 4,D */
int
ext_op_0xE2(gb_cpu *cpu)
{
	cpu->regs[REG_DE].hi |= BIT(4);

	return 8;
}

/* SET 4,E */
int
ext_op_0xE3(gb_cpu *cpu)
{
	cpu->regs[REG_DE].lo |= BIT(4);

	return 8;
}

/* SET 4,H */
int
ext_op_0xE4(gb_cpu *cpu)
{
	cpu->regs[REG_HL].hi |= BIT(4);

	return 8;
}

/* SET 4,L */
int
ext_op_0xE5(gb_cpu *cpu)
{
	cpu->regs[REG_HL].lo |= BIT(4);

	return 8;
}

/* SET 4,(HL) */
int
ext_op_0xE6(gb_cpu *cpu)
{
	cpu->memory[cpu->regs[REG_HL].reg] |= BIT(4);

	return 16;
}

/* SET 4,A */
int
ext_op_0xE7(gb_cpu *cpu)
{
	cpu->regs[REG_AF].hi |= BIT(4);

	return 8;
}

/* SET 5,B */
int
ext_op_0xE8(gb_cpu *cpu)
{
	cpu->regs[REG_BC].hi |= BIT(5);

	return 8;
}

/* SET 5,C */
int
ext_op_0xE9(gb_cpu *cpu)
{
	cpu->regs[REG_BC].lo |= BIT(5);

	return 8;
}

/* SET 5,D */
int
ext_op_0xEA(gb_cpu *cpu)
{
	cpu->regs[REG_DE].hi |= BIT(5);

	return 8;
}

/* SET 5,E */
int
ext_op_0xEB(gb_cpu *cpu)
{
	cpu->regs[REG_DE].lo |= BIT(5);

	return 8;
}

/* SET 5,H */
int
ext_op_0xEC(gb_cpu *cpu)
{
	cpu->regs[REG_HL].hi |= BIT(5);

	return 8;
}

/* SET 5,L */
int
ext_op_0xED(gb_cpu *cpu)
{
	cpu->regs[REG_HL].lo |= BIT(5);

	return 8;
}

/* SET 5,(HL) */
int
ext_op_0xEE(gb_cpu *cpu)
{
	cpu->memory[cpu->regs[REG_HL].reg] |= BIT(5);

	return 16;
}

/* SET 5,A */
int
ext_op_0xEF(gb_cpu *cpu)
{
	cpu->regs[REG_AF].hi |= BIT(5);

	return 8;
}

/* SET 6,B */
int
ext_op_0xF0(gb_cpu *cpu)
{
	cpu->regs[REG_BC].hi |= BIT(6);

	return 8;
}

/* SET 6,C */
int
ext_op_0xF1(gb_cpu *cpu)
{
	cpu->regs[REG_BC].lo |= BIT(6);

	return 8;
}

/* SET 6,D */
int
ext_op_0xF2(gb_cpu *cpu)
{
	cpu->regs[REG_DE].hi |= BIT(6);

	return 8;
}

/* SET 6,E */
int
ext_op_0xF3(gb_cpu *cpu)
{
	cpu->regs[REG_DE].lo |= BIT(6);

	return 8;
}

/* SET 6,H */
int
ext_op_0xF4(gb_cpu *cpu)
{
	cpu->regs[REG_HL].hi |= BIT(6);

	return 8;
}

/* SET 6,L */
int
ext_op_0xF5(gb_cpu *cpu)
{
	cpu->regs[REG_HL].lo |= BIT(6);

	return 8;
}

/* SET 6,(HL) */
int
ext_op_0xF6(gb_cpu *cpu)
{
	cpu->memory[cpu->regs[REG_HL].reg] |= BIT(6);

	return 16;
}

/* SET 6,A */
int
ext_op_0xF7(gb_cpu *cpu)
{
	cpu->regs[REG_AF].hi |= BIT(6);

	return 8;
}

/* SET 7,B */
int
ext_op_0xF8(gb_cpu *cpu)
{
	cpu->regs[REG_BC].hi |= BIT(7);

	return 8;
}

/* SET 7,C */
int
ext_op_0xF9(gb_cpu *cpu)
{
	cpu->regs[REG_BC].lo |= BIT(7);

	return 8;
}

/* SET 7,D */
int
ext_op_0xFA(gb_cpu *cpu)
{
	cpu->regs[REG_DE].hi |= BIT(7);

	return 8;
}

/* SET 7,E */
int
ext_op_0xFB(gb_cpu *cpu)
{
	cpu->regs[REG_DE].lo |= BIT(7);

	return 8;
}

/* SET 7,H */
int
ext_op_0xFC(gb_cpu *cpu)
{
	cpu->regs[REG_HL].hi |= BIT(7);

	return 8;
}

/* SET 7,L */
int
ext_op_0xFD(gb_cpu *cpu)
{
	cpu->regs[REG_HL].lo |= BIT(7);

	return 8;
}

/* SET 7,(HL) */
int
ext_op_0xFE(gb_cpu *cpu)
{
	cpu->memory[cpu->regs[REG_HL].reg] |= BIT(7);

	return 16;
}

/* SET 7,A */
int
ext_op_0xFF(gb_cpu *cpu)
{
	cpu->regs[REG_AF].hi |= BIT(7);

	return 8;
}

