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

	reset_gb(&gb);
	for (int i = 1; i != argc; ++i) {
		if (argv[i][0] == '-') {
			if (!cmd_check(&gb, argv + i)) {
				usage(argv);
				return 0;
			}

			++i;
		} else {
			gb.cart.rom_path = argv[i];
		}
	}

	power_gb(&gb);
	if (gb.cart.rom_size == 0) {
		usage(argv);
		return 0;
	}

	while (!(gb.emu_flags & BIT(QUIT)))
		update_gb(&gb);

	shutdown_gb(&gb);

	return 0;
}

void
usage(const char **argv)
{
	printf("usage: %s rom [-h] [-b addr] [-s scale [-x width -y height]] [-w addr]\n"
	       "\t-b addr: set breakpoint at 'addr' (hex)\n"
	       "\t-h: print this message and quit\n"
	       "\t-s: set screen width to 160 * 'scale' and screen height to 144 * 'scale'\n"
	       "\t-x: set screen width to 'width'\n"
	       "\t-y: set screen height to 'height'\n"
	       "\t-w: add 'addr' to the watch list\n",
		argv[0]);
}

