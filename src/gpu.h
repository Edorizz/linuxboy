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

#include "utils.h"

/* -==+ Screen dimensions +==- */
#define SCR_W		160
#define SCR_H		144

/* -==+ LCD I/O +==- */
#define LCDC		0xFF40	/* LCD Control */
#define STAT		0xFF41	/* LCD Status */
#define SCY		0xFF42	/* Scroll Y */
#define SCX		0xFF43	/* Scroll X */
#define LY		0xFF44	/* Current Scanline */
#define LYC		0xFF45	/* Compare Scanline */
#define DMA		0xFF46	/* DMA Transfer */
#define BGP		0xFF47	/* Backgroud Palette */
#define OBP0		0xFF48	/* Sprite Palette 0 */
#define OBP1		0xFF49	/* Sprite Palette 1 */
#define WY		0xFF4A	/* Window Scroll Y */
#define WX		0xFF4B	/* Window Scroll X */

/* -==+ Convert I/O address to relative address +==- */
#define IO(d)		((d) - 0xFF00)

/* -==+ Pixel Colors */
#define WHITE		0
#define LIGHT_GRAY	1
#define DARK_GRAY	2
#define BLACK		3
#define MAX_COLORS	4

/* -==+ RGB Pixel +==- */
typedef struct _color {
	BYTE r, g, b;
} color;

/*
 * -==+ Game Boy GPU component -==+
 * Proccesses data in VRAM/OAM pointers and outputs result to a
 * screen buffer which is used by the correspoinding Window component
 * and renders it to the screen.
 */
typedef struct _gb_gpu {
	/*
	 * [Video memory pointers]
	 * They all point to the same array so a single pointer
	 * might have been enough but this grants better readability
	 */
	BYTE *vram;	/* 0x8000 - 0x9FFF  (2KiB) */
	BYTE *oam;	/* 0xFE00 - 0xFE9F  (160B)*/
	BYTE *io;	/* 0xFF00 - 0xFF7F  (128B)*/

	/*
	 * [Screen buffer]
	 * Represents the screen pixel buffer. It is used by the window
	 * to draw to the screen.
	 */
	BYTE scr_buf[SCR_H + 1][SCR_W][3];
} gb_gpu;

void draw_scanline(gb_gpu *gpu);
void flip_screen(gb_gpu *gpu);
void clear_screen(gb_gpu *gpu, int color);
BYTE *get_tile(gb_gpu *gpu, BYTE id);
int  get_color(BYTE *pixel);
void draw_tile_row(gb_gpu *gpu, const BYTE *data, int offset, int screen_y, int screen_x, BYTE palette);
void draw_sprite_row(gb_gpu *gpu, const BYTE *data, int offset, int screen_y, int screen_x, BYTE sprite_attr);
void draw_tiles(gb_gpu *gpu);

#endif /* LINUXBOY_GPU_H */

