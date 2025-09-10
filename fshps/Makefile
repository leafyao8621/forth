SRC = $(wildcard src/*.c) $(wildcard src/*/*.c)
OBJ = $(SRC:.c=.o)
BIN = fshps

%.o: %.c
	$(CC) $(CFLAGS) -g -c $< -o $@ -Iinclude

$(BIN): $(OBJ)
	$(CC) $(OBJ) -o $(BIN) -lcontainers -lhttpserver -lhttputil -lfshp -lfsi -lm -lpthread

.PHONY: clean install
clean:
	@rm $(OBJ)
	@rm $(BIN)
install: $(BIN)
	@cp $(BIN) ~/.local/bin
	@cp -r include/fshps ~/.local/include
