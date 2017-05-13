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

