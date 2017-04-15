#ifndef LINUXBOY_CARTRIDGE_H
#define LINUXBOY_CARTRIDGE_H

#include <linuxboy/utils.h>

#define CARTRIDGE_SIZE	0x200000

typedef struct {
	char *rom_path;
	BYTE rom[CARTRIDGE_SIZE];
	int rom_size;
} gb_cartridge;

int load_cartridge(gb_cartridge *cart);

#endif /* LINUXBOY_CARTRIDGE_H */

