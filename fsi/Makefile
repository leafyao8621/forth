CC = gcc
CFLAGS = -Wall -Wextra -Werror -pedantic
SRC = $(wildcard src/vm/*.c) $(wildcard src/vm/*/*.c) $(wildcard src/parser/*.c) $(wildcard src/parser/*/*.c) $(wildcard src/util/*.c)
OBJ = $(SRC:.c=.o)
LIB = lib/libfsi.so
BIN = fsi

%.o: %.c
	$(CC) $(CFLAGS) -fPIC -g -c $< -o $@ -Iinclude

$(LIB): $(OBJ)
	$(CC) -fPIC $(OBJ) -o $(LIB) -lm -lcontainers -shared

$(BIN): $(LIB)
	$(CC) -g src/main.c -o $(BIN) -lm -lcontainers -Llib -lfsi -Iinclude -Wl,-rpath,lib

.PHONY: clean valgrind
clean:
	@rm $(OBJ)
	@rm $(LIB)
	@rm $(BIN)
valgrind: $(BIN)
	@valgrind -s --leak-check=full ./$(BIN) -d script/a.fs
