#include "../include/linuxboy.h"

const WORD routines[INTERRUPT_MAX] = { 0x40, 0x48, 0x50, 0x58, 0x60 };

void
handle_interrupts(gb_cpu *cpu)
{
	BYTE interrupts;

	if (cpu->ime && cpu->memory[IE] && cpu->memory[IF]) {
		interrupts = cpu->memory[IE] & cpu->memory[IF];

		for (int i = 0; i != INTERRUPT_MAX; ++i) {
			if (interrupts & BIT(i)) {
				cpu->memory[IF] ^= BIT(i);
				call(cpu, routines[i]);
				/* call(cpu, 0x40 + i * 8); */
				break;
			}
		}
	}
}

