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

#ifndef LINUXBOY_CARTRIDGE_H
#define LINUXBOY_CARTRIDGE_H

#include "utils.h"

#define CARTRIDGE_SIZE	0x200000
#define CARTRIDGE_TYPE	0x147

enum mbc { MBC_1, MBC_2, RAM_ENABLE, RAM_CHANGE };

typedef struct {
	const char *rom_path;
	BYTE rom[CARTRIDGE_SIZE];
	BYTE flags;
	int rom_size;
	BYTE rom_bank;
	BYTE ram_bank;
} gb_cartridge;

int load_cartridge(gb_cartridge *cart);

#endif /* LINUXBOY_CARTRIDGE_H */

