CC = gcc
CFLAGS = -Wall -Wextra -Werror -pedantic
SRC = $(wildcard src/util/*.c)
OBJ = $(SRC:.c=.o)
LIB = lib/libfshp.so
BIN = fshp

%.o: %.c
	$(CC) $(CFLAGS) -fPIC -O3 -c $< -o $@ -Iinclude

$(LIB): $(OBJ)
	$(CC) -fPIC $(OBJ) -o $(LIB) -lm -lcontainers -shared

$(BIN): $(LIB)
	$(CC) -g src/main.c -o $(BIN) -lm -ldl -lcontainers -Llib -lfsi -lfshp -Iinclude -Wl,-rpath,lib

.PHONY: clean valgrind install
clean:
	@rm $(OBJ)
	@rm $(LIB)
	@rm $(BIN)
valgrind: $(BIN)
	@valgrind -s --leak-check=full --show-leak-kinds=all ./$(BIN) script/a.fshp
install: $(BIN)
	@cp $(BIN) ~/.local/bin
	@cp -r include/fshp ~/.local/include
	@cp -r $(LIB) ~/.local/lib
