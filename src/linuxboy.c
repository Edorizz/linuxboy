/* C library */
#include <stdio.h>
#include <stdlib.h>
/* SDL2 */
#include <GL/glew.h>
#include <SDL2/SDL.h>

#include "../include/linuxboy.h"
#include "../include/glsl_shader.h"

/* MAIN LOOP BIT FLAGS */
#define FLAG_QUIT	0
#define FLAG_DEBUG	1

int
init_window(SDL_Window **window)
{
	SDL_GLContext gl_context;

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		fprintf(stderr, "Video initialization error: %s\n", SDL_GetError());
		return 1;
	}

	*window = SDL_CreateWindow("Linuxboy Gameboy emulator",
				  SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 160, 144,
				  SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

	if (*window == NULL) {
		fprintf(stderr, "Window creation error: %s\n", SDL_GetError());
		return 1;
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

	gl_context = SDL_GL_CreateContext(*window);
	if (gl_context == NULL) {
		fprintf(stderr, "GL Context creation error: %s\n", SDL_GetError());
		return 1;
	}

	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "GLEW initialization error\n");
		return 1;
	}

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	return 0;
}

void
wait_input(BYTE *flags)
{
	SDL_Event event;

	for (;;) {
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:
				*flags |= BIT(FLAG_QUIT);
				return;
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym) {
				case SDLK_q:
					*flags |= BIT(FLAG_QUIT);
					break;
				case SDLK_d:
					*flags ^= BIT(FLAG_DEBUG);
					break;
				}
				return;
			}
		}
	}
}

void
handle_input(BYTE *flags)
{
	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			*flags |= BIT(FLAG_QUIT);
			break;
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym) {
			case SDLK_q:
				*flags |= BIT(FLAG_QUIT);
				break;
			case SDLK_d:
				*flags ^= BIT(FLAG_DEBUG);
				break;
			}
			break;
		}
	}
}

int
main(int argc, char **argv)
{
	SDL_Window *window = NULL;
	gb_cpu cpu;
	int max_pc, curr_cycles, cycles;
	BYTE flags;

	if (load_rom(&cpu, "tetris.gb") != 0)
		return 1;

	init_window(&window);
	glClearColor(0.2f, 0.4f, 0.8f, 1.0f);

	max_pc = 1;
	flags = 0;
	power(&cpu);

	while (!(flags & BIT(FLAG_QUIT))) {
		if (flags & BIT(FLAG_DEBUG))
			wait_input(&flags);
		else
			handle_input(&flags);

		while (curr_cycles < CLOCK_RATE / 60) {
			curr_cycles += cycles = exec_op(&cpu);

			if (cpu.pc > max_pc)
				max_pc = cpu.pc;

			update_timers(&cpu, cycles);
			update_graphics(&cpu, cycles);
			handle_interrupts(&cpu);

			if (flags & BIT(FLAG_DEBUG)) {
				cpu_status(&cpu);
				break;
			}
		}

		if (curr_cycles >= CLOCK_RATE / 60) {
			curr_cycles = 0;

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			draw(&cpu.gpu);
			SDL_GL_SwapWindow(window);
		}
	}

	SDL_DestroyWindow(window);
	SDL_Quit();

	printf("max_pc: %x\n", max_pc);

	return 0;
}

