#ifndef LINUXBOY_TIMERS_H
#define LINUXBOY_TIMERS_H

/* TIMING */
#define DIVIDER_REGISTER	0xFF04
#define TIMA			0xFF05
#define TMA			0xFF06
#define TMC			0xFF07

void set_frequency(gb_cpu *cpu);
void divider_register(gb_cpu *cpu, int ops);
void update_timers(gb_cpu *cpu, int ops);

#endif /* LINUXBOY_TIMERS_H */
