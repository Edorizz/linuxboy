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
#include "gameboy.h"
/* C Library */
#include <stdio.h>
#include <time.h>
/* Linuxboy */
#include "gpu.h"
#include "timers.h"
#include "interrupts.h"

/* Game Boy bootstrap */
const BYTE gb_bootstrap [0x100] = {
	0x31, 0xFE, 0xFF, 0xAF, 0x21, 0xFF, 0x9F, 0x32, 0xCB, 0x7C, 0x20, 0xFB, 0x21, 0x26, 0xFF, 0x0E, 
	0x11, 0x3E, 0x80, 0x32, 0xE2, 0x0C, 0x3E, 0xF3, 0xE2, 0x32, 0x3E, 0x77, 0x77, 0x3E, 0xFC, 0xE0, 
	0x47, 0x11, 0x04, 0x01, 0x21, 0x10, 0x80, 0x1A, 0xCD, 0x95, 0x00, 0xCD, 0x96, 0x00, 0x13, 0x7B, 
	0xFE, 0x34, 0x20, 0xF3, 0x11, 0xD8, 0x00, 0x06, 0x08, 0x1A, 0x13, 0x22, 0x23, 0x05, 0x20, 0xF9, 
	0x3E, 0x19, 0xEA, 0x10, 0x99, 0x21, 0x2F, 0x99, 0x0E, 0x0C, 0x3D, 0x28, 0x08, 0x32, 0x0D, 0x20, 
	0xF9, 0x2E, 0x0F, 0x18, 0xF3, 0x67, 0x3E, 0x64, 0x57, 0xE0, 0x42, 0x3E, 0x91, 0xE0, 0x40, 0x04, 
	0x1E, 0x02, 0x0E, 0x0C, 0xF0, 0x44, 0xFE, 0x90, 0x20, 0xFA, 0x0D, 0x20, 0xF7, 0x1D, 0x20, 0xF2, 
	0x0E, 0x13, 0x24, 0x7C, 0x1E, 0x83, 0xFE, 0x62, 0x28, 0x06, 0x1E, 0xC1, 0xFE, 0x64, 0x20, 0x06, 
	0x7B, 0xE2, 0x0C, 0x3E, 0x87, 0xE2, 0xF0, 0x42, 0x90, 0xE0, 0x42, 0x15, 0x20, 0xD2, 0x05, 0x20, 
	0x4F, 0x16, 0x20, 0x18, 0xCB, 0x4F, 0x06, 0x04, 0xC5, 0xCB, 0x11, 0x17, 0xC1, 0xCB, 0x11, 0x17, 
	0x05, 0x20, 0xF5, 0x22, 0x23, 0x22, 0x23, 0xC9, 0xCE, 0xED, 0x66, 0x66, 0xCC, 0x0D, 0x00, 0x0B, 
	0x03, 0x73, 0x00, 0x83, 0x00, 0x0C, 0x00, 0x0D, 0x00, 0x08, 0x11, 0x1F, 0x88, 0x89, 0x00, 0x0E, 
	0xDC, 0xCC, 0x6E, 0xE6, 0xDD, 0xDD, 0xD9, 0x99, 0xBB, 0xBB, 0x67, 0x63, 0x6E, 0x0E, 0xEC, 0xCC, 
	0xDD, 0xDC, 0x99, 0x9F, 0xBB, 0xB9, 0x33, 0x3E, 0x3C, 0x42, 0xB9, 0xA5, 0xB9, 0xA5, 0x42, 0x3C, 
	0x21, 0x04, 0x01, 0x11, 0xA8, 0x00, 0x1A, 0x13, 0xBE, 0x20, 0xFE, 0x23, 0x7D, 0xFE, 0x34, 0x20, 
	0xF5, 0x06, 0x19, 0x78, 0x86, 0x23, 0x05, 0x20, 0xFB, 0x86, 0x20, 0xFE, 0x3E, 0x01, 0xE0, 0x50
};

int
power_gb(gameboy *gb)
{
	if (load_cartridge(&gb->cart) == -1 ||
	    (gb->win.sdl_win == NULL && create_window(&gb->win, &gb->cpu.joypad, &gb->emu_flags) == -1)) {
		return -1;
	}

	/* Link components */
	gb->cpu.cart = &gb->cart;
	gb->win.scr_buf = &gb->cpu.scr_buf[0][0][0];

	if (gb->emu_flags & BIT(LOAD_STATE)) {
		load_state_gb(gb, gb->state_path);
		gb->emu_flags ^= BIT(LOAD_STATE);
	} else {
		/* Power components */
		power_cpu(&gb->cpu, gb->emu_flags & BIT(BOOTSTRAP) ? gb_bootstrap : NULL);
		gb->emu_flags &= ~BIT(BOOTSTRAP);
	}

	return 0;
}

void
reset_gb(gameboy *gb)
{
	memset(gb, 0, sizeof(*gb));

	gb->win.width = SCR_W;
	gb->win.height = SCR_H;
}

void
shutdown_gb(gameboy *gb)
{
	delete_window(&gb->win);
}

void
update_gb(gameboy *gb)
{
	/* Save current gameboy state to state.gbs if needed */
	if (gb->emu_flags & BIT(SAVE_STATE)) {
		save_state_gb(gb, "state.gbs");
		gb->emu_flags ^= BIT(SAVE_STATE);
	}

	/* Toggle debug mode if hit target breakpoint */
	if (gb->emu_flags & BIT(BREAKPOINT) && gb->cpu.pc == gb->breakpoint) {
		gb->emu_flags |= BIT(DEBUG);
	}

	/* Print CPU status if on debug mode */
	if (gb->emu_flags & BIT(DEBUG)) {
		cpu_status(&gb->cpu);
		if (gb->watch_size) {
			printf("\n--+ WATCH LIST +--\n");
			for (int i = 0; i != gb->watch_size; ++i) {
				printf("%04x: %02x\n", gb->watch_list[i], gb->cpu.memory[gb->watch_list[i]]);
			}
		}

		handle_input(&gb->win);
	}

	/* DEBUGGING: Print current tile map to a map.log */
	if (gb->emu_flags & BIT(MAP_DUMP)) {
		map_dump(&gb->cpu);
		gb->emu_flags ^= BIT(MAP_DUMP);
	}

	if (!(gb->cpu.status & BIT(STOP))) {
		if (!(gb->cpu.status & BIT(HALT))) {
			/* Execute opcode */
			gb->cycles = exec_op(&gb->cpu);
		}

		gb->curr_cycles += gb->cycles;
		
		/* Update internals */
		update_timers(&gb->cpu, gb->cycles);
		update_graphics(&gb->cpu, gb->cycles);
	}

	if (gb->cpu.memory[gb->cpu.pc - 1] != 0xFB) {
		handle_interrupts(&gb->cpu);
	}

	/* Draw only if neccessary */
	if (gb->curr_cycles >= CLOCK_RATE / 60 && gb->cpu.memory[CURR_SCANLINE] >= 144) {
		handle_input(&gb->win);
		gb->curr_cycles -= CLOCK_RATE / 60;

		flip_screen(&gb->cpu);
		render(&gb->win);
		clear_screen(&gb->cpu, WHITE);

		swap_window(&gb->win);
	}
}

void
save_state_gb(gameboy *gb, const char *path)
{
	FILE *fp = fopen(path, "wb");

	fwrite(((BYTE*)(&gb->cpu)) + sizeof(gb_cartridge*),
	       1, sizeof(gb_cpu) - sizeof(gb_cartridge*),
	       fp);

	fclose(fp);
}

void
load_state_gb(gameboy *gb, const char *path)
{
	FILE *fp = fopen(path, "rb");

	fread(((BYTE*)(&gb->cpu)) + sizeof(gb_cartridge*),
	      1, sizeof(gb_cpu) - sizeof(gb_cartridge*),
	      fp);

	fclose(fp);
}

