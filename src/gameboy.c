/* Header file */
#include "gameboy.h"
/* C Library */
#include <stdio.h>
#include <time.h>
/* Linuxboy */
#include "gpu.h"
#include "timers.h"
#include "interrupts.h"

void
power_gb(gameboy *gb)
{
	if (load_cartridge(&gb->cart) == -1 ||
	    (gb->win.sdl_win == NULL && create_window(&gb->win, &gb->cpu.joypad, &gb->emu_flags) == -1)) {
		return;
	}

	/* Link components */
	gb->cpu.cart = &gb->cart;
	gb->win.scr_buf = &gb->cpu.scr_buf[0][0][0];

	if (gb->emu_flags & BIT(LOAD_STATE)) {
		load_state_gb(gb, gb->state_path);
	} else {
		/* Power components */
		power_cpu(&gb->cpu);
	}
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
		map_dump(gb);
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

void
map_dump(gameboy *gb)
{
	FILE *fp = fopen("map.log", "wb");

	for (int i = 0; i != 32; ++i) {
		for (int j = 0; j != 32; ++j) {
			fprintf(fp, "%02x ", gb->cpu.memory[0x9800 + (i * 32) + j]);
		}

		fprintf(fp, "\n");
	}

	fclose(fp);
}

