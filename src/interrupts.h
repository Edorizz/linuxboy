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

#ifndef LINUXBOY_INTERRUPTS_H
#define LINUXBOY_INTERRUPTS_H

#include "cpu.h"

/* INTERRUPT REGISTERS */
#define IE			0xFFFF
#define IF			0xFF0F

enum interrupts	{ VBLANK, LCD_STAT, TIMER, SERIAL, JOYPAD, INTERRUPT_MAX };

void request_interrupt(gb_cpu *cpu, BYTE interrupt);
void handle_interrupts(gb_cpu *cpu);

#endif /* LINUXBOY_INTERRUPTS_H */
