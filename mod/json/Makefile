CC = gcc
CFLAGS = -Wall -Wextra -Werror -pedantic
SRC = $(wildcard src/*.c)
OBJ = $(SRC:.c=.o)
LIB = lib/mod.so

%.o: %.c
	$(CC) $(CFLAGS) -fPIC -g -c $< -o $@ -Iinclude

$(LIB): $(OBJ)
	$(CC) -fPIC $(OBJ) -o $(LIB) -shared -ljson
	fsimod compile

.PHONY: clean install
clean:
	@rm $(OBJ)
	@rm $(LIB)

install: $(LIB)
	@cp -r `pwd` ~/.local/lib/fsi

test: install
	@valgrind -s --leak-check=full --show-leak-kinds=all fsi -d test/test.fs
