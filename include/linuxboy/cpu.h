#ifndef LINUXBOY_CPU_H
#define LINUXBOY_CPU_H

#include <linuxboy/utils.h>
#include <linuxboy/cartridge.h>

/* PIXEL COLORS */
#define WHITE			0
#define LIGHT_GRAY		1
#define DARK_GRAY		2
#define BLACK			3
#define MAX_COLORS		4

/* SPECIAL REGISTERS */
#define LCD_CONTROL		0xFF40
#define LCD_STATUS		0xFF41
#define SCROLL_Y		0xFF42
#define SCROLL_X		0xFF43
#define CURR_SCANLINE		0xFF44
#define TARGET_SCANLINE		0xFF45
#define WINDOW_X		0xFF4A
#define WINDOW_Y		0xFF4B

/* OTHER */
#define CLOCK_RATE		4194304

enum regs	{ REG_AF, REG_BC, REG_DE, REG_HL, REG_MAX };
enum cpu_flags	{ FLAG_C = 4, FLAG_H, FLAG_N, FLAG_Z };
enum cpu_status { HALT, STOP };

/* RGB PIXEL */
typedef struct {
	BYTE r, g, b;
} color;

/* 16-BIT REGISTER */
typedef union {
	WORD reg;
	struct {
		BYTE lo;
		BYTE hi;
	};
} reg;

/* CPU */
typedef struct {
	/* GAME MEMORY */
	gb_cartridge *cart;
	BYTE memory[0x10000]; /* 64KB */
	reg regs[REG_MAX];
	WORD pc;
	reg *stack;
	/* TIMING */
	int divider_cnt;
	int timer_cnt;
	int scanline_cnt;
	/* GRAPHICS */
	BYTE scr_buf[SCR_H][SCR_W][3];
	/* SPECIAL REGISTERS */
	BYTE ime; /* Interrupt master switch */
	BYTE joypad;
	BYTE status;
} gb_cpu;

/* CPU FUNCTIONS */
int  power_cpu(gb_cpu *cpu);
int  exec_op(gb_cpu *cpu);
void dma_transfer(gb_cpu *cpu, BYTE val);
void set_lcd_status(gb_cpu *cpu);
void load_rom_bank(gb_cpu *cpu);
void load_ram_bank(gb_cpu *cpu);

/* GRAPHICS */
void update_graphics(gb_cpu *cpu, int ops);
void draw_scanline(gb_cpu *cpu);
void flip_screen(gb_cpu *cpu);
void clear_screen(gb_cpu *cpu, int color);
BYTE *get_tile(gb_cpu *cpu, BYTE id);
void draw_tile_row(const BYTE *data, BYTE *scr_pos, BYTE palette);
void draw_tile_at(gb_cpu *cpu, const BYTE *data, BYTE *scr_pos);
void draw_tiles(gb_cpu *cpu);

/* DEBUGGING */
void disassemble(const gb_cpu *cpu);
void cpu_status(const gb_cpu *cpu);

#endif /* LINUXBOY_CPU_H */

