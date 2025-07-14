#include <string.h>

#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "init.h"

const char *gitignore =
    "*.o\n"
    "*.so\n"
    "moddef\n"
    ".vscode\n";

ErrInit init(void) {
    if (mkdir("src", 0755)) {
        return ERR_INIT_MKDIR;
    }
    if (mkdir("lib", 0755)) {
        return ERR_INIT_MKDIR;
    }
    int fd = creat("lib/.gitkeep", 0755);
    if (!fd) {
        return ERR_INIT_CREATE;
    }
    close(fd);
    if (mkdir("script", 0755)) {
        return ERR_INIT_MKDIR;
    }
    fd = creat("script/.gitkeep", 0755);
    if (!fd) {
        return ERR_INIT_CREATE;
    }
    close(fd);
    fd = creat(".gitignore", 0755);
    ssize_t sz = write(fd, gitignore, strlen(gitignore));
    if (sz == -1) {
        close(fd);
        return ERR_INIT_WRITE;
    }
    close(fd);
    fd = creat("load_order.txt", 0755);
    if (!fd) {
        return ERR_INIT_CREATE;
    }
    close(fd);
    fd = creat("moddef.txt", 0755);
    if (!fd) {
        return ERR_INIT_CREATE;
    }
    close(fd);
    return ERR_INIT_OK;
}
