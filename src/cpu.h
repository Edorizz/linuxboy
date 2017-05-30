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

/* SPECIAL REGISTERS */
#define LCD_CONTROL		0xFF40
#define LCD_STATUS		0xFF41
#define SCROLL_Y		0xFF42
#define SCROLL_X		0xFF43
#define CURR_SCANLINE		0xFF44
#define TARGET_SCANLINE		0xFF45
#define WINDOW_Y		0xFF4A
#define WINDOW_X		0xFF4B

/* OTHER */
#define CLOCK_RATE	4194304

enum regs	{ REG_AF, REG_BC, REG_DE, REG_HL, REG_MAX };
enum cpu_flags	{ FLAG_C = 4, FLAG_H, FLAG_N, FLAG_Z };
enum cpu_status	{ HALT, STOP };

/* RGB PIXEL */
typedef struct {
	BYTE r, g, b;
} color;

/* 16-BIT REGISTER */
typedef union {
	WORD reg;
	struct {
		BYTE lo;
		BYTE hi;
	};
} reg;

/* CPU */
typedef struct {
	/* GAME MEMORY */
	gb_cartridge *cart;
	BYTE memory[0x10000]; /* 64KB */
	reg regs[REG_MAX];
	WORD pc;
	WORD stack;
	/* SPECIAL REGISTERS */
	BYTE ime; /* Interrupt master switch */
	BYTE joypad;
	BYTE status;
	/* TIMING */
	int divider_cnt;
	int timer_cnt;
	int scanline_cnt;
	/* GRAPHICS */
	BYTE scr_buf[SCR_H + 1][SCR_W][3]; /* Extra screen row is used as a temporary buffer when flipping the screen */
} gb_cpu;

/* CPU FUNCTIONS */
int  power_cpu(gb_cpu *cpu, const BYTE *bootstrap);
int  exec_op(gb_cpu *cpu);
void dma_transfer(gb_cpu *cpu, BYTE val);
void update_graphics(gb_cpu *cpu, int ops);
void load_rom_bank(gb_cpu *cpu);
void load_ram_bank(gb_cpu *cpu);

/* DEBUGGING */
void disassemble(const gb_cpu *cpu);
void map_dump(const gb_cpu *cpu);
void cpu_status(const gb_cpu *cpu);

/* OPCODE HELPERS */
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

