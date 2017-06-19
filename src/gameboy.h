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
#include "gpu.h"
#include "cartridge.h"
#include "window.h"

#define MAX_WATCH	16

/*
 * -==+ Game Boy +==-
 * Main Game Boy data structure.
 * Contains all Game Boy components in an easy to understand
 * (and to mantain) manner.
 */
typedef struct _gb {
	/*
	 * [Main Components]
	 * Main Game Boy components. They mostly work in a pipeline
	 * kind of manner, starting from the cartridge and ending
	 * with the window.

	 * cart -> cpu -> gpu -> win
	 */
	gb_cart cart;
	gb_cpu cpu;
	gb_gpu gpu;
	gl_win win;

	/*
	 * [Emulation]
	 * Variables which serve the purpose of adding functionality to
	 * the emulator. (breakpoints, change speed, watching variables,
	 * etc.)
	 */
	BYTE emu_flags;
	WORD watch_list[MAX_WATCH];
	int watch_size;
	WORD breakpoint;
	const char *state_path;

	/*
	 * [Timing]
	 * Controls the timing of the whole emulator.
	 */
	int curr_cycles;
	int cycles;
} gb;

int  power_gb(gb *gb);
void reset_gb(gb *gb);
void shutdown_gb(gb *gb);
void update_gb(gb *gb);
void resize_gb(gb *gb, int width, int height);
void save_state_gb(gb *gb, const char *path);
void load_state_gb(gb *gb, const char *path);

#endif /* LINUXBOY_GB_H */

