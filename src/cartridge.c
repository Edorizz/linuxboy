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

/* Header file */
#include "cartridge.h"

int
load_cartridge(gb_cartridge *cart)
{
	FILE *fp;

	if (cart->rom_size == 0) {
		if (cart->rom_path == NULL) {
			fprintf(stderr, "No ROM path detected\n");
			return -1;
		}

		if ((fp = fopen(cart->rom_path, "rb")) == NULL) {
			cart->rom_size = 0;
			fprintf(stderr, "Failed to open file %s\n", cart->rom_path);
			return -1;
		}
		
		fseek(fp, 0, SEEK_END);
		cart->rom_size = ftell(fp);
		fseek(fp, 0, SEEK_SET);
		
		fprintf(stderr, "Reading %d bytes (%x hex)\n", cart->rom_size, cart->rom_size);

		if (fread(cart->rom, sizeof(BYTE), cart->rom_size, fp) != cart->rom_size) {
			cart->rom_size = 0;
			fprintf(stderr, "Failed to read file %s\n", cart->rom_path);
			fclose(fp);
			return -1;
		}

		switch (cart->rom[CARTRIDGE_TYPE]) {
		case 0:
			cart->flags = 0;
			break;
		case 0x01:
		case 0x02:
		case 0x03:
		     	cart->flags |= BIT(MBC_1);
		     	break;
		case 0x05:
		case 0x06:
			cart->flags |= BIT(MBC_2);
			break;
		default:
			printf("Unsupported cartridge type (%d)\n", cart->rom[CARTRIDGE_TYPE]);
			return -1;
			break;
		}
		printf("Cartridge type (%d)\n", cart->rom[CARTRIDGE_TYPE]);

		fclose(fp);
	}

	return 0;
}

