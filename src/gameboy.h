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

#ifndef LINUXBOY_GAMEBOY_H
#define LINUXBOY_GAMEBOY_H

#include "cpu.h"
#include "cartridge.h"
#include "window.h"

#define MAX_WATCH	16

typedef struct {
	/* MAIN COMPONENTS */
	gb_cpu cpu;
	gb_cartridge cart;
	gl_window win;
	/* EMULATION */
	BYTE emu_flags;
	WORD watch_list[MAX_WATCH];
	int watch_size;
	WORD breakpoint;
	const char *state_path;
	/* TIMING */
	int curr_cycles;
	int cycles;
} gameboy;

int  power_gb(gameboy *gb);
void reset_gb(gameboy *gb);
void shutdown_gb(gameboy *gb);
void update_gb(gameboy *gb);
void resize_gb(gameboy *gb, int width, int height);
void save_state_gb(gameboy *gb, const char *path);
void load_state_gb(gameboy *gb, const char *path);

#endif /* LINUXBOY_GAMEBOY_H */

