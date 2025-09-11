#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <getopt.h>

#include "init/init.h"
#include "compile/compile.h"
#include "serve/serve.h"

static const char *msg =
    "Usage: fshps [OPTIONS] <action> {init, compile, serve} <port>\n"
    "Arguments:\n"
    "<FILE(S)>\n"
    "Options:\n"
    "-m <MEMORY SIZE>\n"
    "-l <LOOKUP SIZE>\n"
    "-i <LITERAL SIZE>\n"
    "-e <EXT SIZE>\n"
    "-o <MOD SIZE>\n"
    "-p <MOD SO NAME SIZE>\n"
    "-n <INTERPRETED SIZE>\n"
    "-a <DATA STACK SIZE>\n"
    "-f <FLOAT STACK SIZE>\n"
    "-r <RETURN STACK SIZE>\n"
    "-c <COMPILED SIZE>\n";

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fputs(msg, stderr);
        return 1;
    }
    size_t memory = 65536;
    size_t lookup = 9500;
    size_t literal = 2000;
    size_t ext = 2000;
    size_t mod = 2000;
    size_t mod_so = 2000;
    size_t interpreted = 2000;
    size_t data_stack = 2000;
    size_t float_stack = 2000;
    size_t return_stack = 2000;
    size_t compiled = 2000;
    int ret = 0;
    for (; (ret = getopt(argc, argv, "dsm:l:i:e:o:d:n:a:f:r:c:")) != -1;) {
        switch (ret) {
        case 'm':
            memory = atol(optarg);
            break;
        case 'l':
            lookup = atol(optarg);
            break;
        case 'i':
            literal = atol(optarg);
            break;
        case 'e':
            ext = atol(optarg);
            break;
        case 'o':
            mod = atol(optarg);
            break;
        case 'p':
            mod_so = atol(optarg);
            break;
        case 'n':
            interpreted = atol(optarg);
            break;
        case 'a':
            data_stack = atol(optarg);
            break;
        case 'f':
            float_stack = atol(optarg);
            break;
        case 'r':
            return_stack = atol(optarg);
            break;
        case 'c':
            compiled = atol(optarg);
            break;
        case ':':
            fputs(msg, stderr);
            return 1;
        case '?':
            fputs(msg, stderr);
            return 1;
        }
    }
    if (!strcmp(argv[optind], "init")) {
        ErrInit err = fshps_init();
        if (err) {
            switch (err) {
            case ERR_INIT_MKDIR:
                fputs("MKDIR\n", stderr);
                return 1;
            case ERR_INIT_CREATE:
                fputs("CREATE\n", stderr);
                return 1;
            case ERR_INIT_WRITE:
                fputs("WRITE\n", stderr);
                return 1;
            default:
                break;
            }
        }
    } else if (!strcmp(argv[optind], "compile")) {
        ErrCompile err = fshps_compile();
        if (err) {
            switch (err) {
            case ERR_COMPILE_OPEN:
                fputs("OPEN\n", stderr);
                return 1;
            case ERR_COMPILE_OUT_OF_MEMORY:
                fputs("OUT OF MEMORY\n", stderr);
                return 1;
            case ERR_COMPILE_INVALID_HEADER:
                fputs("INVALID HEADER\n", stderr);
                return 1;
            default:
                break;
            }
        }
    } else if (!strcmp(argv[optind], "serve")) {
        if (optind + 1 != argc - 1) {
            fputs(msg, stderr);
            return 1;
        }
        fshps_serve(
            atoi(argv[optind + 1]),
            memory,
            lookup,
            literal,
            ext,
            mod,
            mod_so,
            interpreted,
            data_stack,
            float_stack,
            return_stack,
            compiled
        );
    } else {
        puts("Usage: fshps <action> {init, compile, serve} <ip>");
        return 1;
    }
    return 0;
}
