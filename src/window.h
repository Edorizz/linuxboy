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

#ifndef LINUXBOY_WINDOW_H
#define LINUXBOY_WINDOW_H

#include <SDL2/SDL.h>
#include <GL/glew.h>

#include "utils.h"

enum emu_flags	{ BREAKPOINT, QUIT, DEBUG, MAP_DUMP, SAVE_STATE, LOAD_STATE, BOOTSTRAP };
enum joypad	{ BUTTON_A, BUTTON_B, BUTTON_SELECT, BUTTON_START,
		  PAD_RIGHT, PAD_LEFT, PAD_UP, PAD_DOWN,
		  EVENT, EVENT_PRESS, EVENT_RELEASE };

typedef struct _key_pair {
	int scancode;
	int joypad_bit;
} key_pair;

/*
 * -==+ OpenGL Window -==+
 * Takes data from an external screen buffer of RGB values
 * (specified by 'scr_buf'), and outputs it to an OpenGL
 * window.
 */
typedef struct _gl_win {
	SDL_Window *sdl_win;
	const BYTE *scr_buf;
	int scr_buf_h, scr_buf_w;
	int win_h, win_w;
	BYTE *joypad;
	BYTE *emu_flags;
	/* OPENGL */
	GLuint shader;
	GLuint vao, vbo;
	GLuint texture;
} gl_win;

/* WINDOW FUNCTIONS */
int  create_window(gl_win *win, BYTE *joypad, BYTE *emu_flags);
void link_scr_buf(gl_win *win, const BYTE *buf, int h, int w);
void delete_window(gl_win *win);
void swap_window(gl_win *win);

/* GRAPHICS (OPENGL) */
void init_gl(gl_win *win);
void render(gl_win *win);

/* INPUT FUNCTIONS */
void joypad_event(gl_win *win, int event);
void handle_joypad(gl_win *win, SDL_Event *event);
void handle_input(gl_win *win);

#endif /* LINUBOY_WINDOW_H */

