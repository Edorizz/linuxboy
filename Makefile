CC := gcc
C_FILES := $(wildcard src/*.c)
OBJ_FILES := $(addprefix obj/,$(notdir $(C_FILES:.c=.o)))
CFLAGS := -c -pedantic -std=c99 -Wall
LDFLAGS :=
RM := rm -f
NAME := linuxboy

$(NAME): $(OBJ_FILES)
	$(CC) $(LDFLAGS) -o $@ $^

obj/%.o: src/%.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	$(RM) obj/*.o $(NAME)

