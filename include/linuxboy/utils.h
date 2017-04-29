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
