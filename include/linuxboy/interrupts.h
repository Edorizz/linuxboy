#ifndef LINUXBOY_INTERRUPTS_H
#define LINUXBOY_INTERRUPTS_H

#include <linuxboy/cpu.h>

/* INTERRUPT REGISTERS */
#define IE			0xFFFF
#define IF			0xFF0F

enum interrupts	{ VBLANK, LCD_STAT, TIMER, SERIAL, JOYPAD, INTERRUPT_MAX };

void request_interrupt(gb_cpu *cpu, BYTE interrupt);
void handle_interrupts(gb_cpu *cpu);

#endif /* LINUXBOY_INTERRUPTS_H */
