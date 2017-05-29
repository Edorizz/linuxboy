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
#include "interrupts.h"
/* C library */
#include <stdio.h>
#include <stdlib.h>
/* Linuxboy */
#include "utils.h"
#include "opcodes.h"

void
request_interrupt(gb_cpu *cpu, BYTE interrupt)
{
	write_byte(cpu, IF, read_byte(cpu, IF) | BIT(interrupt));
	cpu->status &= ~(BIT(HALT) | BIT(STOP));
}

void
handle_interrupts(gb_cpu *cpu)
{
	BYTE interrupts;

	if (cpu->ime && (cpu->memory[IE] & cpu->memory[IF])) {
		interrupts = cpu->memory[IE] & cpu->memory[IF];
		cpu->ime = 0;

		for (int i = 0; i != INTERRUPT_MAX; ++i) {
			if (interrupts & BIT(i)) {
				cpu->memory[IF] ^= BIT(i);

				call(cpu, 0x40 + i * 8);
				return;
			}
		}
	}
}

