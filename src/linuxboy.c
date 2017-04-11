/* C library */
#include <stdio.h>
#include <stdlib.h>
/* SDL2 */
#include <GL/glew.h>
#include <SDL2/SDL.h>
/* Linuxboy */
#include <linuxboy/linuxboy.h>

/* Function prototypes */
void usage(char **argv);

int
main(int argc, char **argv)
{
	gameboy gb = { 0 };

	for (int i = 1; i != argc; ++i) {
		if (argv[i][0] == '-') {
			if (i == argc - 1) {
				usage(argv);
				return 0;
			}

			/* TODO: Make some sort of cmd_option struct and group them
			 * together to make it easier to find the desired one.
			 */
			switch (argv[i][1]) {
			case 'b':
				gb.emu_flags |= BIT(BREAKPOINT);
				gb.breakpoint = strtol(argv[i + 1], NULL, 16);
				break;
			case 'h':
				usage(argv);
				return 0;
			case '-':
				/* Long command line options go here */
				break;
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
usage(char **argv)
{
	printf("usage: %s rom [-h] [-b addr]\n"
	       "\t-b addr: set breakpoint at specified address (hex)\n"
	       "\t-h: print this message and quit\n",
		argv[0]);
}

