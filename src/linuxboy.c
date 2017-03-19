#include <stdio.h>
#include <stdlib.h>

#include "../include/linuxboy.h"

int
main(int argc, char **argv)
{
	gb_cpu cpu;
	int debug;

	if (load_rom(&cpu, "tetris.gb") != 0)
		return 1;

	debug = 0;
	power(&cpu);
	for (;;) {
		if (cpu.pc == 0x2817) {
			cpu_status(&cpu);
			debug = 1;
		}

		if (exec_op(&cpu) == -1)
			getchar();

		if (debug)
			getchar();

		handle_interrupts(&cpu);
	}

	return 0;
}

