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

#ifndef LINUXBOY_TIMERS_H
#define LINUXBOY_TIMERS_H

/* TIMING */
#define DIVIDER_REGISTER	0xFF04
#define TIMA			0xFF05
#define TMA			0xFF06
#define TMC			0xFF07

void set_frequency(gb_cpu *cpu);
void divider_register(gb_cpu *cpu, int ops);
void update_timers(gb_cpu *cpu, int ops);

#endif /* LINUXBOY_TIMERS_H */

