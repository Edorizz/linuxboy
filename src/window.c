/* Header file */
#include <linuxboy/window.h>
/* C Library */
#include <stdlib.h>
/* OpenGL */
#include <GL/glew.h>

const key_pair joypad_input[8] = { { SDLK_j, BUTTON_A }, { SDLK_k, BUTTON_B },
				   { SDLK_u, BUTTON_START }, { SDLK_i, BUTTON_START },
				   { SDLK_d, PAD_RIGHT }, { SDLK_a, PAD_LEFT },
				   { SDLK_w, PAD_UP }, { SDLK_s, PAD_DOWN } };

int
create_window(gl_window *win, BYTE *joypad, BYTE *emu_flags)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		fprintf(stderr, "Video initialization error: %s\n", SDL_GetError());
		return -1;
	}

	win->sdl_win = SDL_CreateWindow("Linuxboy Gameboy emulator",
					SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 160, 144,
					SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

	if (win->sdl_win == NULL) {
		fprintf(stderr, "Window creation error: %s\n", SDL_GetError());
		return -1;
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

	if (SDL_GL_CreateContext(win->sdl_win) == NULL) {
		fprintf(stderr, "GL Context creation error: %s\n", SDL_GetError());
		return -1;
	}

	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "GLEW initialization error\n");
		return -1;
	}

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	win->joypad = joypad;
	win->emu_flags = emu_flags;

	return 0;
}

void
delete_window(gl_window *win)
{
	SDL_DestroyWindow(win->sdl_win);
	SDL_Quit();

	win->sdl_win = NULL;
	win->joypad = win->emu_flags = NULL;
}

void
swap_window(gl_window *win)
{
	SDL_GL_SwapWindow(win->sdl_win);
}

void
joypad_event(gl_window *win, int event)
{
	if (event & BIT(EVENT_PRESS))
		*win->joypad &= ~BIT(event & 0xFF);
	else if (event & BIT(EVENT_RELEASE))
		*win->joypad |= BIT(event & 0xFF);
}

void
handle_joypad(gl_window *win, SDL_Event *event)
{
	for (int i = 0; i != 8; ++i) {
		if (joypad_input[i].scancode == event->key.keysym.sym) {
			joypad_event(win, BIT(joypad_input[i].scancode) |
				     (event->type == SDL_KEYDOWN ? BIT(EVENT_PRESS) : BIT(EVENT_RELEASE)));

			return;
		}
	}
}

void
handle_input(gl_window *win)
{
	SDL_Event event;

	do {
		while (SDL_PollEvent(&event)) {
			handle_joypad(win, &event);

			switch (event.type) {
			case SDL_QUIT:
				*win->emu_flags |= BIT(QUIT);
				return;
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym) {
				case SDLK_q:
					*win->emu_flags |= BIT(QUIT);
					break;
				case SDLK_p:
					*win->emu_flags ^= BIT(DEBUG);
					break;
				case SDLK_n:
					return;
				}
				break;
			}
		}
	} while (!(*win->emu_flags & BIT(QUIT)) && *win->emu_flags & BIT(DEBUG));
}

