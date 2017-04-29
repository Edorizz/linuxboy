/* Header file */
#include <linuxboy/cpu.h>
/* C library */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
/* Linuxboy */
#include <linuxboy/utils.h>
#include <linuxboy/opcodes.h>
#include <linuxboy/timers.h>
#include <linuxboy/interrupts.h>

const color colors[MAX_COLORS] = { { 224, 248, 208 },	/* WHITE */
				   { 136, 192, 112 },	/* LIGHT_GRAY */
				   {  52, 104,  86 },	/* DARK_GRAY */
				   {   8,  24,  32 } };	/* BLACK */

/* NOT USABLE */
void
bootstrap(gb_cpu *cpu)
{
	FILE *fp;
	
	if ((fp = fopen("bootstrap.gb", "rb")) == NULL) {
		fprintf(stderr, "Failed to open bootstrap.gb: %s\n", strerror(errno));
		return;
	}
	
	cpu->pc = 0x0;
	fread(&cpu->memory[cpu->pc], 1, 0x100, fp);
	
	while (cpu->pc != 0x100) {
		cpu_status(cpu);
		
		if (exec_op(cpu) == -1)
			return;
		
		getchar();
	}
}

int
power_cpu(gb_cpu *cpu)
{
	/* Execute the DMG bootstrap */
	memset(cpu->memory, 0, 0x10000);
	/* bootstrap(cpu); */
	
	/* Load the first two ROM banks (32KB) into main mamery */
	memcpy(cpu->memory, cpu->cart->rom, 0x8000);
	
	/* Initialize Gameboy */
	cpu->pc = 0x100;
	cpu->stack = (reg*)&cpu->memory[0xFFFE];
	
	/* Initialize CPU registers */
	cpu->regs[REG_AF].reg = 0x01B0;
	cpu->regs[REG_BC].reg = 0x0013;
	cpu->regs[REG_DE].reg = 0x00D8;
	cpu->regs[REG_HL].reg = 0x014D;

	/* Initialize special registers */
	cpu->memory[LCD_CONTROL] = 0x91;
	cpu->memory[LCD_STATUS] = 0x85;
	cpu->memory[IE] = 0x00;
	cpu->memory[IF] = 0xE1;

	/* Reset joypad */
	cpu->joypad = 0xFF;
	
	/* Initialize memory */
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
	
	/* Enable interrupts */
	cpu->ime = 1;
	
	/* Initialize timers */
	cpu->divider_cnt = CLOCK_RATE / 16384;
	cpu->timer_cnt = CLOCK_RATE / 4096;
	cpu->scanline_cnt = CLOCK_RATE / 9198;
	
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
	WORD address = val * 100;
	
	for (int i = 0; i != 0xA0; ++i)
		write_byte(cpu, 0xFE00 + i,
			   read_byte(cpu, address + i));
}

void
set_frequency(gb_cpu *cpu)
{
	switch (read_byte(cpu, TMC) & 0x3) {
	case 0:
		cpu->timer_cnt += CLOCK_RATE / 4096;
		break;
	case 1:
		cpu->timer_cnt += CLOCK_RATE / 262144;
		break;
	case 2:
		cpu->timer_cnt += CLOCK_RATE / 65536;
		break;
	case 3:
		cpu->timer_cnt += CLOCK_RATE / 16384;
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
	/* UPDATE DIVIDER REGISTER */
	divider_register(cpu, ops);
	
	/* UPDATE TIMER */
	if (read_byte(cpu, TMC) & BIT(2)) {
		if ((cpu->timer_cnt -= ops) <= 0) {
			set_frequency(cpu);
			
			if (read_byte(cpu, TIMA) == 255) {
				write_byte(cpu, TIMA,
					   read_byte(cpu, TMA));
				request_interrupt(cpu, TIMER);
			} else {
				write_byte(cpu, TIMA,
					   read_byte(cpu, TIMA) + 1);
			}
		}
	}
}

void
set_lcd_status(gb_cpu *cpu)
{
	BYTE status;
	BYTE prev_mode;
	BYTE scanline;
	
	prev_mode = (status = read_byte(cpu, LCD_STATUS)) & 0x3;
	scanline = read_byte(cpu, CURR_SCANLINE);
	if (read_byte(cpu, LCD_CONTROL) & BIT(7)) {
		if (scanline > 144) {
			/* MODE 1 */
			status = ((status >> 2) << 2) | 0x1;
			prev_mode |= BIT(3);
		} else {
			if (cpu->scanline_cnt > 456 - 80) {
				/* MODE 2 */
				status = ((status >> 2) << 2) | 0x2;
				prev_mode |= BIT(3);
			} else if (cpu->scanline_cnt > 456 - 252) {
				/* MODE 3 */
				status = ((status >> 2) << 2) | 0x3;
			} else {
				/* MODE 0 */
				status = (status >> 2) << 2;
				prev_mode |= BIT(3);
			}
		}

		if (prev_mode & BIT(3) && ((prev_mode & 0x3) != (status & 0x3)))
			request_interrupt(cpu, LCD_STAT);

		if (read_byte(cpu, TARGET_SCANLINE) == scanline) {
			status |= BIT(2);
			if (status & BIT(6))
				request_interrupt(cpu, LCD_STAT);
		} else {
			status &= ~BIT(2);
		}
	} else {
		status = ((status >> 2) << 2) | 0x1;
		cpu->scanline_cnt = 456;
		write_byte(cpu, CURR_SCANLINE, 0);
	}
	
	write_byte(cpu, LCD_STATUS, status);
}

void
load_rom_bank(gb_cpu *cpu)
{
	if (cpu->cart->rom_bank == 0)
		++cpu->cart->rom_bank;

	memcpy(&cpu->memory[0x4000], &cpu->cart->rom[cpu->cart->rom_bank * 0x4000], 0x4000);
}

void
update_graphics(gb_cpu *cpu, int ops)
{
	BYTE scanline;
	
	set_lcd_status(cpu);
	
	if (read_byte(cpu, LCD_CONTROL) & BIT(7)) {
		if ((cpu->scanline_cnt -= ops) <= 0) {
			++cpu->memory[CURR_SCANLINE];
			scanline = read_byte(cpu, CURR_SCANLINE);
			cpu->scanline_cnt += 456;
			
			if (scanline == 144) {
				draw_scanline(cpu);
				request_interrupt(cpu, VBLANK);
			} else if (scanline == 153)
				cpu->memory[CURR_SCANLINE] = 0;
			else if (scanline < 144)
				draw_scanline(cpu);
		}
	}
}

void
draw_scanline(gb_cpu *cpu)
{
	BYTE lcd, scanline, scroll_x, scroll_y, id, *data;

	lcd = read_byte(cpu, LCD_CONTROL);
	scanline = read_byte(cpu, CURR_SCANLINE) - 1;
	scroll_x = read_byte(cpu, SCROLL_X);
	scroll_y = read_byte(cpu, SCROLL_Y);

	/* Draw tiles */
	if (lcd & BIT(0)) {
		for (int i = 0; i * 8 - scroll_x < SCR_W; ++i) {
			id = cpu->memory[(0x9800 + (lcd & BIT(3)) * 0x400) +
					 ((scroll_y + scanline) / 8 % 32 * 32) +
					 ((scroll_x / 8 + i) % 32)];
			data = get_tile(cpu, id);
			
			draw_tile_row(cpu, data + (scroll_y + scanline) % 8 * 2,
				      i == 0 ? scroll_x % 8 : 0, scanline, MAX(i * 8 - scroll_x % 8, 0), cpu->memory[0xFF47]);
		}
	}

	/* Draw window */
	if (lcd & BIT(5)) {
		/* TODO */
	}

	/* Draw sprites */
	if (lcd & BIT(1)) {
		/* TODO */
	}
}

/* TODO: Too much malloc! */
void
flip_screen(gb_cpu *cpu)
{
	BYTE *cpy, *top_row, *bot_row;

	cpy = malloc(SCR_W * 3);
	for (int i = 0; i != SCR_H / 2; ++i) {
		top_row = &cpu->scr_buf[SCR_H - i - 1][0][0];
		bot_row = &cpu->scr_buf[i][0][0];

		memcpy(cpy, bot_row, SCR_W * 3);
		memcpy(bot_row, top_row, SCR_W * 3);
		memcpy(top_row, cpy, SCR_W * 3);
	}

	free(cpy);
}

void
clear_screen(gb_cpu *cpu, int color)
{
	for (int i = 0; i != SCR_H; ++i)
		for (int j = 0; j != SCR_W; ++j)
			memcpy(cpu->scr_buf[i][j], &colors[color], 3);
}

BYTE *
get_tile(gb_cpu *cpu, BYTE id)
{
	if (cpu->memory[LCD_CONTROL] & BIT(4))
		return &cpu->memory[0x8000 + id * 16];
	else
		return &cpu->memory[0x8800 + (128 + *(SIGNED_BYTE*)&id) * 16];
}

void
draw_tile_row(gb_cpu *cpu, const BYTE *data, int offset, int screen_y, int screen_x, BYTE palette)
{
	BYTE b1, b2, color;

	b1 = *data;
	b2 = *(data + 1);

	for (int i = offset; i != 8 && screen_x + i - offset < SCR_W; ++i) {
		color = ((b2 >> (7 - i)) << 1 & 0x2) | ((b1 >> (7 - i)) & 0x1);
		color = (palette >> (color * 2)) & 0x3;

		memcpy(cpu->scr_buf[screen_y][screen_x + i - offset], &colors[color], 3);
	}
}

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
	printf("\nsp(%04x): %04x\n", (WORD)((BYTE*)cpu->stack - cpu->memory), cpu->stack->reg);
	
	/* Pretty flag */
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
}

