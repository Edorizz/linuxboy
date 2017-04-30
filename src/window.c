/* Header file */
#include <linuxboy/window.h>
/* C Library */
#include <stdlib.h>
/* OpenGL */
#include <GL/glew.h>
/* Linuxboy */
#include <linuxboy/glsl_shader.h>

const key_pair joypad_input[8] = { { SDLK_j, BUTTON_A }, { SDLK_k, BUTTON_B },
				   { SDLK_i, BUTTON_SELECT }, { SDLK_u, BUTTON_START },
				   { SDLK_d, PAD_RIGHT }, { SDLK_a, PAD_LEFT },
				   { SDLK_w, PAD_UP }, { SDLK_s, PAD_DOWN } };

int
create_window(gl_window *win, BYTE *joypad, BYTE *emu_flags)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		fprintf(stderr, "Video initialization error: %s\n", SDL_GetError());
		return -1;
	}

	win->sdl_win = SDL_CreateWindow("Linuxboy",
					SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
					win->width, win->height,
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

	SDL_GL_SetSwapInterval(1);
	
	win->joypad = joypad;
	win->emu_flags = emu_flags;

	init_gl(win);

	return 0;
}

void
delete_window(gl_window *win)
{
	/* Terminate SDL */
	SDL_DestroyWindow(win->sdl_win);
	SDL_Quit();

	/* Delete OpenGL buffers */
	glDeleteVertexArrays(1, &win->vao);
	glDeleteBuffers(1, &win->vbo);
	glDeleteTextures(1, &win->texture);

	/* Just being a good guy */
	win->sdl_win = NULL;
	win->joypad = win->emu_flags = NULL;
}

void
swap_window(gl_window *win)
{
	SDL_GL_SwapWindow(win->sdl_win);
}

void
init_gl(gl_window *win)
{
	/* Quad vertices */
	static const GLfloat vertex_data[] = { -1.0f,  1.0f, 1.0f, 0.0f, 1.0f,
					       -1.0f, -1.0f, 1.0f, 0.0f, 0.0f,
					        1.0f, -1.0f, 1.0f, 1.0f, 0.0f,
					        1.0f, -1.0f, 1.0f, 1.0f, 0.0f,
					        1.0f,  1.0f, 1.0f, 1.0f, 1.0f,
					       -1.0f,  1.0f, 1.0f, 0.0f, 1.0f };
	
	/* Basic shaders */
	win->shader = compile_program("res/vertex.glsl", "res/fragment.glsl");
	
	/* Screen quad */
	glGenVertexArrays(1, &win->vao);
	glGenBuffers(1, &win->vbo);
	
	glBindVertexArray(win->vao);
	
	glBindBuffer(GL_ARRAY_BUFFER, win->vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data, GL_STATIC_DRAW);
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, (GLvoid*)(sizeof(GLfloat) * 3));
	glEnableVertexAttribArray(1);
	
	glBindVertexArray(0);
	
	/* Display texture */
	glGenTextures(1, &win->texture);
	glBindTexture(GL_TEXTURE_2D, win->texture);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_W, SCR_H, 0, GL_RGB, GL_UNSIGNED_BYTE, win->scr_buf);
	
	glBindTexture(GL_TEXTURE_2D, 0);
}

void
render(gl_window *win)
{
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(win->shader);
	
	glBindTexture(GL_TEXTURE_2D, win->texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
		     SCR_W, SCR_H, 0, GL_RGB, GL_UNSIGNED_BYTE, win->scr_buf);
	glBindVertexArray(win->vao);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	
	glUseProgram(0);
}

void
joypad_event(gl_window *win, int event)
{
	if (event & BIT(EVENT_PRESS))
		*win->joypad &= ~(event & 0xFF);
	else if (event & BIT(EVENT_RELEASE))
		*win->joypad |= event & 0xFF;
}

void
handle_joypad(gl_window *win, SDL_Event *event)
{
	for (int i = 0; i != 8; ++i) {
		if (joypad_input[i].scancode == event->key.keysym.sym) {
			joypad_event(win, BIT(joypad_input[i].joypad_bit) |
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
				case SDLK_m:
					*win->emu_flags |= BIT(MAP_DUMP);
				case SDLK_n:
					return;
				case SDLK_v:
					SDL_GL_SetSwapInterval(!SDL_GL_GetSwapInterval());
					return;
				}
				break;
			}
		}
	} while (!(*win->emu_flags & BIT(QUIT)) && *win->emu_flags & BIT(DEBUG));
}

