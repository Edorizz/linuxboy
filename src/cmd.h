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

#define CMD_OPTIONS	7

typedef struct {
	char cmd_short;
	char *cmd_long;
	int (*fnc)(gameboy*, const char*);
} cmd_option;

int cmd_check(gameboy *gb, const char **argv);

/* COMMAND LINE OPTIONS */
int cmd_breakpoint(gameboy *gb, const char *arg);
int cmd_win_width(gameboy *gb, const char *arg);
int cmd_win_height(gameboy *gb, const char *arg);
int cmd_win_scale(gameboy *gb, const char *arg);
int cmd_add_watch(gameboy *gb, const char *arg);
int cmd_load_state(gameboy *gb, const char *arg);
int cmd_help(gameboy *gb, const char *arg);

#endif /* LINUXBOY_CMD_H */

