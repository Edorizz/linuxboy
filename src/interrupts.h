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

#ifndef LINUXBOY_INTERRUPTS_H
#define LINUXBOY_INTERRUPTS_H

/* -==+ Interrupt Registers +==- */
#define IE			0xFFFF
#define IF			0xFF0F

/* -==+ Interrupts +==- */
enum interrupts	{ VBLANK, LCD_STAT, TIMER, SERIAL, JOYPAD, INTERRUPT_MAX };

#endif /* LINUXBOY_INTERRUPTS_H */

