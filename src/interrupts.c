/* Header file */
#include <linuxboy/interrupts.h>
/* C library */
#include <stdio.h>
#include <stdlib.h>
/* Linuxboy */
#include <linuxboy/utils.h>
#include <linuxboy/opcodes.h>

void
request_interrupt(gb_cpu *cpu, BYTE interrupt)
{
	write_byte(cpu, IE, read_byte(cpu, IE) | BIT(interrupt));
}

void
handle_interrupts(gb_cpu *cpu)
{
	BYTE interrupts;

	if (cpu->ime && cpu->memory[IE] && cpu->memory[IF]) {
		interrupts = cpu->memory[IE] & cpu->memory[IF];

		for (int i = 0; i != INTERRUPT_MAX; ++i) {
			if (interrupts & BIT(i)) {
				cpu->memory[IF] ^= BIT(i);
				call(cpu, 0x40 + i * 8);
				break;
			}
		}
	}
}

