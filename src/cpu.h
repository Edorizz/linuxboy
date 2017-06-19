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

#ifndef LINUXBOY_CPU_H
#define LINUXBOY_CPU_H

#include "utils.h"
#include "cartridge.h"

#define CLOCK_RATE	4194304

enum regs	{ REG_AF, REG_BC, REG_DE, REG_HL, REG_MAX };
enum cpu_flags	{ FLAG_C = 4, FLAG_H, FLAG_N, FLAG_Z };
enum cpu_status	{ HALT, STOP, TIMER_RELOAD };

/* -==+ 16-BIT Register +==- */
typedef union _reg {
	WORD reg;
	struct {
		BYTE lo;
		BYTE hi;
	};
} reg;

/*
 * -==+ Game Boy CPU component +==-
 * Does all main operations, takes input from a Game Boy
 * cartridge pointer ('cart').

 * Other main components depend on the CPU's main memory
 * array ('memory') to operate. The GPU takes input from
 * VRAM, OAM and the I/O ports while the APU takes input
 * only from the I/O ports.
 */
typedef struct _gb_cpu {
	/* [Cartridge] */
	gb_cart *cart;
	void (*mbc)(struct _gb_cpu*, WORD, BYTE);
	/* [Game Memory] */
	BYTE memory[0x10000]; /* 64KB */
	reg regs[REG_MAX];
	WORD pc;
	WORD stack;
	/* [Special Registers] */
	BYTE ime; /* Interrupt master switch */
	BYTE joypad;
	BYTE status;
	/* [Timing] */
	int divider_cnt;
	int timer_cnt;
	int scanline_cnt;
	int tmp_cnt;
} gb_cpu;

/* -==+ CPU Functions +==- */
int  power_cpu(gb_cpu *cpu, const BYTE *bootstrap);
int  exec_op(gb_cpu *cpu);
void dma_transfer(gb_cpu *cpu, BYTE val);
void update_graphics(gb_cpu *cpu, int ops);
void load_rom_bank(gb_cpu *cpu);
void load_ram_bank(gb_cpu *cpu);

/* -==+ Memory Bank Controllers +==- */
void mbc0(gb_cpu *cpu, WORD addr, BYTE val);
void mbc1(gb_cpu *cpu, WORD addr, BYTE val);
void mbc2(gb_cpu *cpu, WORD addr, BYTE val);
void mbc3(gb_cpu *cpu, WORD addr, BYTE val);
void mbc5(gb_cpu *cpu, WORD addr, BYTE val);
void mbc6(gb_cpu *cpu, WORD addr, BYTE val);

/* -==+ Debugging +==- */
void disassemble(const gb_cpu *cpu);
void map_dump(const gb_cpu *cpu);
void cpu_status(const gb_cpu *cpu);

/* -==+ Opcode Helpers +==- */
BYTE inc_byte(BYTE *flag, BYTE b);
BYTE dec_byte(BYTE *flag, BYTE b);
void swap_byte(BYTE *flag, BYTE *b);
void rot_byte(BYTE *flag, BYTE *b, BYTE rot_flags);
void shift_byte(BYTE *flag, BYTE *b, BYTE shift_flags);
void add_byte(BYTE *flag, BYTE *b, int val);
void add_word(BYTE *flag, WORD *w, int val);
void sub_byte(BYTE *flag, BYTE *b, BYTE val);

void and_byte(BYTE *flag, BYTE *b, BYTE val);
void xor_byte(BYTE *flag, BYTE *b, BYTE val);
void or_byte(BYTE *flag, BYTE *b, BYTE val);
void cp_byte(BYTE *flag, BYTE b, BYTE val);
void test_bit(BYTE *flag, BYTE b, BYTE bit);

BYTE read_byte(gb_cpu *cpu, WORD addr);
WORD read_word(gb_cpu *cpu, WORD addr);
void write_byte(gb_cpu *cpu, WORD addr, BYTE val);
void write_word(gb_cpu *cpu, WORD addr, WORD val);

WORD pop(gb_cpu *cpu);
void push(gb_cpu *cpu, WORD val);

void call(gb_cpu *cpu, WORD addr);
void ret(gb_cpu *cpu);

#endif /* LINUXBOY_CPU_H */

