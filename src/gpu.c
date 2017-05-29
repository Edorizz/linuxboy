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
#include "gpu.h"
/* C library */
#include <string.h>
/* Linuxboy */
#include "cpu.h"

const color colors[MAX_COLORS] = { { 224, 248, 208 },	/* WHITE */
				   { 136, 192, 112 },	/* LIGHT_GRAY */
				   {  52, 104,  86 },	/* DARK_GRAY */
				   {   8,  24,  32 } };	/* BLACK */

/* TODO: Make this clearer */
void
draw_scanline(gb_cpu *cpu)
{
	BYTE lcd, scanline, scroll_x, scroll_y, window_x, window_y, id, *data, *attr;
	
	lcd = read_byte(cpu, LCD_CONTROL);
	scanline = read_byte(cpu, CURR_SCANLINE);

	scroll_x = read_byte(cpu, SCROLL_X);
	scroll_y = read_byte(cpu, SCROLL_Y);
	window_x = read_byte(cpu, WINDOW_X);
	window_y = read_byte(cpu, WINDOW_Y);

	/* Draw tiles */
	if (lcd & BIT(0)) {
		for (int i = 0; i * 8 - scroll_x < SCR_W; ++i) {
			id = cpu->memory[(0x9800 + (lcd & BIT(3) ? 0x400 : 0)) +
					 ((scroll_y + scanline) / 8 % 32 * 32) +
					 ((scroll_x / 8 + i) % 32)];
			data = get_tile(cpu, id);
			
			draw_tile_row(cpu, data + (scroll_y + scanline) % 8 * 2,
				      i == 0 ? scroll_x % 8 : 0, scanline, MAX(i * 8 - scroll_x % 8, 0), cpu->memory[0xFF47]);
		}
	}

	/* Draw window */
	if (lcd & BIT(5)) {
		if (window_y <= scanline) {
			for (int i = 0; i * 8 + window_x - 7 < SCR_W; ++i) {
				id = cpu->memory[(0x9800 + (lcd & BIT(6) ? 0x400 : 0)) +
						 ((scanline - window_y) / 8 * 32) + i];
				data = get_tile(cpu, id);

				draw_tile_row(cpu, data + (scanline - window_y) % 8 * 2,
					      window_x - 7 + i * 8 < 0 ? 7 - window_x : 0, scanline, MAX(window_x - 7 + i * 8, 0), cpu->memory[0xFF47]);
			}
		}
	}

	/* Draw sprites */
	if (lcd & BIT(1)) {
		if (lcd & BIT(2)) {
			for (int i = 0; i != 40; ++i) {
				attr = &cpu->memory[0xFE00 + i * 4];
				
				if (attr[0] - 16 <= scanline && scanline < attr[0]) {
					if (attr[3] & BIT(6)) {
						data = &cpu->memory[0x8000 + attr[2] * 16 + ((15 - (scanline - (attr[0] - 16))) * 2)];
					} else {
						data = &cpu->memory[0x8000 + attr[2] * 16 + ((scanline - (attr[0] - 16)) * 2)];
					}
					
					if (attr[1] - 8 < 0) {
						draw_sprite_row(cpu, data, 8 - attr[1], scanline, 0, attr[3]);
					} else {
						draw_sprite_row(cpu, data, 0, scanline, attr[1] - 8, attr[3]);
					}
				}
			}
		} else {
			for (int i = 0; i != 40; ++i) {
				attr = &cpu->memory[0xFE00 + i * 4];
				
				if (attr[0] - 16 <= scanline && scanline < attr[0] - 8) {
					if (attr[3] & BIT(6)) {
						data = &cpu->memory[0x8000 + attr[2] * 16 + ((7 - (scanline - (attr[0] - 16))) * 2)];
					} else {
						data = &cpu->memory[0x8000 + attr[2] * 16 + ((scanline - (attr[0] - 16)) * 2)];
					}
					
					if (attr[1] - 8 < 0) {
						draw_sprite_row(cpu, data, 8 - attr[1], scanline, 0, attr[3]);
					} else {
						draw_sprite_row(cpu, data, 0, scanline, attr[1] - 8, attr[3]);
					}
				}
			}
		}
	}
}

void
flip_screen(gb_cpu *cpu)
{
	BYTE *top_row, *bot_row;

	for (int i = 0; i != SCR_H / 2; ++i) {
		top_row = &cpu->scr_buf[SCR_H - i - 1][0][0];
		bot_row = &cpu->scr_buf[i][0][0];

		memcpy(&cpu->scr_buf[SCR_H][0][0], bot_row, SCR_W * 3);
		memcpy(bot_row, top_row, SCR_W * 3);
		memcpy(top_row, &cpu->scr_buf[SCR_H][0][0], SCR_W * 3);
	}
}

void
clear_screen(gb_cpu *cpu, int color)
{
	for (int i = 0; i != SCR_H; ++i) {
		for (int j = 0; j != SCR_W; ++j) {
			memcpy(cpu->scr_buf[i][j], &colors[color], 3);
		}
	}
}

BYTE *
get_tile(gb_cpu *cpu, BYTE id)
{
	if (cpu->memory[LCD_CONTROL] & BIT(4)) {
		return &cpu->memory[0x8000 + id * 16];
	} else {
		return &cpu->memory[0x8800 + (128 + *(SIGNED_BYTE*)&id) * 16];
	}
}

int
get_color(BYTE *pixel)
{
	for (int i = 0; i != MAX_COLORS; ++i) {
		if (memcmp(&colors[i], pixel, 3) == 0) {
			return i;
		}
	}

	return -1;
}

void
draw_tile_row(gb_cpu *cpu, const BYTE *data, int offset, int screen_y, int screen_x, BYTE palette)
{
	BYTE b1, b2, color;

	b1 = *data;
	b2 = *(data + 1);

	for (int i = offset; i != 8 && screen_x + i - offset < SCR_W; ++i) {
		color = (palette >> ((((b2 >> (7 - i)) << 1 & 0x2) | ((b1 >> (7 - i)) & 0x1)) * 2)) & 0x3;

		memcpy(cpu->scr_buf[screen_y][screen_x + i - offset], &colors[color], 3);
	}
}

void
draw_sprite_row(gb_cpu *cpu, const BYTE *data, int offset, int screen_y, int screen_x, BYTE attr)
{
	BYTE b1, b2, palette, color;

	b1 = *data;
	b2 = *(data + 1);
	palette = cpu->memory[attr & BIT(4) ? 0xFF49 : 0xFF48];

	if (attr & BIT(5)) {
		for (int i = offset; i != 8 && screen_x + i - offset < SCR_W; ++i) {
			color = ((b2 >> i) << 1 & 0x2) | ((b1 >> i) & 0x1);

			if (color != WHITE && (!(attr & BIT(7)) || get_color(cpu->scr_buf[screen_y][screen_x + i - offset]) == WHITE)) {
				color = (palette >> (color * 2)) & 0x3;	
				memcpy(cpu->scr_buf[screen_y][screen_x + i - offset], &colors[color], 3);
			}
		}
	} else {
		for (int i = offset; i != 8 && screen_x + i - offset < SCR_W; ++i) {
			color = ((b2 >> (7 - i)) << 1 & 0x2) | ((b1 >> (7 - i)) & 0x1);
			
			if (color != WHITE && (!(attr & BIT(7)) || get_color(cpu->scr_buf[screen_y][screen_x + i - offset]) == WHITE)) {
				color = (palette >> (color * 2)) & 0x3;
				memcpy(cpu->scr_buf[screen_y][screen_x + i - offset], &colors[color], 3);
			}
		}
	}
}

