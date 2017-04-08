/* C library */
#include <stdio.h>
#include <stdlib.h>
/* SDL2 */
#include <GL/glew.h>
#include <SDL2/SDL.h>
/* Linuxboy */
#include <linuxboy/linuxboy.h>

/* Function prototypes */
void usage(char **argv);
int  init_window(SDL_Window **window);
void wait_input(gb_cpu *cpu);
void handle_input(gb_cpu *cpu);
void process_input(gb_cpu *cpu, SDL_Event *event);

int
main(int argc, char **argv)
{
	SDL_Window *window = NULL;
	gb_cpu cpu = { 0 };
	int curr_cycles, cycles, ops;

	for (int i = 1; i != argc; ++i) {
		if (argv[i][0] == '-') {
			if (i == argc - 1) {
				usage(argv);
				return 0;
			}

			/* TODO: Make some sort of cmd_option struct and group them
			 * together to make it easier to find the desired one.
			 */
			switch (argv[i][1]) {
			case 'b':
				cpu.flags |= BIT(BREAKPOINT);
				cpu.breakpoint = strtol(argv[i + 1], NULL, 16);
				break;
			case 'h':
				usage(argv);
				return 0;
			case '-':
				/* Long command line options go here */
				break;
			}

			++i;
		} else {
			cpu.rom_path = argv[i];
		}
	}

	if (cpu.rom_path == NULL || !load_rom(&cpu)) {
		usage(argv);
		return 0;
	}

	init_window(&window);
	power(&cpu);
	ops = 0;

	while (!(cpu.flags & BIT(QUIT))) {
		if (cpu.flags & BIT(BREAKPOINT) && cpu.pc == cpu.breakpoint)
			cpu.flags |= BIT(DEBUG);
		
		if (cpu.flags & BIT(DEBUG)) {
			cpu_status(&cpu);
			printf("%d/%d cycles\nop #%d\n", curr_cycles, CLOCK_RATE / 60, ops);
			wait_input(&cpu);
		} else {
			handle_input(&cpu);
		}
		
		curr_cycles += cycles = exec_op(&cpu);
		++ops;

		update_timers(&cpu, cycles);
		update_graphics(&cpu, cycles);
		handle_interrupts(&cpu);

		if (curr_cycles >= CLOCK_RATE / 60) {
			curr_cycles = 0;

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			draw(&cpu.gpu);
			SDL_GL_SwapWindow(window);
		}
	}

	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}

void
usage(char **argv)
{
	printf("usage: %s rom [-h] [-b addr]\n"
	       "\t-b addr: set breakpoint at specified address (hex)\n"
	       "\t-h: print this message and quit\n",
		argv[0]);
}

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
wait_input(gb_cpu *cpu)
{
	SDL_Event event;

	while (!(cpu->flags & BIT(QUIT)) && cpu->flags & BIT(DEBUG)) {
		while (SDL_PollEvent(&event)) {
			process_input(cpu, &event);
			
			switch (event.type) {
			case SDL_QUIT:
				cpu->flags |= BIT(QUIT);
				return;
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym) {
				case SDLK_q:
					cpu->flags |= BIT(QUIT);
					break;
				case SDLK_p:
					cpu->flags ^= BIT(DEBUG);
					break;
				case SDLK_n:
					return;
				}
				break;
			}
		}
	}
}

void
handle_input(gb_cpu *cpu)
{
	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		process_input(cpu, &event);
		
		switch (event.type) {
		case SDL_QUIT:
			cpu->flags |= BIT(QUIT);
			break;
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym) {
			case SDLK_q:
				cpu->flags |= BIT(QUIT);
				break;
			case SDLK_p:
				cpu->flags ^= BIT(DEBUG);
				break;
			}
			break;
		}
	}
}

void
process_input(gb_cpu *cpu, SDL_Event *event)
{
	switch (event->type) {
	case SDL_KEYDOWN:
		switch (event->key.keysym.sym) {
		case SDLK_a:
			key_event(cpu, PAD_LEFT, PRESS);
			break;
		case SDLK_d:
			key_event(cpu, PAD_RIGHT, PRESS);
			break;
		case SDLK_w:
			key_event(cpu, PAD_UP, PRESS);
			break;
		case SDLK_s:
			key_event(cpu, PAD_DOWN, PRESS);
			break;
		case SDLK_j:
			key_event(cpu, A, PRESS);
			break;
		case SDLK_k:
			key_event(cpu, B, PRESS);
			break;
		case SDLK_u:
			key_event(cpu, START, PRESS);
			break;
		case SDLK_i:
			key_event(cpu, SELECT, PRESS);
			break;
		}
		break;
	case SDL_KEYUP:
		switch (event->key.keysym.sym) {
		case SDLK_a:
			key_event(cpu, PAD_LEFT, RELEASE);
			break;
		case SDLK_d:
			key_event(cpu, PAD_RIGHT, RELEASE);
			break;
		case SDLK_w:
			key_event(cpu, PAD_UP, RELEASE);
			break;
		case SDLK_s:
			key_event(cpu, PAD_DOWN, RELEASE);
			break;
		case SDLK_j:
			key_event(cpu, A, RELEASE);
			break;
		case SDLK_k:
			key_event(cpu, B, RELEASE);
			break;
		case SDLK_u:
			key_event(cpu, START, RELEASE);
			break;
		case SDLK_i:
			key_event(cpu, SELECT, RELEASE);
			break;
		}
		break;
	}
}

