#ifndef LINUXBOY_H
#define LINUXBOY_H

#define LO(word)	((word) & 0x00FF) /* *((BYTE*)&w) */
#define HI(word)	(((word) & 0xFF00) >> 8) /* *((BYTE*)&w + 1) */

enum regs { REG_AF, REG_BC, REG_DE, REG_HL, REG_MAX };
enum flags { FLAG_C = 4, FLAG_H, FLAG_N, FLAG_Z };

typedef unsigned char	BYTE;
typedef signed char	SIGNED_BYTE;
typedef unsigned short	WORD;
typedef signed short	SIGNED_WORD;

typedef struct {
	char *assembly;
	int arg_size;
	void *func;
} op;

typedef struct {
	BYTE memory[0x10000]; /* 64KB */
	BYTE rom[0x200000];   /* 2MB */
	WORD regs[REG_MAX];
	WORD pc;
	WORD *stack;
	int rom_size;
} gb_cpu;

int load_rom(gb_cpu *cpu, const char *rom_path);
int power(gb_cpu *cpu);
int exec_op(gb_cpu *cpu);

void disassemble(const gb_cpu *cpu);
void cpu_status(const gb_cpu *cpu);

/* OPCODES */
void nop(gb_cpu *cpu);
void ld_bc_d16(gb_cpu *cpu, WORD d16);
void ld_bc_a(gb_cpu *cpu);
void inc_bc(gb_cpu *cpu);
void inc_b(gb_cpu *cpu);
void dec_b(gb_cpu *cpu);
void ld_b_d8(gb_cpu *cpu);
void rlca(gb_cpu *cpu);
void ld_a16_sp(gb_cpu *cpu);
void add_hl_bc(gb_cpu *cpu);
void ld_a_bc(gb_cpu *cpu);
void dec_bc(gb_cpu *cpu);
void inc_c(gb_cpu *cpu);
void dec_c(gb_cpu *cpu);
void ld_c_d8(gb_cpu *cpu);
void rrca(gb_cpu *cpu);

#endif /* LINUXBOY_H */
