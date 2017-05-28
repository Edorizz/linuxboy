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

