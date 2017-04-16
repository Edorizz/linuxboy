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

void
map_dump(gameboy *gb)
{
	FILE *fp = fopen("map.log", "wb");
	int id;

	for (int i = 0; i != 32; ++i) {
		for (int j = 0; j != 32; ++j) {
			if ((id = gb->cpu.memory[0x9800 + (i * 32) + j]) == 0x2F)
				fprintf(fp, "   ");
			else
				fprintf(fp, "%02x ", id);
		}
		fprintf(fp, "\n");
	}

	fclose(fp);
}

int
main(int argc, char **argv)
{
	gameboy gb;

	reset_gb(&gb);
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
			case 'x':
				gb.win.width = strtol(argv[i + 1], NULL, 10);
				break;
			case 'y':
				gb.win.height = strtol(argv[i + 1], NULL, 10);
				break;
			case 's':
				gb.win.width *= strtol(argv[i + 1], NULL, 10);
				gb.win.height *= strtol(argv[i + 1], NULL, 10);
				break;
			case 'w':
				if (gb.watch_size == MAX_WATCH)
					fprintf(stderr, "Watch list size exceeded\n");
				else
					gb.watch_list[gb.watch_size++] = strtol(argv[i + 1], NULL, 16);
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

	while (!(gb.emu_flags & BIT(QUIT))) {
		if (gb.emu_flags & BIT(MAP_DUMP)) {
			map_dump(&gb);
			return 0;
		}

		update_gb(&gb);
	}

	shutdown_gb(&gb);

	return 0;
}

void
usage(char **argv)
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

