#ifndef LINUXBOY_CMD_H
#define LINUXBOY_CMD_H

#include <linuxboy/gameboy.h>

#define CMD_OPTIONS	6

typedef struct {
	char cmd_short;
	char *cmd_long;
	void (*fnc)(gameboy*, const char*);
} cmd_option;

int cmd_check(gameboy *gb, const char **argv);

/* COMMAND LINE OPTIONS */
void cmd_breakpoint(gameboy *gb, const char *arg);
void cmd_win_width(gameboy *gb, const char *arg);
void cmd_win_height(gameboy *gb, const char *arg);
void cmd_win_scale(gameboy *gb, const char *arg);
void cmd_add_watch(gameboy *gb, const char *arg);
void cmd_help(gameboy *gb, const char *arg);

#endif /* LINUXBOY_CMD_H */

