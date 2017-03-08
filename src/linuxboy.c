#include <stdio.h>
#include <stdlib.h>

#include "../include/linuxboy.h"

int
main(int argc, char **argv)
{
	gb_cpu cpu;

	if (load_rom(&cpu, "tetris.gb") != 0)
		return 1;

	disassemble(&cpu);

	return 0;
}
