CC = gcc
CFLAGS = -Wall -Wextra -Werror -pedantic
SRC = $(wildcard src/*.c) $(wildcard src/*/*.c)
OBJ = $(SRC:.c=.o)
BIN = fsimod

%.o: %.c
	$(CC) $(CFLAGS) -O3 -c $< -o $@ -Iinclude

$(BIN): $(OBJ)
	$(CC) $(OBJ) -o $(BIN) -lcontainers

.PHONY: clean install
clean:
	@rm $(OBJ)
	@rm $(BIN)
install: $(BIN)
	@cp $(BIN) ~/.local/bin
