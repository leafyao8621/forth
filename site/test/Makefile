CC = gcc
CFLAGS = -Wall -Wextra -Werror -pedantic
SRC = $(wildcard src/*.c)
OBJ = $(SRC:.c=.o)
LIB = lib/handler.so

%.o: %.c
	$(CC) $(CFLAGS) -fPIC -O3 -c $< -o $@

$(LIB): $(OBJ)
	$(CC) -fPIC $(OBJ) -o $(LIB) -shared -lfshps -lcontainers -lhttpserver -lhttputil -ljson -lfshp -lfsi -lm -lpthread -ldl
.PHONY: clean
clean:
	@rm $(OBJ)
	@rm $(LIB)

