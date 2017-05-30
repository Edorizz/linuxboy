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

/* Header file */
#include "cmd.h"

const cmd_option cmd_options[CMD_OPTIONS] = { { 'b', "break", cmd_breakpoint },
					      { 'x', "width", cmd_win_width },
					      { 'y', "height", cmd_win_height },
					      { 's', "scale", cmd_win_scale },
					      { 'w', "watch", cmd_add_watch },
					      { 'l', "state", cmd_load_state },
					      { 'h', "help", cmd_help },
					      { 'n', "bootstrap", cmd_bootstrap } };

int
cmd_check(gameboy *gb, const char **argv)
{
	if (argv[0][1] == '-') {
		for (int i = 0; i != CMD_OPTIONS; ++i) {
			if (strcmp(&argv[0][2], cmd_options[i].cmd_long) == 0) {
				return cmd_options[i].fnc(gb, argv[1]);
			}
		}
	} else if (argv[0][2] == '\0') {
		for (int i = 0; i != CMD_OPTIONS; ++i) {
			if (argv[0][1] == cmd_options[i].cmd_short) {
				return cmd_options[i].fnc(gb, argv[1]);
			}
		}
	}

	return -1;
}

int
cmd_breakpoint(gameboy *gb, const char *arg)
{
	if (arg) {
		gb->emu_flags |= BIT(BREAKPOINT);
		gb->breakpoint = strtol(arg, NULL, 16);
		
		return 1;
	}

	return 0;
}

int
cmd_win_width(gameboy *gb, const char *arg)
{
	if (arg) {
		gb->win.width = strtol(arg, NULL, 10);
	
		return 1;
	}

	return 0;
}

int
cmd_win_height(gameboy *gb, const char *arg)
{
	if (arg) {
		gb->win.height = strtol(arg, NULL, 10);

		return 1;
	}

	return 0;
}

int
cmd_win_scale(gameboy *gb, const char *arg)
{
	if (arg) {
		gb->win.width *= strtol(arg, NULL, 10);
		gb->win.height *= strtol(arg, NULL, 10);

		return 1;
	}

	return 0;
}

int
cmd_add_watch(gameboy *gb, const char *arg)
{
	if (arg) {
		if (gb->watch_size == MAX_WATCH) {
			fprintf(stderr, "Watch list size exceeded\n");
		} else {
			gb->watch_list[gb->watch_size++] = strtol(arg, NULL, 16);
		}
		
		return 1;
	}

	return 0;
}

int
cmd_load_state(gameboy *gb, const char *arg)
{
	if (arg) {
		gb->emu_flags |= BIT(LOAD_STATE);
		gb->state_path = arg;
		
		return 1;
	}

	return 0;
}

int
cmd_help(gameboy *gb, const char *arg)
{
	printf("\t-b: set breakpoint at 'addr' (hex)\n"
	       "\t-h: print this message and quit\n"
	       "\t-s: set screen width to 160 * 'scale' and screen height to 144 * 'scale'\n"
	       "\t-x: set screen width to 'width'\n"
	       "\t-y: set screen height to 'height'\n"
	       "\t-w: add 'addr' to the watch list\n"
	       "\t-l: load CPU state from 'state'\n"
	       "\t-n: load bootstrap\n");

	return 0;
}

int
cmd_bootstrap(gameboy *gb, const char *arg)
{
	gb->emu_flags |= BIT(BOOTSTRAP);

	return 0;
}

