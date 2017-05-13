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
