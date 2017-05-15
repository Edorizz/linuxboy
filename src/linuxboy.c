/* C library */
#include <stdio.h>
/* Linuxboy */
#include "linuxboy.h"

/* Function prototypes */
void usage(const char **argv);

int
main(int argc, const char **argv)
{
	gameboy gb;
	int arg_size;

	reset_gb(&gb);
	for (int i = 1; i != argc; ++i) {
		if (argv[i][0] == '-') {
			if ((arg_size = cmd_check(&gb, argv + i)) == -1) {
				usage(argv);
				return 0;
			}

			i += arg_size;
		} else {
			gb.cart.rom_path = argv[i];
		}
	}

	power_gb(&gb);
	if (gb.cart.rom_size == 0) {
		usage(argv);
		return 0;
	}

	while (!(gb.emu_flags & BIT(QUIT))) {
		update_gb(&gb);
	}

	shutdown_gb(&gb);

	return 0;
}

void
usage(const char **argv)
{
	printf("usage: %s rom [-h] [-b addr] [-s scale [-x width -y height]] [-w addr]\n", argv[0]);
}

