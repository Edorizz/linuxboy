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

#ifndef LINUXBOY_UTILS_H
#define LINUXBOY_UTILS_H

#include <stdio.h>

#define MAX(a, b)		((a) > (b) ? (a) : (b))
#define MIN(a, b)		((a) < (b) ? (a) : (b))

/* BYTE MANIPULATION */
#define LO(word)		((word) & 0x00FF)		/* *((BYTE*)&w) */
#define HI(word)		(((word) & 0xFF00) >> 8)	/* *((BYTE*)&w + 1) */

/* BIT MANIPULATION */
#define FLAG(cpu)		((cpu)->regs[REG_AF].lo)
#define FLAG_P(cpu)		(&(FLAG(cpu)))
#define BIT(b)			(1 << (b))
#define RESET_FLAGS(f, b)	((f) &= ~(b))

/* SCREEN DIMENSIONS */
#define SCR_W			160
#define SCR_H			144

#define IMPLEMENT(s)		(fprintf(stderr, "Missing implementation: %s", (s)))

typedef unsigned char	BYTE;
typedef signed char	SIGNED_BYTE;
typedef unsigned short	WORD;
typedef signed short	SIGNED_WORD;

#endif /* LINUXBOY_UTILS_H */
