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
#include "interrupts.h"

/* Monochrome shades, change this to change main colors */
const color colors[MAX_COLORS] = { { 224, 248, 208 },	/* WHITE */
				   { 136, 192, 112 },	/* LIGHT_GRAY */
				   {  52, 104,  86 },	/* DARK_GRAY */
				   {   8,  24,  32 } };	/* BLACK */

void
update_graphics(gb_gpu *gpu, int cycles)
{
	BYTE stat;

	if (gpu->io[IO(LCDC)] & BIT(7)) {
		gpu->scanline_cnt += cycles;

		switch ((stat = gpu->io[IO(STAT)]) & 0x3) {
		case 2:
			if (gpu->scanline_cnt >= 80) {
				gpu->scanline_cnt -= 80;

				gpu->io[IO(STAT)] = (stat & ~0x3) | 0x3;
			}
			
			break;
		case 3:
			if (gpu->scanline_cnt >= 172) {
				gpu->scanline_cnt -= 172;

				draw_scanline(gpu);
				if (stat & BIT(3)) {
					/* request_interrupt(gpu, LCD_STAT); */
					gpu->io[IO(IF)] |= BIT(LCD_STAT);
				}

				gpu->io[IO(STAT)] = (stat & ~0x3) | 0x0;
			}

			break;
		case 0:
			if (gpu->scanline_cnt >= 204) {
				gpu->scanline_cnt -= 204;

				if (++gpu->io[IO(LY)] >= 144) {
					gpu->io[IO(STAT)] = (stat & ~0x3) | 0x1;
					/* request_interrupt(gpu, LCD_STAT); */
					gpu->io[IO(IF)] |= BIT(VBLANK);

					if (stat & BIT(4)) {
						/* request_interrupt(gpu, LCD_STAT); */
						gpu->io[IO(IF)] |= BIT(LCD_STAT);
					}
				} else {
					gpu->io[IO(STAT)] = (stat & ~0x3) | 0x2;
					
					if (stat & BIT(5)) {
						/* request_interrupt(gpu, LCD_STAT); */
						gpu->io[IO(IF)] |= BIT(LCD_STAT);
					}
				}
			}

			break;
		case 1:
			if (gpu->scanline_cnt >= 456) {
				gpu->scanline_cnt -= 456;

				if (++gpu->io[IO(LY)] >= 153) {
					gpu->io[IO(STAT)] = (stat & ~0x3) | 0x2;
					gpu->io[IO(LY)] = 0;

					if (stat & BIT(5)) {
						/* request_interrupt(gpu, LCD_STAT); */
						gpu->io[IO(IF)] |= BIT(LCD_STAT);
					}
				}
			}

			break;
		}

		if (gpu->io[IO(LY)] == gpu->io[IO(LYC)]) {
			gpu->io[IO(STAT)] |= BIT(2);
			if (stat & BIT(6)) {
				/* request_interrupt(gpu, LCD_STAT); */
				gpu->io[IO(IF)] |= BIT(LCD_STAT);
			}
		} else {
			gpu->io[IO(STAT)] &= ~BIT(2);
		}
	} else {
		gpu->io[IO(STAT)] &= ~0x3/* |= BIT(2) */;
	}
}

/* TODO: Make this clearer */
void
draw_scanline(gb_gpu *gpu)
{
	BYTE lcdc, scanline, scroll_y, scroll_x, window_y, window_x, id, *data, *attr;

	/* LCD Controller */
	lcdc	 = gpu->io[IO(LCDC)];

	/* Scanline */
	scanline = gpu->io[IO(LY)];

	/* Get scroll registers */
	scroll_y = gpu->io[IO(SCY)];
	scroll_x = gpu->io[IO(SCX)];
	window_y = gpu->io[IO(WY)];
	window_x = gpu->io[IO(WX)];

	if (scanline < 0 || scanline >= SCR_H) {
		printf("ly: %d\n", scanline);
		return;
	}

	/* Draw tiles */
	if (lcdc & BIT(0)) {
		for (int i = 0; i * 8 - scroll_x < SCR_W; ++i) {
			id = gpu->vram[(0x1800 + (lcdc & BIT(3) ? 0x400 : 0)) +
				       ((scroll_y + scanline) / 8 % 32 * 32) +
				       ((scroll_x / 8 + i) % 32)];
			data = get_tile(gpu, id);

			draw_tile_row(gpu, data + (scroll_y + scanline) % 8 * 2,
				      i == 0 ? scroll_x % 8 : 0, scanline, MAX(i * 8 - scroll_x % 8, 0), gpu->io[IO(BGP)]);
		}
	}

	/* Draw window */
	if (lcdc & BIT(5)) {
		if (window_y <= scanline) {
			for (int i = 0; i * 8 + window_x - 7 < SCR_W; ++i) {
				id = gpu->vram[(0x1800 + (lcdc & BIT(6) ? 0x400 : 0)) +
					       ((scanline - window_y) / 8 * 32) + i];
				data = get_tile(gpu, id);

				draw_tile_row(gpu, data + (scanline - window_y) % 8 * 2,
					      window_x - 7 + i * 8 < 0 ? 7 - window_x : 0, scanline, MAX(window_x - 7 + i * 8, 0), gpu->io[IO(BGP)]);
			}
		}
	}

	/* Draw sprites */
	if (lcdc & BIT(1)) {
		if (lcdc & BIT(2)) {
			for (int i = 0; i != 40; ++i) {
				attr = &gpu->oam[i * 4];
				
				if (attr[0] - 16 <= scanline && scanline < attr[0]) {
					if (attr[3] & BIT(6)) {
						data = &gpu->vram[attr[2] * 16 + ((15 - (scanline - (attr[0] - 16))) * 2)];
					} else {
						data = &gpu->vram[attr[2] * 16 + ((scanline - (attr[0] - 16)) * 2)];
					}
					
					if (attr[1] - 8 < 0) {
						draw_sprite_row(gpu, data, 8 - attr[1], scanline, 0, attr[3]);
					} else {
						draw_sprite_row(gpu, data, 0, scanline, attr[1] - 8, attr[3]);
					}
				}
			}
		} else {
			for (int i = 0; i != 40; ++i) {
				attr = &gpu->oam[i * 4];
				
				if (attr[0] - 16 <= scanline && scanline < attr[0] - 8) {
					if (attr[3] & BIT(6)) {
						data = &gpu->vram[attr[2] * 16 + ((7 - (scanline - (attr[0] - 16))) * 2)];
					} else {
						data = &gpu->vram[attr[2] * 16 + ((scanline - (attr[0] - 16)) * 2)];
					}
					
					if (attr[1] - 8 < 0) {
						draw_sprite_row(gpu, data, 8 - attr[1], scanline, 0, attr[3]);
					} else {
						draw_sprite_row(gpu, data, 0, scanline, attr[1] - 8, attr[3]);
					}
				}
			}
		}
	}
}

void
flip_screen(gb_gpu *gpu)
{
	BYTE *top_row, *bot_row;

	for (int i = 0; i != SCR_H / 2; ++i) {
		top_row = &gpu->scr_buf[SCR_H - i - 1][0][0];
		bot_row = &gpu->scr_buf[i][0][0];

		memcpy(&gpu->scr_buf[SCR_H][0][0], bot_row, SCR_W * 3);
		memcpy(bot_row, top_row, SCR_W * 3);
		memcpy(top_row, &gpu->scr_buf[SCR_H][0][0], SCR_W * 3);
	}
}

void
clear_screen(gb_gpu *gpu, int color)
{
	for (int i = 0; i != SCR_H; ++i) {
		for (int j = 0; j != SCR_W; ++j) {
			memcpy(gpu->scr_buf[i][j], &colors[color], 3);
		}
	}
}

BYTE *
get_tile(gb_gpu *gpu, BYTE id)
{
	if (gpu->io[IO(LCDC)] & BIT(4)) {
		return &gpu->vram[id * 0x10];
	} else {
		return &gpu->vram[0x0800 + (128 + *(SIGNED_BYTE*)&id) * 0x10];
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

int
paletted(int color, BYTE palette)
{
	for (int i = 0; i != 4; ++i) {
		if (((palette >> (i * 2)) & 0x3) == color) {
			return i;
		}
	}

	return -1;
}

void
draw_tile_row(gb_gpu *gpu, const BYTE *data, int offset, int screen_y, int screen_x, BYTE palette)
{
	BYTE b1, b2, color;

	b1 = *data;
	b2 = *(data + 1);

	for (int i = offset; i < 8 && screen_x + i - offset < SCR_W; ++i) {
		color = ((b2 >> (7 - i)) << 1 & 0x2) | ((b1 >> (7 - i)) & 0x1);
		memcpy(gpu->scr_buf[SCR_H][screen_x + i - offset], &colors[color], 3);

		color = (palette >> ((((b2 >> (7 - i)) << 1 & 0x2) | ((b1 >> (7 - i)) & 0x1)) * 2)) & 0x3;
		memcpy(gpu->scr_buf[screen_y][screen_x + i - offset], &colors[color], 3);
	}
}

void
draw_sprite_row(gb_gpu *gpu, const BYTE *data, int offset, int screen_y, int screen_x, BYTE attr)
{
	BYTE b1, b2, palette, color;

	b1 = *data;
	b2 = *(data + 1);
	palette = gpu->io[IO(attr & BIT(4) ? OBP1 : OBP0)];

	if (attr & BIT(5)) {
		for (int i = offset; i != 8 && screen_x + i - offset < SCR_W; ++i) {
			color = ((b2 >> i) << 1 & 0x2) | ((b1 >> i) & 0x1);

			if (color != WHITE &&
			    (!(attr & BIT(7)) || get_color(gpu->scr_buf[SCR_H][screen_x + i - offset]) == WHITE)) {
				color = (palette >> (color * 2)) & 0x3;	
				memcpy(gpu->scr_buf[screen_y][screen_x + i - offset], &colors[color], 3);
			}
		}
	} else {
		for (int i = offset; i != 8 && screen_x + i - offset < SCR_W; ++i) {
			color = ((b2 >> (7 - i)) << 1 & 0x2) | ((b1 >> (7 - i)) & 0x1);
			
			if (color != WHITE &&
			    (!(attr & BIT(7)) || get_color(gpu->scr_buf[SCR_H][screen_x + i - offset]) == WHITE)) {
				color = (palette >> (color * 2)) & 0x3;
				memcpy(gpu->scr_buf[screen_y][screen_x + i - offset], &colors[color], 3);
			}
		}
	}
}

