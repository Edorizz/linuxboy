#ifndef LINUXBOY_CMD_H
#define LINUXBOY_CMD_H

#include "gameboy.h"

#define CMD_OPTIONS	7

typedef struct {
	char cmd_short;
	char *cmd_long;
	int (*fnc)(gameboy*, const char*);
} cmd_option;

int cmd_check(gameboy *gb, const char **argv);

/* COMMAND LINE OPTIONS */
int cmd_breakpoint(gameboy *gb, const char *arg);
int cmd_win_width(gameboy *gb, const char *arg);
int cmd_win_height(gameboy *gb, const char *arg);
int cmd_win_scale(gameboy *gb, const char *arg);
int cmd_add_watch(gameboy *gb, const char *arg);
int cmd_load_state(gameboy *gb, const char *arg);
int cmd_help(gameboy *gb, const char *arg);

#endif /* LINUXBOY_CMD_H */

