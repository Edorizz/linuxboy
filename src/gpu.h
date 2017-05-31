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

#ifndef LINUXBOY_GPU_H
#define LINUXBOY_GPU_H

#include "cpu.h"

/* PIXEL COLORS */
#define WHITE			0
#define LIGHT_GRAY		1
#define DARK_GRAY		2
#define BLACK			3
#define MAX_COLORS		4

void draw_scanline(gb_cpu *cpu);
void flip_screen(gb_cpu *cpu);
void clear_screen(gb_cpu *cpu, int color);
BYTE *get_tile(gb_cpu *cpu, BYTE id);
int  get_color(BYTE *pixel);
void draw_tile_row(gb_cpu *cpu, const BYTE *data, int offset, int screen_y, int screen_x, BYTE palette);
void draw_sprite_row(gb_cpu *cpu, const BYTE *data, int offset, int screen_y, int screen_x, BYTE sprite_attr);
void draw_tiles(gb_cpu *cpu);

#endif /* LINUXBOY_GPU_H */

