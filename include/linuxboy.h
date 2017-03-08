#ifndef LINUXBOY_H
#define LINUXBOY_H

typedef unsigned char BYTE;
typedef signed char SIGNED_BYTE;
typedef unsigned short WORD;
typedef signed short SIGNED_WORD;

typedef struct {
	char *assembly;
	int arg_size;
	void *func;
} opcode;

typedef struct {
	BYTE memory[0x10000]; /* 64KB */
	BYTE rom[0x200000];   /* 2MB */
	int rom_size;
} gb_cpu;

int load_rom(gb_cpu *cpu, const char *rom_path);
void disassemble(const gb_cpu *cpu);

#endif /* LINUXBOY_H */
