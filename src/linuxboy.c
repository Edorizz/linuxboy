/*
 * Linuxboy - Nintendo Game Boy Emulator
 * Copyright (C) 2017  Edgar Mendoza

 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

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
	printf("usage: %s rom [-h] [-b addr] [-s scale [-x width -y height]] [-w addr] [-l state] [-n]\n", argv[0]);
}

