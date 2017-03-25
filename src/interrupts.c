#include "../include/linuxboy.h"
#include <stdio.h>

/* Not really necessary, may remove */
const WORD routines[INTERRUPT_MAX] = { 0x40, 0x48, 0x50, 0x58, 0x60 };

void
request_interrupt(gb_cpu *cpu, BYTE interrupt)
{
	write_byte(cpu, IE, read_byte(cpu, IE) & BIT(interrupt));
}

void
handle_interrupts(gb_cpu *cpu)
{
	BYTE interrupts;

	if (cpu->ime && cpu->memory[IE] && cpu->memory[IF]) {
		interrupts = cpu->memory[IE] & cpu->memory[IF];

		for (int i = 0; i != INTERRUPT_MAX; ++i) {
			if (interrupts & BIT(i)) {
				printf("interrupt! bit %d\n", i);
				cpu->memory[IF] ^= BIT(i);
				call(cpu, routines[i]);
				/* call(cpu, 0x40 + i * 8); */
				break;
			}
		}
	}
}

