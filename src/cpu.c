/* Header file */
#include "cpu.h"
/* C library */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
/* Linuxboy */
#include "gpu.h"
#include "utils.h"
#include "opcodes.h"
#include "timers.h"
#include "interrupts.h"

/* NOT USABLE */
void
load_bootstrap(gb_cpu *cpu)
{
	FILE *fp;
	
	if ((fp = fopen("bootstrap.gb", "rb")) == NULL) {
		fprintf(stderr, "Failed to open bootstrap.gb: %s\n", strerror(errno));
		return;
	}
	
	fread(cpu->memory, 1, 0x100, fp);
}

int
power_cpu(gb_cpu *cpu)
{
	/* Execute the DMG bootstrap */
	memset(cpu->memory, 0, 0x10000);
	
	/* Load the first two ROM banks (32KB) into main mamery */
	memcpy(cpu->memory, cpu->cart->rom, 0x8000);
	load_bootstrap(cpu);
	
	/* Initialize Gameboy */
	cpu->pc = 0x0;
	cpu->stack = 0xFFFE;
	
	/* Initialize CPU registers
	cpu->regs[REG_AF].reg = 0x01B0;
	cpu->regs[REG_BC].reg = 0x0013;
	cpu->regs[REG_DE].reg = 0x00D8;
	cpu->regs[REG_HL].reg = 0x014D;
	*/

	/* Initialize special registers */
	cpu->memory[LCD_CONTROL] = /*0x91*/ 0x00;
	cpu->memory[LCD_STATUS] = /*0x85*/0x84;
	cpu->memory[IE] = 0x00;
	cpu->memory[IF] = 0xE1;

	/* Reset joypad */
	cpu->joypad = 0xFF;
	
	cpu->memory[0xFF05] = 0x00;
	cpu->memory[0xFF06] = 0x00;
	cpu->memory[0xFF07] = 0xF8;

	/* Initialize memory
	cpu->memory[0xFF05] = 0x00; 
	cpu->memory[0xFF06] = 0x00; 
	cpu->memory[0xFF07] = 0x00; 
	cpu->memory[0xFF10] = 0x80; 
	cpu->memory[0xFF11] = 0xBF; 
	cpu->memory[0xFF12] = 0xF3; 
	cpu->memory[0xFF14] = 0xBF; 
	cpu->memory[0xFF16] = 0x3F; 
	cpu->memory[0xFF17] = 0x00; 
	cpu->memory[0xFF19] = 0xBF; 
	cpu->memory[0xFF1A] = 0x7F; 
	cpu->memory[0xFF1B] = 0xFF; 
	cpu->memory[0xFF1C] = 0x9F; 
	cpu->memory[0xFF1E] = 0xBF; 
	cpu->memory[0xFF20] = 0xFF; 
	cpu->memory[0xFF21] = 0x00; 
	cpu->memory[0xFF22] = 0x00; 
	cpu->memory[0xFF23] = 0xBF; 
	cpu->memory[0xFF24] = 0x77; 
	cpu->memory[0xFF25] = 0xF3;
	cpu->memory[0xFF26] = 0xF1; 
	cpu->memory[0xFF40] = 0x91; 
	cpu->memory[0xFF42] = 0x00; 
	cpu->memory[0xFF43] = 0x00; 
	cpu->memory[0xFF45] = 0x00; 
	cpu->memory[0xFF47] = 0xFC; 
	cpu->memory[0xFF48] = 0xFF; 
	cpu->memory[0xFF49] = 0xFF; 
	cpu->memory[0xFF4A] = 0x00; 
	cpu->memory[0xFF4B] = 0x00; 
	cpu->memory[0xFFFF] = 0x00;
	*/
	
	/* Enable interrupts
	cpu->ime = 1;
	*/
	
	/* Initialize timers */
	cpu->divider_cnt = CLOCK_RATE / 16384;
	cpu->timer_cnt = CLOCK_RATE / 4096;
	/*cpu->scanline_cnt = CLOCK_RATE / 9198;*/
	cpu->scanline_cnt = 0;
	
	return 0;
}

int
exec_op(gb_cpu *cpu)
{
	const op *opcode;
	void *arg;
	
	opcode = &ops[cpu->memory[cpu->pc]];
	arg = &cpu->memory[cpu->pc + 1];
	
	if (opcode->func == NULL) {
		printf("Missing implementation %s at $%04x\n", opcode->assembly, cpu->pc);
		exit(1);
	}

	if (opcode->arg_size & SIGNED) {
		cpu->pc += opcode->arg_size - SIGNED;
		
		switch (opcode->arg_size - SIGNED) {
		case 1:
			return ((int(*)(gb_cpu*))opcode->func)(cpu);
		case 2:
			return ((int(*)(gb_cpu*, SIGNED_BYTE))opcode->func)(cpu, *(SIGNED_BYTE*)arg);
		case 3:
			return ((int(*)(gb_cpu*, SIGNED_WORD))opcode->func)(cpu, *(SIGNED_WORD*)arg);
		}
	} else {
		cpu->pc += opcode->arg_size;
		
		switch (opcode->arg_size) {
		case 1:
			return ((int(*)(gb_cpu*))opcode->func)(cpu);
		case 2:
			return ((int(*)(gb_cpu*, BYTE))opcode->func)(cpu, *(BYTE*)arg);
		case 3:
			return ((int(*)(gb_cpu*, WORD))opcode->func)(cpu, *(WORD*)arg);
		}
	}
	
	return 0;
}

void
dma_transfer(gb_cpu *cpu, BYTE val)
{
	WORD address = val << 8;
	
	for (int i = 0; i != 0xA0; ++i) {
		write_byte(cpu, 0xFE00 + i, read_byte(cpu, address + i));
	}
}

void
set_frequency(gb_cpu *cpu)
{
	switch (read_byte(cpu, TMC) & 0x3) {
	case 0:
		cpu->timer_cnt += /*1024*/1024;
		break;
	case 1:
		cpu->timer_cnt += /*16*/16;
		break;
	case 2:
		cpu->timer_cnt += /*64*/64;
		break;
	case 3:
		cpu->timer_cnt += /*256*/256;
		break;
	}
}

void
divider_register(gb_cpu *cpu, int ops)
{
	if ((cpu->divider_cnt -= ops) < 0) {
		cpu->divider_cnt += 256;
		++cpu->memory[DIVIDER_REGISTER];
	}
}

void
update_timers(gb_cpu *cpu, int ops)
{
	/* update divider register */
	divider_register(cpu, ops);
	
	/* Update timer */
	if (read_byte(cpu, TMC) & BIT(2)) {
		if ((cpu->timer_cnt -= ops) <= 0) {
			set_frequency(cpu);
			
			if (read_byte(cpu, TIMA) == 255) {
				write_byte(cpu, TIMA, read_byte(cpu, TMA));

				request_interrupt(cpu, TIMER);
			} else {
				write_byte(cpu, TIMA, read_byte(cpu, TIMA) + 1);
			}
		}
	}
}

void
load_rom_bank(gb_cpu *cpu)
{
	if (cpu->cart->rom_bank == 0) {
		++cpu->cart->rom_bank;
	}

	memcpy(&cpu->memory[0x4000], &cpu->cart->rom[cpu->cart->rom_bank * 0x4000], 0x4000);
}

void
load_ram_bank(gb_cpu *cpu)
{
	if (cpu->cart->ram_bank == 0) {
		++cpu->cart->ram_bank;
	}

	memcpy(&cpu->memory[0xA000], &cpu->cart->rom[cpu->cart->ram_bank * 0x2000], 0x2000);
}

void
update_graphics(gb_cpu *cpu, int ops)
{
	BYTE stat;

	if (read_byte(cpu, LCD_CONTROL) & BIT(7)) {
		cpu->scanline_cnt += ops;

		switch ((stat = cpu->memory[LCD_STATUS]) & 0x3) {
		case 2:
			if (cpu->scanline_cnt >= 80) {
				cpu->scanline_cnt -= 80;

				cpu->memory[LCD_STATUS] = (stat & ~0x3) | 0x3;
			}
			
			break;
		case 3:
			if (cpu->scanline_cnt >= 172) {
				cpu->scanline_cnt -= 172;

				draw_scanline(cpu);
				cpu->memory[LCD_STATUS] = (stat & ~0x3) | 0x0;

				if (stat & BIT(3)) {
					request_interrupt(cpu, LCD_STAT);
				}
			}

			break;
		case 0:
			if (cpu->scanline_cnt >= 204) {
				cpu->scanline_cnt -= 204;

				if (++cpu->memory[CURR_SCANLINE] >= 144) {
					cpu->memory[LCD_STATUS] = (stat & ~0x3) | 0x1;
					request_interrupt(cpu, VBLANK);

					if (stat & BIT(4)) {
						request_interrupt(cpu, LCD_STAT);
					}
				} else {
					cpu->memory[LCD_STATUS] = (stat & ~0x3) | 0x2;

					if (stat & BIT(5)) {
						request_interrupt(cpu, LCD_STAT);
					}
				}
			}

			break;
		case 1:
			if (cpu->scanline_cnt >= 456) {
				cpu->scanline_cnt -= 456;

				if (++cpu->memory[CURR_SCANLINE] >= 153) {
					cpu->memory[LCD_STATUS] = (stat & ~0x3) | 0x2;
					cpu->memory[CURR_SCANLINE] = 0;

					if (stat & BIT(5)) {
						request_interrupt(cpu, LCD_STAT);
					}
				}
			}

			break;
		}

		if (cpu->memory[CURR_SCANLINE] == cpu->memory[TARGET_SCANLINE]) {
			cpu->memory[LCD_STATUS] |= BIT(2);
			if (stat & BIT(6)) {
				request_interrupt(cpu, LCD_STAT);
			}
		} else {
			cpu->memory[LCD_STATUS] &= ~BIT(2);
		}
	} else {
		cpu->memory[LCD_STATUS] &= ~0x3/* |= BIT(2) */;
	}
}

/*
 * -==+ DEBUGGING +==-
 */

void
disassemble(const gb_cpu *cpu)
{
	int pc;
	BYTE op;
	
	pc = 0;
	while (pc != cpu->cart->rom_size) {
		op = cpu->cart->rom[pc];
		
		if (op == 0xCB) {
			op = cpu->cart->rom[pc + 1];
			printf("%6x: %s\n", pc, ext_ops[op].assembly);
			
			pc += ext_ops[op].arg_size + 1;
		} else {
			printf("%6x: %s\n", pc, ops[op].assembly);
			
			pc += ops[op].arg_size;
		}
	}
}

void
map_dump(const gb_cpu *cpu)
{
	FILE *fp = fopen("map.log", "wb");

	for (int i = 0; i != 32; ++i) {
		for (int j = 0; j != 32; ++j) {
			fprintf(fp, "%02x ", cpu->memory[0x9800 + (i * 32) + j]);
		}

		fprintf(fp, "\n");
	}

	fclose(fp);
}

void
cpu_status(const gb_cpu *cpu)
{
	const op *opcode;
	const SIGNED_BYTE *sb;
	const SIGNED_WORD *sw;
	
	BYTE flag;
	
	opcode = &ops[cpu->memory[cpu->pc]];
	
	printf("\x1B[2J\x1B[;H");
	printf("pc: %04x\tasm: (%02x) ", cpu->pc, cpu->memory[cpu->pc]);
	
	if (opcode->arg_size & SIGNED) {
		switch (opcode->arg_size - SIGNED) {
		case 1:
			printf(opcode->assembly);
			break;
		case 2:
			sb = (SIGNED_BYTE*)&cpu->memory[cpu->pc + 1];
			
			printf(opcode->assembly,
			       *sb < 0 ? "-" : "", *sb < 0 ? -*sb : *sb);
			break;
		case 3:
			sw = (SIGNED_WORD*)&cpu->memory[cpu->pc + 1];
			
			printf(opcode->assembly,
			       *sw < 0 ? "-" : "", *sw < 0 ? -*sw : *sw);
			break;
		}
	} else {
		switch (opcode->arg_size) {
		case 1:
			printf(opcode->assembly);
			break;
		case 2:
			printf(opcode->assembly, *(BYTE*)&cpu->memory[cpu->pc + 1]);
			break;
		case 3:
			printf(opcode->assembly, *(WORD*)&cpu->memory[cpu->pc + 1]);
			break;
		}
	}

	/* THIS CAN ALL GO IN ONE BIG PRINTF() BUT THIS WAY LOOKS CLEANER */
	
	/* Stack pointer */
	printf("\nsp(%04x): %02x%02x\n", cpu->stack, cpu->memory[cpu->stack + 1], cpu->memory[cpu->stack]);
	
	/* Pretty little flag */
	flag = cpu->regs[REG_AF].lo;
	printf("\nflags: %c%c%c%c",
	       flag & BIT(FLAG_Z) ? 'Z' : '.',
	       flag & BIT(FLAG_N) ? 'N' : '.',
	       flag & BIT(FLAG_H) ? 'H' : '.',
	       flag & BIT(FLAG_C) ? 'C' : '.');
	
	/* Print registers */
	printf("\nreg_af: %04x\n"
	       "reg_bc: %04x\n"
	       "reg_de: %04x\n"
	       "reg_hl: %04x\n",
	       cpu->regs[REG_AF].reg,
	       cpu->regs[REG_BC].reg,
	       cpu->regs[REG_DE].reg,
	       cpu->regs[REG_HL].reg);

	/* Print other stuff */
	printf("\nie: %02x\tif: %02x\time: %d",
	       cpu->memory[IE],
	       cpu->memory[IF],
	       cpu->ime);

	printf("\nlcd: %02x\tstat: %02x\n",
	       cpu->memory[LCD_CONTROL],
	       cpu->memory[LCD_STATUS]);

	printf("\n ==+ TIMING +==\n");

	printf("\nscanline_cnt: %d\n",
	       cpu->scanline_cnt);

	printf("timer_cnt: %d\n"
	       "TIMA: %d (%.2x)\n"
	       "TMA: %d (%.2x)\n"
	       "TMC: %d (%.2x)\n",
	       cpu->timer_cnt,
	       cpu->memory[TIMA], cpu->memory[TIMA],
	       cpu->memory[TMA], cpu->memory[TMA],
	       cpu->memory[TMC], cpu->memory[TMC]);
}

/*
 * -==+ OPCODE HELPERS +==-
 */

/* DOESN'T MODIFY GAME MEMORY */
BYTE
inc_byte(BYTE *flag, BYTE b)
{
	RESET_FLAGS(*flag, BIT(FLAG_Z) | BIT(FLAG_N) | BIT(FLAG_H));
	
	if ((BYTE)(b + 1) == 0) {
		*flag |= BIT(FLAG_Z);
	}
	
	/* Half-carry if bit 4 changed (not sure) */
	if ((b ^ (BYTE)(b + 1)) & BIT(4)) {
		*flag |= BIT(FLAG_H);
	}
	
	return b + 1;
}

/* DOESN'T MODIFY GAME MEMORY */
BYTE
dec_byte(BYTE *flag, BYTE b)
{
	RESET_FLAGS(*flag, BIT(FLAG_Z) | BIT(FLAG_H));
	*flag |= BIT(FLAG_N);
	
	if ((BYTE)(b - 1) == 0) {
		*flag |= BIT(FLAG_Z);
	}
	
	/* Half-carry if lower nibble is 0 */
	if (!(b & 0x0F)) {
		*flag |= BIT(FLAG_H);
	}
	
	return b - 1;
}

void
swap_byte(BYTE *flag, BYTE *b)
{
	BYTE nibble;
	RESET_FLAGS(*flag, BIT(FLAG_C) | BIT(FLAG_H) | BIT(FLAG_N) | BIT(FLAG_Z));
	
	nibble = *b & 0x0F;
	*b = (*b >> 4) | (nibble << 4);
	
	if (*b == 0) {
		*flag |= BIT(FLAG_Z);
	}
}

void
rot_byte(BYTE *flag, BYTE *b, BYTE rot_flags)
{
	BYTE bit, prev_c; 

	prev_c = *flag & BIT(FLAG_C);
	RESET_FLAGS(*flag, BIT(FLAG_C) | BIT(FLAG_H) | BIT(FLAG_N) | BIT(FLAG_Z)); /* ? */
	
	if (rot_flags & LEFT) {
		bit = *b >> 7;
		
		*b <<= 1;
		if (rot_flags & CIRCULAR) {
			*b |= bit;
		} else {
			*b |= (prev_c >> FLAG_C) & 1;
		}
	} else {
		bit = *b & 1;
		
		*b >>= 1;
		if (rot_flags & CIRCULAR) {
			*b |= bit << 7;
		} else {
			*b |= (prev_c) << (7 - FLAG_C);
		}
	}
	
	if (*b == 0) {
		*flag |= BIT(FLAG_Z);
	}
	
	*flag ^= (-bit ^ *flag) & BIT(FLAG_C);
}

void
shift_byte(BYTE *flag, BYTE *b, BYTE shift_flags)
{
	BYTE bit;

	RESET_FLAGS(*flag, BIT(FLAG_C) | BIT(FLAG_H) | BIT(FLAG_N) | BIT(FLAG_Z));

	if (shift_flags & BIT(LEFT)) {
		bit = *b >> 7;

		*b <<= 1;
	} else {
		/* Need to keep msb and lsb */
		bit = (*b >> 6) & 2;
		bit |= *b & 1;

		*b >>= 1;
		bit &= 1;
		if (shift_flags & BIT(ARITHMETIC)) {
			*b |= (bit >> 1) << 7;
		}
	}

	if (*b == 0) {
		*flag |= BIT(FLAG_Z);
	}
	
	*flag ^= (-bit ^ *flag) & BIT(FLAG_C);
}

void
add_byte(BYTE *flag, BYTE *b, int val)
{
	RESET_FLAGS(*flag, BIT(FLAG_C) | BIT(FLAG_H) | BIT(FLAG_N) | BIT(FLAG_Z));
	
	if (*b + val > 0xFF) {
		*flag |= BIT(FLAG_C);
	}
	
	if ((*b & 0x0F) + (val & 0x0F) > 0x0F) {
		*flag |= BIT(FLAG_H);
	}
	
	*b += val;
	
	if (*b == 0) {
		*flag |= BIT(FLAG_Z);
	}
}

void
add_word(BYTE *flag, WORD *w, int val)
{
	RESET_FLAGS(*flag, BIT(FLAG_N));
	
	*flag ^= (-(*w + val > 0xFFFF) ^ *flag) & BIT(FLAG_C);
	*flag ^= (-((*w & 0x0FFF) + (val & 0x0FFF) > 0x0FFF) ^ *flag) & BIT(FLAG_H);
	
	*w += val;
}

void
sub_byte(BYTE *flag, BYTE *b, BYTE val)
{
	RESET_FLAGS(*flag, BIT(FLAG_C) | BIT(FLAG_H) | BIT(FLAG_Z));
	*flag |= BIT(FLAG_N);
	
	if (*b < val) {
		*flag |= BIT(FLAG_C);
	}
	
	if ((val & 0x0F) > (*b & 0x0F)) {
		*flag |= BIT(FLAG_H);
	}
	
	*b -= val;
	
	if (*b == 0) {
		*flag |= BIT(FLAG_Z);
	}
}

void
and_byte(BYTE *flag, BYTE *b, BYTE val)
{
	RESET_FLAGS(*flag, BIT(FLAG_C) | BIT(FLAG_N) | BIT(FLAG_Z));
	*flag |= BIT(FLAG_H);
	
	*b &= val;
	
	if (*b == 0) {
		*flag |= BIT(FLAG_Z);
	}
}

void
xor_byte(BYTE *flag, BYTE *b, BYTE val)
{
	RESET_FLAGS(*flag, BIT(FLAG_C) | BIT(FLAG_H) | BIT(FLAG_N) | BIT(FLAG_Z));
	
	*b ^= val;
	
	if (*b == 0) {
		*flag |= BIT(FLAG_Z);
	}
}

void
or_byte(BYTE *flag, BYTE *b, BYTE val)
{
	RESET_FLAGS(*flag, BIT(FLAG_C) | BIT(FLAG_H) | BIT(FLAG_N) | BIT(FLAG_Z));
	
	*b |= val;
	
	if (*b == 0) {
		*flag |= BIT(FLAG_Z);
	}
}

void
cp_byte(BYTE *flag, BYTE b, BYTE val)
{
	sub_byte(flag, &b, val);
}

void
test_bit(BYTE *flag, BYTE b, BYTE bit)
{
	RESET_FLAGS(*flag, BIT(FLAG_N) | BIT(FLAG_Z));
	*flag |= BIT(FLAG_H);

	if (!(b & BIT(bit))) {
		*flag |= BIT(FLAG_Z);
	}
}

BYTE
read_byte(gb_cpu *cpu, WORD addr)
{
	return cpu->memory[addr];
}

WORD
read_word(gb_cpu *cpu, WORD addr)
{
	return *(WORD*)&cpu->memory[addr];
}

void
write_byte(gb_cpu *cpu, WORD addr, BYTE val)
{
	if (addr < 0x2000) {
		/* Enable RAM bank */
		if (cpu->cart->flags & BIT(MBC_2) && addr & BIT(4))
			return;

		if ((val & 0x0F) == 0x0A) {
			cpu->cart->flags |= BIT(RAM_ENABLE);
		} else {
			cpu->cart->flags &= ~BIT(RAM_ENABLE);
		}
	} else if (addr < 0x4000) {
		/* Lo ROM bank change */
		if (cpu->cart->flags & BIT(MBC_1)) {
			cpu->cart->rom_bank = (cpu->cart->rom_bank & 0xE0) | (val & 0x1F);
		} else if (cpu->cart->flags & BIT(MBC_2)) {
			cpu->cart->rom_bank = val & 0x0F;
		}

		load_rom_bank(cpu);
	} else if (addr < 0x6000) {
		/* Hi ROM or RAM bank change */
		if (cpu->cart->flags & BIT(MBC_1)) {
			if (cpu->cart->flags & BIT(RAM_CHANGE)) {
				cpu->cart->ram_bank = val & 0x3;
				load_ram_bank(cpu);
			} else {
				cpu->cart->rom_bank = (cpu->cart->rom_bank & 0x1F) | (val & 0xE0);
				load_rom_bank(cpu);
			}
		}
	} else if (addr < 0x8000) {
		/* ROM/RAM banking selection */
		if (val & BIT(0)) {
			cpu->cart->flags &= ~BIT(RAM_CHANGE);
		} else {
			cpu->cart->flags |= BIT(RAM_CHANGE);
		}
	} else if (addr >= 0xE000 && addr < 0xFE00) {
		/* ECHO memory */
		cpu->memory[addr] = val;
		write_byte(cpu, addr - 0x2000, val);
	} else if (addr >= 0xFEA0 && addr < 0xFF00) {
		/* Not usable */
	} else if (addr == DIVIDER_REGISTER) {
		/* Writing to the divider register resets it */
		cpu->memory[addr] = 0;
	} else if (addr == TMC) {
			set_frequency(cpu);

		cpu->memory[TMC] = val;
	} else if (addr == 0xFF00) {
		if (val & BIT(4) && val & BIT(5)) {
			cpu->memory[addr] = ~(3 << 4);
		} else if (val & BIT(4)) {
			cpu->memory[addr] = (cpu->joypad & 0x0F);
		} else if (val & BIT(5)) {
			cpu->memory[addr] = (cpu->joypad >> 4);
		}
	} else if (addr == IF) {
		cpu->memory[IF] = (cpu->memory[IF] & ~0x1F) | (val & 0x1F);
	} else if (addr == 0xFF46) {
		dma_transfer(cpu, val);
	} else if (addr == 0xFF50) {
		/* Unmap bootrom */
		if (val == 1) {
			memcpy(cpu->memory, cpu->cart->rom, 0x100);
		}
	} else {
		cpu->memory[addr] = val;
	}
}

void
write_word(gb_cpu *cpu, WORD addr, WORD val)
{
	*(WORD*)&cpu->memory[addr] = val;
}

WORD
pop(gb_cpu *cpu)
{
	WORD val = *(WORD*)&cpu->memory[cpu->stack];

	cpu->stack += 2;
	return val;
}

void
push(gb_cpu *cpu, WORD val)
{
	cpu->stack -= 2;

	cpu->memory[cpu->stack + 1] = (val & 0xFF00) >> 8;
	cpu->memory[cpu->stack] = val & 0x00FF;
}

void
call(gb_cpu *cpu, WORD addr)
{
	push(cpu, cpu->pc);
	cpu->pc = addr;
}

void
ret(gb_cpu *cpu)
{
	cpu->pc = pop(cpu);
}

