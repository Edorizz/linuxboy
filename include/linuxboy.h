#ifndef LINUXBOY_H
#define LINUXBOY_H

/* OpenGL */
#include <GL/glew.h>
#include <glsl_shader.h>

/* SCREEN DIMENSIONS */
#define SCREEN_WIDTH		160
#define SCREEN_HEIGHT		144

/* PIXEL COLORS */
#define BLACK			0
#define DARK_GRAY		1
#define LIGHT_GRAY		2
#define WHITE			3
#define MAX_COLORS		4

/* INTERRUPTS REGISTERS */
#define IE			0xFFFF
#define IF			0xFF0F

/* TIMING */
#define DIVIDER_REGISTER	0xFF04
#define TIMA			0xFF05
#define TMA			0xFF06
#define TMC			0xFF07

/* LCD */
#define LCD_CONTROL		0xFF40
#define LCD_STATUS		0xFF41
#define CURR_SCANLINE		0xFF44
#define TARGET_SCANLINE		0xFF45

/* OTHER */
#define SIGNED			4
#define CLOCK_RATE		4194304

/* BYTE MANIPULATION */
#define LO(word)		((word) & 0x00FF)		/* *((BYTE*)&w) */
#define HI(word)		(((word) & 0xFF00) >> 8)	/* *((BYTE*)&w + 1) */

/* BIT MANIPULATION */
#define FLAG(cpu)		((cpu)->regs[REG_AF].lo)
#define FLAG_P(cpu)		(&(FLAG(cpu)))
#define BIT(b)			(1 << (b))
#define RESET_FLAGS(f, b)	((f) &= ~(b))

#define IMPLEMENT(s)		(fprintf(stderr, "Missing implementation: %s", (s)))

enum interrupts	{ VBLANK, LCD_STAT, TIMER, SERIAL, JOYPAD, INTERRUPT_MAX };
enum regs	{ REG_AF, REG_BC, REG_DE, REG_HL, REG_MAX };
enum cpu_flags	{ FLAG_C = 4, FLAG_H, FLAG_N, FLAG_Z };
enum emu_flags	{ BREAKPOINT, QUIT, DEBUG };
enum rot_flags	{ RIGHT, LEFT, CIRCULAR };

typedef unsigned char	BYTE;
typedef signed char	SIGNED_BYTE;
typedef unsigned short	WORD;
typedef signed short	SIGNED_WORD;

/* GRAPHICS */
typedef struct {
	/* OPENGL */
	GLuint shader_program;
	GLuint vao, vbo;
	GLuint texture;
	/* SCREEN DATA */
	int screen_width, screen_height;
	GLubyte *screen_data;
} gb_gpu;

typedef struct {
	GLuint r, g, b;
} color;

/* OPCODE */
typedef struct {
	char *assembly;
	int arg_size;
	void *func;
} op;

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
	BYTE memory[0x10000]; /* 64KB */
	BYTE rom[0x200000];   /* 2MB */
	reg regs[REG_MAX];
	WORD pc;
	reg *stack;
	int rom_size;
	BYTE ime; /* Interrupt master switch */
	/* TIMING */
	int divider_counter;
	int timer_counter;
	/* GRAPHICS */
	int scanline_counter;
	gb_gpu gpu;
	/* EMULATION */
	char *rom_path;
	BYTE flags;
	int breakpoint;
} gb_cpu;

/* CPU FUNCTIONS */
int load_rom(gb_cpu *cpu);
int power(gb_cpu *cpu);
int exec_op(gb_cpu *cpu);
void dma_transfer(gb_cpu *cpu, BYTE val);
void set_frequency(gb_cpu *cpu);
void divider_register(gb_cpu *cpu, int ops);
void request_interrupt(gb_cpu *cpu, BYTE interrupt);
void update_timers(gb_cpu *cpu, int ops);
void set_lcd_status(gb_cpu *cpu);
void update_graphics(gb_cpu *cpu, int ops);
void handle_interrupts(gb_cpu *cpu);

/* GPU FUNCTIONS */
void init_gpu(gb_gpu *gpu, int screen_width, int screen_height);
void free_gpu(gb_gpu *gpu);
void draw(gb_gpu *gpu);

/* DEBUGGING */
void disassemble(const gb_cpu *cpu);
void cpu_status(const gb_cpu *cpu);

/* OPCODE HELPERS */
BYTE inc_byte(BYTE *flag, BYTE b);
BYTE dec_byte(BYTE *flag, BYTE b);
void rot_byte(BYTE *flag, BYTE *b, BYTE rot_flags);
void add_byte(BYTE *flag, BYTE *b, int val);
void add_word(BYTE *flag, WORD *w, int val);
void sub_byte(BYTE *flag, BYTE *b, BYTE val);

void and_byte(BYTE *flag, BYTE *b, BYTE val);
void xor_byte(BYTE *flag, BYTE *b, BYTE val);
void or_byte(BYTE *flag, BYTE *b, BYTE val);
void cp_byte(BYTE *flag, BYTE b, BYTE val);

BYTE read_byte(gb_cpu *cpu, WORD addr);
WORD read_word(gb_cpu *cpu, WORD addr);
void write_byte(gb_cpu *cpu, WORD addr, BYTE val);
void write_word(gb_cpu *cpu, WORD addr, WORD val);

WORD pop(gb_cpu *cpu);
void push(gb_cpu *cpu, WORD val);

void call(gb_cpu *cpu, WORD addr);
void ret(gb_cpu *cpu);

/* OPCODES */
int op_0x00(gb_cpu *cpu);
int op_0x01(gb_cpu *cpu, WORD d16);
int op_0x02(gb_cpu *cpu);
int op_0x03(gb_cpu *cpu);
int op_0x04(gb_cpu *cpu);
int op_0x05(gb_cpu *cpu);
int op_0x06(gb_cpu *cpu, BYTE d8);
int op_0x07(gb_cpu *cpu);
int op_0x08(gb_cpu *cpu, WORD d16);
int op_0x09(gb_cpu *cpu);
int op_0x0A(gb_cpu *cpu);
int op_0x0B(gb_cpu *cpu);
int op_0x0C(gb_cpu *cpu);
int op_0x0D(gb_cpu *cpu);
int op_0x0E(gb_cpu *cpu, BYTE d8);
int op_0x0F(gb_cpu *cpu);

int op_0x10(gb_cpu *cpu, BYTE d8);
int op_0x11(gb_cpu *cpu, WORD d16);
int op_0x12(gb_cpu *cpu);
int op_0x13(gb_cpu *cpu);
int op_0x14(gb_cpu *cpu);
int op_0x15(gb_cpu *cpu);
int op_0x16(gb_cpu *cpu, BYTE d8);
int op_0x17(gb_cpu *cpu);
int op_0x18(gb_cpu *cpu, SIGNED_BYTE r8);
int op_0x19(gb_cpu *cpu);
int op_0x1A(gb_cpu *cpu);
int op_0x1B(gb_cpu *cpu);
int op_0x1C(gb_cpu *cpu);
int op_0x1D(gb_cpu *cpu);
int op_0x1E(gb_cpu *cpu, BYTE d8);
int op_0x1F(gb_cpu *cpu);

int op_0x20(gb_cpu *cpu, SIGNED_BYTE r8);
int op_0x21(gb_cpu *cpu, WORD d16);
int op_0x22(gb_cpu *cpu);
int op_0x23(gb_cpu *cpu);
int op_0x24(gb_cpu *cpu);
int op_0x25(gb_cpu *cpu);
int op_0x26(gb_cpu *cpu, BYTE d8);
int op_0x27(gb_cpu *cpu);
int op_0x28(gb_cpu *cpu, SIGNED_BYTE r8);
int op_0x29(gb_cpu *cpu);
int op_0x2A(gb_cpu *cpu);
int op_0x2B(gb_cpu *cpu);
int op_0x2C(gb_cpu *cpu);
int op_0x2D(gb_cpu *cpu);
int op_0x2E(gb_cpu *cpu, BYTE d8);
int op_0x2F(gb_cpu *cpu);

int op_0x30(gb_cpu *cpu, SIGNED_BYTE r8);
int op_0x31(gb_cpu *cpu, WORD d16);
int op_0x32(gb_cpu *cpu);
int op_0x33(gb_cpu *cpu);
int op_0x34(gb_cpu *cpu);
int op_0x35(gb_cpu *cpu);
int op_0x36(gb_cpu *cpu, BYTE d8);
int op_0x37(gb_cpu *cpu);
int op_0x38(gb_cpu *cpu, SIGNED_BYTE r8);
int op_0x39(gb_cpu *cpu);
int op_0x3A(gb_cpu *cpu);
int op_0x3B(gb_cpu *cpu);
int op_0x3C(gb_cpu *cpu);
int op_0x3D(gb_cpu *cpu);
int op_0x3E(gb_cpu *cpu, BYTE d8);
int op_0x3F(gb_cpu *cpu);

int op_0x40(gb_cpu *cpu);
int op_0x41(gb_cpu *cpu);
int op_0x42(gb_cpu *cpu);
int op_0x43(gb_cpu *cpu);
int op_0x44(gb_cpu *cpu);
int op_0x45(gb_cpu *cpu);
int op_0x46(gb_cpu *cpu);
int op_0x47(gb_cpu *cpu);
int op_0x48(gb_cpu *cpu);
int op_0x49(gb_cpu *cpu);
int op_0x4A(gb_cpu *cpu);
int op_0x4B(gb_cpu *cpu);
int op_0x4C(gb_cpu *cpu);
int op_0x4D(gb_cpu *cpu);
int op_0x4E(gb_cpu *cpu);
int op_0x4F(gb_cpu *cpu);

int op_0x50(gb_cpu *cpu);
int op_0x51(gb_cpu *cpu);
int op_0x52(gb_cpu *cpu);
int op_0x53(gb_cpu *cpu);
int op_0x54(gb_cpu *cpu);
int op_0x55(gb_cpu *cpu);
int op_0x56(gb_cpu *cpu);
int op_0x57(gb_cpu *cpu);
int op_0x58(gb_cpu *cpu);
int op_0x59(gb_cpu *cpu);
int op_0x5A(gb_cpu *cpu);
int op_0x5B(gb_cpu *cpu);
int op_0x5C(gb_cpu *cpu);
int op_0x5D(gb_cpu *cpu);
int op_0x5E(gb_cpu *cpu);
int op_0x5F(gb_cpu *cpu);

int op_0x60(gb_cpu *cpu);
int op_0x61(gb_cpu *cpu);
int op_0x62(gb_cpu *cpu);
int op_0x63(gb_cpu *cpu);
int op_0x64(gb_cpu *cpu);
int op_0x65(gb_cpu *cpu);
int op_0x66(gb_cpu *cpu);
int op_0x67(gb_cpu *cpu);
int op_0x68(gb_cpu *cpu);
int op_0x69(gb_cpu *cpu);
int op_0x6A(gb_cpu *cpu);
int op_0x6B(gb_cpu *cpu);
int op_0x6C(gb_cpu *cpu);
int op_0x6D(gb_cpu *cpu);
int op_0x6E(gb_cpu *cpu);
int op_0x6F(gb_cpu *cpu);

int op_0x70(gb_cpu *cpu);
int op_0x71(gb_cpu *cpu);
int op_0x72(gb_cpu *cpu);
int op_0x73(gb_cpu *cpu);
int op_0x74(gb_cpu *cpu);
int op_0x75(gb_cpu *cpu);
int op_0x76(gb_cpu *cpu);
int op_0x77(gb_cpu *cpu);
int op_0x78(gb_cpu *cpu);
int op_0x79(gb_cpu *cpu);
int op_0x7A(gb_cpu *cpu);
int op_0x7B(gb_cpu *cpu);
int op_0x7C(gb_cpu *cpu);
int op_0x7D(gb_cpu *cpu);
int op_0x7E(gb_cpu *cpu);
int op_0x7F(gb_cpu *cpu);

int op_0x80(gb_cpu *cpu);
int op_0x81(gb_cpu *cpu);
int op_0x82(gb_cpu *cpu);
int op_0x83(gb_cpu *cpu);
int op_0x84(gb_cpu *cpu);
int op_0x85(gb_cpu *cpu);
int op_0x86(gb_cpu *cpu);
int op_0x87(gb_cpu *cpu);
int op_0x88(gb_cpu *cpu);
int op_0x89(gb_cpu *cpu);
int op_0x8A(gb_cpu *cpu);
int op_0x8B(gb_cpu *cpu);
int op_0x8C(gb_cpu *cpu);
int op_0x8D(gb_cpu *cpu);
int op_0x8E(gb_cpu *cpu);
int op_0x8F(gb_cpu *cpu);

int op_0x90(gb_cpu *cpu);
int op_0x91(gb_cpu *cpu);
int op_0x92(gb_cpu *cpu);
int op_0x93(gb_cpu *cpu);
int op_0x94(gb_cpu *cpu);
int op_0x95(gb_cpu *cpu);
int op_0x96(gb_cpu *cpu);
int op_0x97(gb_cpu *cpu);
int op_0x98(gb_cpu *cpu);
int op_0x99(gb_cpu *cpu);
int op_0x9A(gb_cpu *cpu);
int op_0x9B(gb_cpu *cpu);
int op_0x9C(gb_cpu *cpu);
int op_0x9D(gb_cpu *cpu);
int op_0x9E(gb_cpu *cpu);
int op_0x9F(gb_cpu *cpu);

int op_0xA0(gb_cpu *cpu);
int op_0xA1(gb_cpu *cpu);
int op_0xA2(gb_cpu *cpu);
int op_0xA3(gb_cpu *cpu);
int op_0xA4(gb_cpu *cpu);
int op_0xA5(gb_cpu *cpu);
int op_0xA6(gb_cpu *cpu);
int op_0xA7(gb_cpu *cpu);
int op_0xA8(gb_cpu *cpu);
int op_0xA9(gb_cpu *cpu);
int op_0xAA(gb_cpu *cpu);
int op_0xAB(gb_cpu *cpu);
int op_0xAC(gb_cpu *cpu);
int op_0xAD(gb_cpu *cpu);
int op_0xAE(gb_cpu *cpu);
int op_0xAF(gb_cpu *cpu);

int op_0xB0(gb_cpu *cpu);
int op_0xB1(gb_cpu *cpu);
int op_0xB2(gb_cpu *cpu);
int op_0xB3(gb_cpu *cpu);
int op_0xB4(gb_cpu *cpu);
int op_0xB5(gb_cpu *cpu);
int op_0xB6(gb_cpu *cpu);
int op_0xB7(gb_cpu *cpu);
int op_0xB8(gb_cpu *cpu);
int op_0xB9(gb_cpu *cpu);
int op_0xBA(gb_cpu *cpu);
int op_0xBB(gb_cpu *cpu);
int op_0xBC(gb_cpu *cpu);
int op_0xBD(gb_cpu *cpu);
int op_0xBE(gb_cpu *cpu);
int op_0xBF(gb_cpu *cpu);

int op_0xC0(gb_cpu *cpu);
int op_0xC1(gb_cpu *cpu);
int op_0xC2(gb_cpu *cpu, WORD a16);
int op_0xC3(gb_cpu *cpu, WORD a16);
int op_0xC4(gb_cpu *cpu, WORD a16);
int op_0xC5(gb_cpu *cpu);
int op_0xC6(gb_cpu *cpu, BYTE d8);
int op_0xC7(gb_cpu *cpu);
int op_0xC8(gb_cpu *cpu);
int op_0xC9(gb_cpu *cpu);
int op_0xCA(gb_cpu *cpu, WORD a16);
int op_0xCB(gb_cpu *cpu);
int op_0xCC(gb_cpu *cpu, WORD a16);
int op_0xCD(gb_cpu *cpu, WORD a16);
int op_0xCE(gb_cpu *cpu, BYTE d8);
int op_0xCF(gb_cpu *cpu);

int op_0xD0(gb_cpu *cpu);
int op_0xD1(gb_cpu *cpu);
int op_0xD2(gb_cpu *cpu, WORD a16);
int op_0xD4(gb_cpu *cpu, WORD a16);
int op_0xD5(gb_cpu *cpu);
int op_0xD6(gb_cpu *cpu, BYTE d8);
int op_0xD7(gb_cpu *cpu);
int op_0xD8(gb_cpu *cpu);
int op_0xD9(gb_cpu *cpu);
int op_0xDA(gb_cpu *cpu, WORD a16);
int op_0xDC(gb_cpu *cpu, WORD a16);
int op_0xDE(gb_cpu *cpu, BYTE d8);
int op_0xDF(gb_cpu *cpu);

int op_0xE0(gb_cpu *cpu, BYTE a8);
int op_0xE1(gb_cpu *cpu);
int op_0xE2(gb_cpu *cpu);
int op_0xE5(gb_cpu *cpu);
int op_0xE6(gb_cpu *cpu, BYTE d8);
int op_0xE7(gb_cpu *cpu);
int op_0xE8(gb_cpu *cpu, SIGNED_BYTE r8);
int op_0xE9(gb_cpu *cpu);
int op_0xEA(gb_cpu *cpu, WORD a16);
int op_0xEE(gb_cpu *cpu, BYTE d8);
int op_0xEF(gb_cpu *cpu);

int op_0xF0(gb_cpu *cpu, BYTE a8);
int op_0xF1(gb_cpu *cpu);
int op_0xF2(gb_cpu *cpu);
int op_0xF3(gb_cpu *cpu);
int op_0xF5(gb_cpu *cpu);
int op_0xF6(gb_cpu *cpu, BYTE d8);
int op_0xF7(gb_cpu *cpu);
int op_0xF8(gb_cpu *cpu, BYTE r8);
int op_0xF9(gb_cpu *cpu);
int op_0xFA(gb_cpu *cpu, WORD a16);
int op_0xFB(gb_cpu *cpu);
int op_0xFE(gb_cpu *cpu, BYTE d8);
int op_0xFF(gb_cpu *cpu);

/* EXTENDED OPCODES */
int ext_op_0x00(gb_cpu *cpu);
int ext_op_0x01(gb_cpu *cpu);
int ext_op_0x02(gb_cpu *cpu);
int ext_op_0x03(gb_cpu *cpu);
int ext_op_0x04(gb_cpu *cpu);
int ext_op_0x05(gb_cpu *cpu);
int ext_op_0x06(gb_cpu *cpu);
int ext_op_0x07(gb_cpu *cpu);
int ext_op_0x08(gb_cpu *cpu);
int ext_op_0x09(gb_cpu *cpu);
int ext_op_0x0A(gb_cpu *cpu);
int ext_op_0x0B(gb_cpu *cpu);
int ext_op_0x0C(gb_cpu *cpu);
int ext_op_0x0D(gb_cpu *cpu);
int ext_op_0x0E(gb_cpu *cpu);
int ext_op_0x0F(gb_cpu *cpu);

int ext_op_0x10(gb_cpu *cpu);
int ext_op_0x11(gb_cpu *cpu);
int ext_op_0x12(gb_cpu *cpu);
int ext_op_0x13(gb_cpu *cpu);
int ext_op_0x14(gb_cpu *cpu);
int ext_op_0x15(gb_cpu *cpu);
int ext_op_0x16(gb_cpu *cpu);
int ext_op_0x17(gb_cpu *cpu);
int ext_op_0x18(gb_cpu *cpu);
int ext_op_0x19(gb_cpu *cpu);
int ext_op_0x1A(gb_cpu *cpu);
int ext_op_0x1B(gb_cpu *cpu);
int ext_op_0x1C(gb_cpu *cpu);
int ext_op_0x1D(gb_cpu *cpu);
int ext_op_0x1E(gb_cpu *cpu);
int ext_op_0x1F(gb_cpu *cpu);

int ext_op_0x20(gb_cpu *cpu);
int ext_op_0x21(gb_cpu *cpu);
int ext_op_0x22(gb_cpu *cpu);
int ext_op_0x23(gb_cpu *cpu);
int ext_op_0x24(gb_cpu *cpu);
int ext_op_0x25(gb_cpu *cpu);
int ext_op_0x26(gb_cpu *cpu);
int ext_op_0x27(gb_cpu *cpu);
int ext_op_0x28(gb_cpu *cpu);
int ext_op_0x29(gb_cpu *cpu);
int ext_op_0x2A(gb_cpu *cpu);
int ext_op_0x2B(gb_cpu *cpu);
int ext_op_0x2C(gb_cpu *cpu);
int ext_op_0x2D(gb_cpu *cpu);
int ext_op_0x2E(gb_cpu *cpu);
int ext_op_0x2F(gb_cpu *cpu);

int ext_op_0x30(gb_cpu *cpu);
int ext_op_0x31(gb_cpu *cpu);
int ext_op_0x32(gb_cpu *cpu);
int ext_op_0x33(gb_cpu *cpu);
int ext_op_0x34(gb_cpu *cpu);
int ext_op_0x35(gb_cpu *cpu);
int ext_op_0x36(gb_cpu *cpu);
int ext_op_0x37(gb_cpu *cpu);
int ext_op_0x38(gb_cpu *cpu);
int ext_op_0x39(gb_cpu *cpu);
int ext_op_0x3A(gb_cpu *cpu);
int ext_op_0x3B(gb_cpu *cpu);
int ext_op_0x3C(gb_cpu *cpu);
int ext_op_0x3D(gb_cpu *cpu);
int ext_op_0x3E(gb_cpu *cpu);
int ext_op_0x3F(gb_cpu *cpu);

int ext_op_0x40(gb_cpu *cpu);
int ext_op_0x41(gb_cpu *cpu);
int ext_op_0x42(gb_cpu *cpu);
int ext_op_0x43(gb_cpu *cpu);
int ext_op_0x44(gb_cpu *cpu);
int ext_op_0x45(gb_cpu *cpu);
int ext_op_0x46(gb_cpu *cpu);
int ext_op_0x47(gb_cpu *cpu);
int ext_op_0x48(gb_cpu *cpu);
int ext_op_0x49(gb_cpu *cpu);
int ext_op_0x4A(gb_cpu *cpu);
int ext_op_0x4B(gb_cpu *cpu);
int ext_op_0x4C(gb_cpu *cpu);
int ext_op_0x4D(gb_cpu *cpu);
int ext_op_0x4E(gb_cpu *cpu);
int ext_op_0x4F(gb_cpu *cpu);

int ext_op_0x50(gb_cpu *cpu);
int ext_op_0x51(gb_cpu *cpu);
int ext_op_0x52(gb_cpu *cpu);
int ext_op_0x53(gb_cpu *cpu);
int ext_op_0x54(gb_cpu *cpu);
int ext_op_0x55(gb_cpu *cpu);
int ext_op_0x56(gb_cpu *cpu);
int ext_op_0x57(gb_cpu *cpu);
int ext_op_0x58(gb_cpu *cpu);
int ext_op_0x59(gb_cpu *cpu);
int ext_op_0x5A(gb_cpu *cpu);
int ext_op_0x5B(gb_cpu *cpu);
int ext_op_0x5C(gb_cpu *cpu);
int ext_op_0x5D(gb_cpu *cpu);
int ext_op_0x5E(gb_cpu *cpu);
int ext_op_0x5F(gb_cpu *cpu);

int ext_op_0x60(gb_cpu *cpu);
int ext_op_0x61(gb_cpu *cpu);
int ext_op_0x62(gb_cpu *cpu);
int ext_op_0x63(gb_cpu *cpu);
int ext_op_0x64(gb_cpu *cpu);
int ext_op_0x65(gb_cpu *cpu);
int ext_op_0x66(gb_cpu *cpu);
int ext_op_0x67(gb_cpu *cpu);
int ext_op_0x68(gb_cpu *cpu);
int ext_op_0x69(gb_cpu *cpu);
int ext_op_0x6A(gb_cpu *cpu);
int ext_op_0x6B(gb_cpu *cpu);
int ext_op_0x6C(gb_cpu *cpu);
int ext_op_0x6D(gb_cpu *cpu);
int ext_op_0x6E(gb_cpu *cpu);
int ext_op_0x6F(gb_cpu *cpu);

int ext_op_0x70(gb_cpu *cpu);
int ext_op_0x71(gb_cpu *cpu);
int ext_op_0x72(gb_cpu *cpu);
int ext_op_0x73(gb_cpu *cpu);
int ext_op_0x74(gb_cpu *cpu);
int ext_op_0x75(gb_cpu *cpu);
int ext_op_0x76(gb_cpu *cpu);
int ext_op_0x77(gb_cpu *cpu);
int ext_op_0x78(gb_cpu *cpu);
int ext_op_0x79(gb_cpu *cpu);
int ext_op_0x7A(gb_cpu *cpu);
int ext_op_0x7B(gb_cpu *cpu);
int ext_op_0x7C(gb_cpu *cpu);
int ext_op_0x7D(gb_cpu *cpu);
int ext_op_0x7E(gb_cpu *cpu);
int ext_op_0x7F(gb_cpu *cpu);

int ext_op_0x80(gb_cpu *cpu);
int ext_op_0x81(gb_cpu *cpu);
int ext_op_0x82(gb_cpu *cpu);
int ext_op_0x83(gb_cpu *cpu);
int ext_op_0x84(gb_cpu *cpu);
int ext_op_0x85(gb_cpu *cpu);
int ext_op_0x86(gb_cpu *cpu);
int ext_op_0x87(gb_cpu *cpu);
int ext_op_0x88(gb_cpu *cpu);
int ext_op_0x89(gb_cpu *cpu);
int ext_op_0x8A(gb_cpu *cpu);
int ext_op_0x8B(gb_cpu *cpu);
int ext_op_0x8C(gb_cpu *cpu);
int ext_op_0x8D(gb_cpu *cpu);
int ext_op_0x8E(gb_cpu *cpu);
int ext_op_0x8F(gb_cpu *cpu);

int ext_op_0x90(gb_cpu *cpu);
int ext_op_0x91(gb_cpu *cpu);
int ext_op_0x92(gb_cpu *cpu);
int ext_op_0x93(gb_cpu *cpu);
int ext_op_0x94(gb_cpu *cpu);
int ext_op_0x95(gb_cpu *cpu);
int ext_op_0x96(gb_cpu *cpu);
int ext_op_0x97(gb_cpu *cpu);
int ext_op_0x98(gb_cpu *cpu);
int ext_op_0x99(gb_cpu *cpu);
int ext_op_0x9A(gb_cpu *cpu);
int ext_op_0x9B(gb_cpu *cpu);
int ext_op_0x9C(gb_cpu *cpu);
int ext_op_0x9D(gb_cpu *cpu);
int ext_op_0x9E(gb_cpu *cpu);
int ext_op_0x9F(gb_cpu *cpu);

int ext_op_0xA0(gb_cpu *cpu);
int ext_op_0xA1(gb_cpu *cpu);
int ext_op_0xA2(gb_cpu *cpu);
int ext_op_0xA3(gb_cpu *cpu);
int ext_op_0xA4(gb_cpu *cpu);
int ext_op_0xA5(gb_cpu *cpu);
int ext_op_0xA6(gb_cpu *cpu);
int ext_op_0xA7(gb_cpu *cpu);
int ext_op_0xA8(gb_cpu *cpu);
int ext_op_0xA9(gb_cpu *cpu);
int ext_op_0xAA(gb_cpu *cpu);
int ext_op_0xAB(gb_cpu *cpu);
int ext_op_0xAC(gb_cpu *cpu);
int ext_op_0xAD(gb_cpu *cpu);
int ext_op_0xAE(gb_cpu *cpu);
int ext_op_0xAF(gb_cpu *cpu);

int ext_op_0xB0(gb_cpu *cpu);
int ext_op_0xB1(gb_cpu *cpu);
int ext_op_0xB2(gb_cpu *cpu);
int ext_op_0xB3(gb_cpu *cpu);
int ext_op_0xB4(gb_cpu *cpu);
int ext_op_0xB5(gb_cpu *cpu);
int ext_op_0xB6(gb_cpu *cpu);
int ext_op_0xB7(gb_cpu *cpu);
int ext_op_0xB8(gb_cpu *cpu);
int ext_op_0xB9(gb_cpu *cpu);
int ext_op_0xBA(gb_cpu *cpu);
int ext_op_0xBB(gb_cpu *cpu);
int ext_op_0xBC(gb_cpu *cpu);
int ext_op_0xBD(gb_cpu *cpu);
int ext_op_0xBE(gb_cpu *cpu);
int ext_op_0xBF(gb_cpu *cpu);

int ext_op_0xC0(gb_cpu *cpu);
int ext_op_0xC0(gb_cpu *cpu);
int ext_op_0xC1(gb_cpu *cpu);
int ext_op_0xC2(gb_cpu *cpu);
int ext_op_0xC3(gb_cpu *cpu);
int ext_op_0xC4(gb_cpu *cpu);
int ext_op_0xC5(gb_cpu *cpu);
int ext_op_0xC6(gb_cpu *cpu);
int ext_op_0xC7(gb_cpu *cpu);
int ext_op_0xC8(gb_cpu *cpu);
int ext_op_0xC9(gb_cpu *cpu);
int ext_op_0xCA(gb_cpu *cpu);
int ext_op_0xCB(gb_cpu *cpu);
int ext_op_0xCC(gb_cpu *cpu);
int ext_op_0xCD(gb_cpu *cpu);
int ext_op_0xCE(gb_cpu *cpu);
int ext_op_0xCF(gb_cpu *cpu);

int ext_op_0xD0(gb_cpu *cpu);
int ext_op_0xD1(gb_cpu *cpu);
int ext_op_0xD2(gb_cpu *cpu);
int ext_op_0xD3(gb_cpu *cpu);
int ext_op_0xD4(gb_cpu *cpu);
int ext_op_0xD5(gb_cpu *cpu);
int ext_op_0xD6(gb_cpu *cpu);
int ext_op_0xD7(gb_cpu *cpu);
int ext_op_0xD8(gb_cpu *cpu);
int ext_op_0xD9(gb_cpu *cpu);
int ext_op_0xDA(gb_cpu *cpu);
int ext_op_0xDB(gb_cpu *cpu);
int ext_op_0xDC(gb_cpu *cpu);
int ext_op_0xDD(gb_cpu *cpu);
int ext_op_0xDE(gb_cpu *cpu);
int ext_op_0xDF(gb_cpu *cpu);

int ext_op_0xE0(gb_cpu *cpu);
int ext_op_0xE1(gb_cpu *cpu);
int ext_op_0xE2(gb_cpu *cpu);
int ext_op_0xE3(gb_cpu *cpu);
int ext_op_0xE4(gb_cpu *cpu);
int ext_op_0xE5(gb_cpu *cpu);
int ext_op_0xE6(gb_cpu *cpu);
int ext_op_0xE7(gb_cpu *cpu);
int ext_op_0xE8(gb_cpu *cpu);
int ext_op_0xE9(gb_cpu *cpu);
int ext_op_0xEA(gb_cpu *cpu);
int ext_op_0xEB(gb_cpu *cpu);
int ext_op_0xEC(gb_cpu *cpu);
int ext_op_0xED(gb_cpu *cpu);
int ext_op_0xEE(gb_cpu *cpu);
int ext_op_0xEF(gb_cpu *cpu);

int ext_op_0xF0(gb_cpu *cpu);
int ext_op_0xF1(gb_cpu *cpu);
int ext_op_0xF2(gb_cpu *cpu);
int ext_op_0xF3(gb_cpu *cpu);
int ext_op_0xF4(gb_cpu *cpu);
int ext_op_0xF5(gb_cpu *cpu);
int ext_op_0xF6(gb_cpu *cpu);
int ext_op_0xF7(gb_cpu *cpu);
int ext_op_0xF8(gb_cpu *cpu);
int ext_op_0xF9(gb_cpu *cpu);
int ext_op_0xFA(gb_cpu *cpu);
int ext_op_0xFB(gb_cpu *cpu);
int ext_op_0xFC(gb_cpu *cpu);
int ext_op_0xFD(gb_cpu *cpu);
int ext_op_0xFE(gb_cpu *cpu);
int ext_op_0xFF(gb_cpu *cpu);

#endif /* LINUXBOY_H */

