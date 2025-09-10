CC = gcc
CFLAGS = -Wall -Wextra -Werror -pedantic
SRC = $(wildcard src/*.c)
OBJ = $(SRC:.c=.o)
LIB = lib/handler.so

%.o: %.c
	$(CC) $(CFLAGS) -fPIC -O3 -c $< -o $@ -Iinclude

$(LIB): $(OBJ)
	$(CC) -fPIC $(OBJ) -o $(LIB) -shared
.PHONY: clean
clean:
	@rm $(OBJ)
	@rm $(LIB)

