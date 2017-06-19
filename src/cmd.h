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

#ifndef LINUXBOY_CMD_H
#define LINUXBOY_CMD_H

#include "gameboy.h"

#define CMD_OPTIONS	8

/*
 * -==+ Command Line Argument +==-
 * Links a specific character or string to a function.
 * (makes handling with command line arguments easier)
 */
typedef struct _cmd_opt {
	/*
	 * [Argument]
	 * Specifies which character or string the user has to
	 * type to trigger the argument function ('fnc').
	 */
	char cmd_short;
	char *cmd_long;

	/* [Link Function] */
	int (*fnc)(gb*, const char*);
} cmd_opt;

int cmd_check(gb *gb, const char **argv);

/* -==+ Command Line Options +==- */
int cmd_breakpoint(gb *gb, const char *arg);
int cmd_win_width(gb *gb, const char *arg);
int cmd_win_height(gb *gb, const char *arg);
int cmd_win_scale(gb *gb, const char *arg);
int cmd_add_watch(gb *gb, const char *arg);
int cmd_load_state(gb *gb, const char *arg);
int cmd_help(gb *gb, const char *arg);
int cmd_bootstrap(gb *gb, const char *arg);

#endif /* LINUXBOY_CMD_H */

