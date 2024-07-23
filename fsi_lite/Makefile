CC = gcc
CFLAGS = -Wall -Wextra -Werror -pedantic -ansi
SRC = $(wildcard src/*.c) $(wildcard src/vm/*.c) $(wildcard src/vm/*/*.c) $(wildcard src/parser/*.c) $(wildcard src/parser/*/*.c) $(wildcard src/util/*.c)
OBJ = $(SRC:.c=.o)
BIN = fsi

%.o: %.c
	$(CC) $(CFLAGS) -g -c $< -o $@ -Iinclude

$(BIN): $(OBJ)
	$(CC) -g $(OBJ) -o $(BIN)

.PHONY: clean valgrind
clean:
	@rm $(OBJ)
	@rm $(BIN)
valgrind: $(BIN)
	@valgrind -s --leak-check=full --show-leak-kinds=all ./$(BIN) -d script/a.fs
