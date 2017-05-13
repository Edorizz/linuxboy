/* Header file */
#include <linuxboy/gameboy.h>
/* C Library */
#include <stdio.h>
#include <time.h>
/* Linuxboy */
#include <linuxboy/timers.h>
#include <linuxboy/interrupts.h>

void
power_gb(gameboy *gb)
{
	if (load_cartridge(&gb->cart) == -1)
		return;

	if (gb->win.sdl_win == NULL &&
	    create_window(&gb->win, &gb->cpu.joypad, &gb->emu_flags) == -1)
		return;

	/* Link components */
	gb->cpu.cart = &gb->cart;
	gb->win.scr_buf = &gb->cpu.scr_buf[0][0][0];

	/* Power components */
	power_cpu(&gb->cpu);
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
	/* Toggle debug mode if hit target breakpoint */
	if (gb->emu_flags & BIT(BREAKPOINT) && gb->cpu.pc == gb->breakpoint)
		gb->emu_flags |= BIT(DEBUG);

	/* Print CPU status if on debug mode */
	if (gb->emu_flags & BIT(DEBUG)) {
		cpu_status(&gb->cpu);
		if (gb->watch_size) {
			printf("\n--+ WATCH LIST +--\n");
			for (int i = 0; i != gb->watch_size; ++i)
				printf("%04x: %02x\n",
				       gb->watch_list[i], gb->cpu.memory[gb->watch_list[i]]);
		}

		handle_input(&gb->win);
	}

	if (gb->emu_flags & BIT(MAP_DUMP)) {
		map_dump(gb);
		gb->emu_flags ^= BIT(MAP_DUMP);
	}

	if (!(gb->cpu.status & BIT(STOP))) {
		if (!(gb->cpu.status & BIT(HALT))) {
			/* Execute opcode */
			gb->curr_cycles += gb->cycles = exec_op(&gb->cpu);
		}
		
		/* Update internals */
		update_timers(&gb->cpu, gb->cycles);
		update_graphics(&gb->cpu, gb->cycles);
	}

	if (gb->cpu.memory[gb->cpu.pc - 1] != 0xFB)
		handle_interrupts(&gb->cpu);

	/* Draw only if neccessary */
	if (gb->curr_cycles >= CLOCK_RATE / 60) {
		handle_input(&gb->win);
		gb->curr_cycles = 0;

		flip_screen(&gb->cpu);
		render(&gb->win);
		clear_screen(&gb->cpu, WHITE);

		swap_window(&gb->win);
	}
}

void
map_dump(gameboy *gb)
{
	FILE *fp = fopen("map.log", "wb");

	for (int i = 0; i != 32; ++i) {
		for (int j = 0; j != 32; ++j)
			fprintf(fp, "%02x ", gb->cpu.memory[0x9800 + (i * 32) + j]);
		fprintf(fp, "\n");
	}

	fclose(fp);
}

