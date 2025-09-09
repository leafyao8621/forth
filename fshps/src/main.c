#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "init/init.h"
#include "compile/compile.h"
#include "serve/serve.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        puts("Usage: fshps <action> {init, compile, serve} <port>");
        return 1;
    }
    if (!strcmp(argv[1], "init")) {
        ErrInit err = init();
        if (err) {
            switch (err) {
            case ERR_INIT_MKDIR:
                puts("MKDIR");
                return 1;
            case ERR_INIT_CREATE:
                puts("CREATE");
                return 1;
            case ERR_INIT_WRITE:
                puts("WRITE");
                return 1;
            default:
                break;
            }
        }
    } else if (!strcmp(argv[1], "compile")) {
        ErrCompile err = compile();
        if (err) {
            switch (err) {
            case ERR_COMPILE_OPEN:
                puts("OPEN");
                return 1;
            case ERR_COMPILE_OUT_OF_MEMORY:
                puts("OUT OF MEMORY");
                return 1;
            case ERR_COMPILE_INVALID_HEADER:
                puts("INVALID HEADER");
                return 1;
            default:
                break;
            }
        }
    } else if (!strcmp(argv[1], "serve")) {
        serve(atoi(argv[2]));
    } else {
        puts("Usage: fshps <action> {init, compile, serve} <ip>");
        return 1;
    }
    return 0;
}
