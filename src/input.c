/* Header file */
#include <linuxboy/input.h>
/* Linuxboy */
#include <linuxboy/utils.h>

void
key_event(gb_cpu *cpu, int key, int event)
{
	if (event == PRESS)
		cpu->joypad &= ~BIT(key);
	else if (event == RELEASE)
		cpu->joypad |= BIT(key);
}

