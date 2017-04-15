#ifndef LINUXBOY_GAMEBOY_H
#define LINUXBOY_GAMEBOY_H

#include <linuxboy/cpu.h>
#include <linuxboy/cartridge.h>
#include <linuxboy/window.h>

typedef struct {
	/* MAIN COMPONENTS */
	gb_cpu cpu;
	gb_cartridge cart;
	gl_window win;
	/* EMULATION */
	BYTE emu_flags;
	WORD breakpoint;
	int curr_cycles;
	int cycles;
} gameboy;

void power_gb(gameboy *gb);
void shutdown_gb(gameboy *gb);
void update_gb(gameboy *gb);

#endif /* LINUXBOY_GAMEBOY_H */

