#ifndef LINUXBOY_H
#define LINUXBOY_H

#define LO(word)		((word) & 0x00FF) /* *((BYTE*)&w) */
#define HI(word)		(((word) & 0xFF00) >> 8) /* *((BYTE*)&w + 1) */

#define BIT(b)			(1 << (b))
#define RESET_FLAGS(f, b)	((f) &= ~(b))

#define IMPLEMENT(s)		(fprintf(stderr, "Missing implementation: %s", (s)))

enum regs	{ REG_AF, REG_BC, REG_DE, REG_HL, REG_MAX };
enum flags	{ FLAG_C = 4, FLAG_H, FLAG_N, FLAG_Z };
enum rot_flags	{ RIGHT, LEFT, CIRCULAR };

typedef unsigned char	BYTE;
typedef signed char	SIGNED_BYTE;
typedef unsigned short	WORD;
typedef signed short	SIGNED_WORD;

typedef struct {
	char *assembly;
	int arg_size;
	void *func;
} op;

typedef union {
	WORD reg;
	struct {
		BYTE lo;
		BYTE hi;
	};
} reg;

typedef struct {
	BYTE memory[0x10000]; /* 64KB */
	BYTE rom[0x200000];   /* 2MB */
	reg regs[REG_MAX];
	WORD pc;
	reg *stack;
	int rom_size;
} gb_cpu;

int load_rom(gb_cpu *cpu, const char *rom_path);
int power(gb_cpu *cpu);
int exec_op(gb_cpu *cpu);

void disassemble(const gb_cpu *cpu);
void cpu_status(const gb_cpu *cpu);

/* OPCODES */
void op_0x00(gb_cpu *cpu);
void op_0x01(gb_cpu *cpu, WORD d16);
void op_0x02(gb_cpu *cpu);
void op_0x03(gb_cpu *cpu);
void op_0x04(gb_cpu *cpu);
void op_0x05(gb_cpu *cpu);
void op_0x06(gb_cpu *cpu, BYTE d8);
void op_0x07(gb_cpu *cpu);
void op_0x08(gb_cpu *cpu, WORD d16);
void op_0x09(gb_cpu *cpu);
void op_0x0A(gb_cpu *cpu);
void op_0x0B(gb_cpu *cpu);
void op_0x0C(gb_cpu *cpu);
void op_0x0D(gb_cpu *cpu);
void op_0x0E(gb_cpu *cpu, BYTE d8);
void op_0x0F(gb_cpu *cpu);

void op_0x10(gb_cpu *cpu, BYTE d8);
void op_0x11(gb_cpu *cpu, WORD d16);
void op_0x12(gb_cpu *cpu);
void op_0x13(gb_cpu *cpu);
void op_0x14(gb_cpu *cpu);
void op_0x15(gb_cpu *cpu);
void op_0x16(gb_cpu *cpu, BYTE d8);
void op_0x17(gb_cpu *cpu);
void op_0x18(gb_cpu *cpu, BYTE r8);
void op_0x19(gb_cpu *cpu);
void op_0x1A(gb_cpu *cpu);
void op_0x1B(gb_cpu *cpu);
void op_0x1C(gb_cpu *cpu);
void op_0x1D(gb_cpu *cpu);
void op_0x1E(gb_cpu *cpu, BYTE d8);
void op_0x1F(gb_cpu *cpu);

void op_0x20(gb_cpu *cpu, BYTE r8);
void op_0x21(gb_cpu *cpu, WORD d16);
void op_0x22(gb_cpu *cpu);
void op_0x23(gb_cpu *cpu);
void op_0x24(gb_cpu *cpu);
void op_0x25(gb_cpu *cpu);
void op_0x26(gb_cpu *cpu, BYTE d8);
void op_0x27(gb_cpu *cpu);
void op_0x28(gb_cpu *cpu, BYTE r8);
void op_0x29(gb_cpu *cpu);
void op_0x2A(gb_cpu *cpu);
void op_0x2B(gb_cpu *cpu);
void op_0x2C(gb_cpu *cpu);
void op_0x2D(gb_cpu *cpu);
void op_0x2E(gb_cpu *cpu, BYTE d8);
void op_0x2F(gb_cpu *cpu);

void op_0x30(gb_cpu *cpu, BYTE r8);
void op_0x31(gb_cpu *cpu, WORD d16);
void op_0x32(gb_cpu *cpu);
void op_0x33(gb_cpu *cpu);
void op_0x34(gb_cpu *cpu);
void op_0x35(gb_cpu *cpu);
void op_0x36(gb_cpu *cpu, BYTE d8);
void op_0x37(gb_cpu *cpu);
void op_0x38(gb_cpu *cpu, BYTE r8);
void op_0x39(gb_cpu *cpu);
void op_0x3A(gb_cpu *cpu);
void op_0x3B(gb_cpu *cpu);
void op_0x3C(gb_cpu *cpu);
void op_0x3D(gb_cpu *cpu);
void op_0x3E(gb_cpu *cpu, BYTE d8);
void op_0x3F(gb_cpu *cpu);

void op_0x40(gb_cpu *cpu);
void op_0x41(gb_cpu *cpu);
void op_0x42(gb_cpu *cpu);
void op_0x43(gb_cpu *cpu);
void op_0x44(gb_cpu *cpu);
void op_0x45(gb_cpu *cpu);
void op_0x46(gb_cpu *cpu);
void op_0x47(gb_cpu *cpu);
void op_0x48(gb_cpu *cpu);
void op_0x49(gb_cpu *cpu);
void op_0x4A(gb_cpu *cpu);
void op_0x4B(gb_cpu *cpu);
void op_0x4C(gb_cpu *cpu);
void op_0x4D(gb_cpu *cpu);
void op_0x4E(gb_cpu *cpu);
void op_0x4F(gb_cpu *cpu);

void op_0x50(gb_cpu *cpu);
void op_0x51(gb_cpu *cpu);
void op_0x52(gb_cpu *cpu);
void op_0x53(gb_cpu *cpu);
void op_0x54(gb_cpu *cpu);
void op_0x55(gb_cpu *cpu);
void op_0x56(gb_cpu *cpu);
void op_0x57(gb_cpu *cpu);
void op_0x58(gb_cpu *cpu);
void op_0x59(gb_cpu *cpu);
void op_0x5A(gb_cpu *cpu);
void op_0x5B(gb_cpu *cpu);
void op_0x5C(gb_cpu *cpu);
void op_0x5D(gb_cpu *cpu);
void op_0x5E(gb_cpu *cpu);
void op_0x5F(gb_cpu *cpu);

void op_0x60(gb_cpu *cpu);
void op_0x61(gb_cpu *cpu);
void op_0x62(gb_cpu *cpu);
void op_0x63(gb_cpu *cpu);
void op_0x64(gb_cpu *cpu);
void op_0x65(gb_cpu *cpu);
void op_0x66(gb_cpu *cpu);
void op_0x67(gb_cpu *cpu);
void op_0x68(gb_cpu *cpu);
void op_0x69(gb_cpu *cpu);
void op_0x6A(gb_cpu *cpu);
void op_0x6B(gb_cpu *cpu);
void op_0x6C(gb_cpu *cpu);
void op_0x6D(gb_cpu *cpu);
void op_0x6E(gb_cpu *cpu);
void op_0x6F(gb_cpu *cpu);

void op_0x70(gb_cpu *cpu);
void op_0x71(gb_cpu *cpu);
void op_0x72(gb_cpu *cpu);
void op_0x73(gb_cpu *cpu);
void op_0x74(gb_cpu *cpu);
void op_0x75(gb_cpu *cpu);
void op_0x76(gb_cpu *cpu);
void op_0x77(gb_cpu *cpu);
void op_0x78(gb_cpu *cpu);
void op_0x79(gb_cpu *cpu);
void op_0x7A(gb_cpu *cpu);
void op_0x7B(gb_cpu *cpu);
void op_0x7C(gb_cpu *cpu);
void op_0x7D(gb_cpu *cpu);
void op_0x7E(gb_cpu *cpu);
void op_0x7F(gb_cpu *cpu);

#endif /* LINUXBOY_H */
