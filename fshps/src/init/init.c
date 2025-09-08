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

const char *gitignore = "route\n";

const char *route =
    "URL\n"
    "/ \n"
    "GET\n"
    "src/index.fshp\n";

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
}
