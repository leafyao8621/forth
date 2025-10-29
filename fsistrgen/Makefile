CC = gcc
SRC = $(wildcard src/*.c)
OBJ = $(SRC:.c=.o)
CFLAGS = -Wall -Wextra -Werror -pedantic
BIN = fsistrgen

%.o: %.c
	$(CC) $(CFLAGS) -O3 -c $< -o $@

$(BIN): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(BIN)

.PHONY: install clean
install: $(BIN)
	cp $(BIN) ~/.local/bin/
clean:
	@rm $(OBJ)
	@rm $(BIN)
