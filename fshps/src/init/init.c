#include <string.h>

#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "init.h"

const char *index_fshp =
    "<!DOCTYPE html>\n"
    "<html>\n"
    "    <head><head>\n"
    "    <body>\n"
    "        <h1><?fshp : main .\" FSHP\" ; main ?></h1>\n"
    "    </body>\n"
    "</html>\n";

const char *gitignore = "route\n*.c\n*.o\n*.so\n";

const char *route =
    "KEY\n"
    "index\n"
    "URL\n"
    "/ \n"
    "GET\n"
    "src/index.fshp\n";

const char *makefile =
    "CC = gcc\n"
    "CFLAGS = -Wall -Wextra -Werror -pedantic\n"
    "SRC = $(wildcard src/*.c)\n"
    "OBJ = $(SRC:.c=.o)\n"
    "LIB = lib/handler.so\n\n"
    "%.o: %.c\n"
    "\t$(CC) $(CFLAGS) -fPIC -O3 -c $< -o $@\n\n"
    "$(LIB): $(OBJ)\n"
    "\t$(CC) -fPIC $(OBJ) -o $(LIB) -shared\n"
    ".PHONY: clean\n"
    "clean:\n"
    "\t@rm $(OBJ)\n"
    "\t@rm $(LIB)\n\n";

ErrInit init(void) {
    if (mkdir("script", 0755)) {
        return ERR_INIT_MKDIR;
    }
    int fd = creat("script/index.fshp", 0755);
    ssize_t sz = write(fd, index_fshp, strlen(index_fshp));
    if (sz == -1) {
        close(fd);
        return ERR_INIT_WRITE;
    }
    close(fd);
    fd = creat(".gitignore", 0755);
    sz = write(fd, gitignore, strlen(gitignore));
    if (sz == -1) {
        close(fd);
        return ERR_INIT_WRITE;
    }
    close(fd);
    fd = creat("route.txt", 0755);
    if (!fd) {
        return ERR_INIT_CREATE;
    }
    sz = write(fd, route, strlen(route));
    if (sz == -1) {
        close(fd);
        return ERR_INIT_WRITE;
    }
    close(fd);
    if (mkdir("src", 0755)) {
        return ERR_INIT_MKDIR;
    }
    fd = creat("src/.gitkeep", 0755);
    if (!fd) {
        return ERR_INIT_CREATE;
    }
    close(fd);
    if (mkdir("lib", 0755)) {
        return ERR_INIT_MKDIR;
    }
    fd = creat("lib/.gitkeep", 0755);
    if (!fd) {
        return ERR_INIT_CREATE;
    }
    close(fd);
    fd = creat("Makefile", 0755);
    if (!fd) {
        return ERR_INIT_CREATE;
    }
    sz = write(fd, makefile, strlen(makefile));
    if (sz == -1) {
        close(fd);
        return ERR_INIT_WRITE;
    }
    close(fd);
    return ERR_INIT_OK;
}
