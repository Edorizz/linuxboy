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
	/* TIMING */
	int curr_cycles;
	int cycles;
} gameboy;

void power_gb(gameboy *gb);
void reset_gb(gameboy *gb);
void shutdown_gb(gameboy *gb);
void update_gb(gameboy *gb);
void resize_gb(gameboy *gb, int width, int height);

void map_dump(gameboy *gb);

#endif /* LINUXBOY_GAMEBOY_H */
