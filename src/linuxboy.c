/* C library */
#include <stdio.h>
#include <stdlib.h>
/* SDL2 */
#include <GL/glew.h>
#include <SDL2/SDL.h>

#include "../include/linuxboy.h"
#include "../include/glsl_shader.h"

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

int
main(int argc, char **argv)
{
	SDL_Window *window = NULL;
	SDL_Event event;
	gb_cpu cpu;
	int quit, debug, max_pc, curr_cycles;

	if (load_rom(&cpu, "tetris.gb") != 0)
		return 1;

	init_window(&window);
	glClearColor(0.2f, 0.4f, 0.8f, 1.0f);

	debug = max_pc = 0;
	power(&cpu);

	while (!quit) {
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:
				quit = 1;
				break;
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym) {
				case SDLK_q:
					quit = 1;
					break;
				case SDLK_d:
					debug = !debug;
					break;
				}
			}
		}

		curr_cycles = 0;
		while (curr_cycles < CLOCK_RATE / 60)
			curr_cycles += exec_op(&cpu);

		if (cpu.pc > max_pc)
			max_pc = cpu.pc;

		if (cpu.pc == 0x297) {
			cpu_status(&cpu);
			getchar();
		}

		if (debug) {
			cpu_status(&cpu);
			getchar();
		}

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		draw(&cpu.gpu);
		SDL_GL_SwapWindow(window);

		handle_interrupts(&cpu);
	}

	SDL_DestroyWindow(window);
	SDL_Quit();

	printf("max_pc: %x\n", max_pc);

	return 0;

	/*
	for (;;) {
		if (cpu.pc == 0x2817) {
			cpu_status(&cpu);
			debug = 1;
		}
	}
	*/
}

