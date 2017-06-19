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
load_cartridge(gb_cart *cart)
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
		
		fprintf(stderr, "Reading %d bytes (%xh)\n", cart->rom_size, cart->rom_size);

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
		     	cart->flags = MBC1;
		     	break;
		case 0x05:
		case 0x06:
			cart->flags = MBC2;
			break;
		case 0x0F:
		case 0x10:
		case 0x11:
		case 0x12:
		case 0x13:
			cart->flags = MBC3;
			break;
		case 0x19:
		case 0x1A:
		case 0x1B:
		case 0x1C:
		case 0x1D:
		case 0x1E:
			cart->flags = MBC5;
			break;
		default:
			printf("Unsupported cartridge type (%x)\n", cart->rom[CARTRIDGE_TYPE]);
			return -1;
			break;
		}
		printf("Cartridge type (%x)\n", cart->rom[CARTRIDGE_TYPE]);

		fclose(fp);
	}

	return 0;
}

