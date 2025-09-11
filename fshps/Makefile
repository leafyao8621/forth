CC = gcc
SRC = $(wildcard src/*/*.c)
CFLAGS = -Wall -Wextra -Werror -pedantic
OBJ = $(SRC:.c=.o)
LIB = lib/libfshps.so
BIN = fshps

%.o: %.c
	$(CC) $(CFLAGS) -fPIC -O3 -c $< -o $@ -Iinclude

$(LIB): $(OBJ)
	$(CC) -fPIC $(OBJ) -o $(LIB) -shared -lcontainers -lhttpserver -lhttputil -ljson -lfshp -lfsi -lm -lpthread -ldl

$(BIN): $(LIB)
	$(CC) $(CFLAGS) -O3 src/main.c -o $(BIN) -lfshps -lcontainers -lhttpserver -lhttputil -ljson -lfshp -lfsi -lm -lpthread -ldl -Wl,-rpath,lib

.PHONY: clean install
clean:
	@rm $(OBJ)
	@rm $(LIB)
	@rm $(BIN)
install: $(BIN)
	@cp $(BIN) ~/.local/bin
	@cp -r include/fshps ~/.local/include
	@cp -r $(LIB) ~/.local/lib
