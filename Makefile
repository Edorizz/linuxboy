CC := gcc
C_FILES := $(wildcard src/*.c)
OBJ_FILES := $(addprefix obj/,$(notdir $(C_FILES:.c=.o)))
CFLAGS := -c -std=c99 -Wall
LDFLAGS := -lSDL2 -lGLEW -lGLU -lGL
RM := rm -f
NAME := lboy

$(NAME): $(OBJ_FILES)
	$(CC) $(LDFLAGS) -O3 -fomit-frame-pointer -o $@ $^

obj/%.o: src/%.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	$(RM) obj/*.o $(NAME)

