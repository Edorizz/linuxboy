/* Header file */
#include <linuxboy/gameboy.h>
/* C Library */
#include <stdio.h>
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
	gb->gpu.vram = &gb->cpu.memory[0x8000];

	/* Power components */
	power_cpu(&gb->cpu);
	power_gpu(&gb->gpu, SCREEN_WIDTH, SCREEN_HEIGHT);
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

	/*
	if (gb->cpu.pc == 0x282a) {
		FILE *fp = fopen("gpu.log", "wb");
		fwrite(gb->gpu.vram, 16, 1, fp);
		fclose(fp);
	}
	*/

	/* Print CPU status if on debug mode */
	if (gb->emu_flags & BIT(DEBUG))
		cpu_status(&gb->cpu);

	/* Proccess input */
	handle_input(&gb->win);

	/* Execute opcode */
	gb->curr_cycles += gb->cycles = exec_op(&gb->cpu);

	/* Update internals */
	update_timers(&gb->cpu, gb->cycles);
	update_graphics(&gb->cpu, gb->cycles);
	handle_interrupts(&gb->cpu);

	/* Draw only if neccessary */
	if (gb->curr_cycles >= CLOCK_RATE / 60) {
		gb->curr_cycles = 0;

		draw_tiles(&gb->gpu);
		flip_screen(&gb->gpu);
		render(&gb->gpu);

		swap_window(&gb->win);
	}
}

