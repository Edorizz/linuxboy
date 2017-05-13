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
		
		fprintf(stderr, "Reading %d bytes (%x hex)\n",
			cart->rom_size, cart->rom_size);

		if (fread(cart->rom, sizeof(BYTE), cart->rom_size, fp) != cart->rom_size) {
			cart->rom_size = 0;
			fprintf(stderr, "Failed to read file %s\n", cart->rom_path);
			fclose(fp);
			return -1;
		}

		switch (cart->rom[CARTRIDGE_TYPE]) {
		case 1:
		case 2:
		case 3:
			cart->flags |= BIT(MBC_1);
			break;
		case 4:
		case 5:
			cart->flags |= BIT(MBC_2);
			break;
		}

		fclose(fp);
	}

	return 0;
}

