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

enum emu_flags	{ BREAKPOINT, QUIT, DEBUG, MAP_DUMP, SAVE_STATE, LOAD_STATE };
enum joypad	{ BUTTON_A, BUTTON_B, BUTTON_SELECT, BUTTON_START,
		  PAD_RIGHT, PAD_LEFT, PAD_UP, PAD_DOWN,
		  EVENT, EVENT_PRESS, EVENT_RELEASE };

typedef struct {
	int scancode;
	int joypad_bit;
} key_pair;

typedef struct {
	SDL_Window *sdl_win;
	const BYTE *scr_buf;
	int width, height;
	BYTE *joypad;
	BYTE *emu_flags;
	/* OPENGL */
	GLuint shader;
	GLuint vao, vbo;
	GLuint texture;
} gl_window;

/* WINDOW FUNCTIONS */
int  create_window(gl_window *win, BYTE *joypad, BYTE *emu_flags);
void delete_window(gl_window *win);
void swap_window(gl_window *win);

/* GRAPHICS (OPENGL) */
void init_gl(gl_window *win);
void render(gl_window *win);

/* INPUT FUNCTIONS */
void joypad_event(gl_window *win, int event);
void handle_joypad(gl_window *win, SDL_Event *event);
void handle_input(gl_window *win);

#endif /* LINUBOY_WINDOW_H */

