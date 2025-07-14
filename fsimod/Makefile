CC = gcc
CFLAGS = -Wall -Wextra -Werror -pedantic
SRC = $(wildcard src/*.c) $(wildcard src/*/*.c)
OBJ = $(SRC:.c=.o)
BIN = fsimod

%.o: %.c
	$(CC) $(CFLAGS) -g -c $< -o $@ -Iinclude

$(BIN): $(OBJ)
	$(CC) $(OBJ) -o $(BIN)

.PHONY: clean install
clean:
	@rm $(OBJ)
	@rm $(LIB)
	@rm $(BIN)
install: $(BIN)
	@cp $(BIN) ~/.local/bin
