#include <stdio.h>
#include <stdlib.h>

#include "../include/linuxboy.h"

#define DEBUG

int
main(int argc, char **argv)
{
	gb_cpu cpu;
	int op_size;

	if (load_rom(&cpu, "tetris.gb") != 0)
		return 1;

	power(&cpu);
	for (;;) {
#ifdef DEBUG
			cpu_status(&cpu);

			if ((op_size = exec_op(&cpu)) == -1)
				return 1;

			getchar();
#else
			if ((op_size = exec_op(&cpu)) == -1)
				return 1;
#endif
	}

	return 0;
}
