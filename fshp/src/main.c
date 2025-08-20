#include <string.h>
#include <getopt.h>

#include <fshp/fshp.h>

const static char *msg =
    "Usage: fsi [OPTIONS] <FILE(S)>\n"
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

int main(int argc, char **argv) {
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
    ForthVM vm;
    ForthParser parser;
    ForthVMStatus ret_vm = VM_STATUS_OK;
    ForthParserStatus ret_parser = PARSER_STATUS_OK;
    ret_parser = parser_initialize(&parser);
    String buf;
    int ret = DArrayChar_initialize(&buf, 1000);
    if (ret) {
        fprintf(stderr, "%s\n", "Out of memory");
        return 1;
    }
    bool success = false;
    FILE *fin = 0;
    ret = 0;
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
    ret_vm =
        vm_initialize(
            &vm,
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
    for (int i = optind; i < argc; ++i) {
        vm_reset(&vm);
        FSHPStatus status = fshp_process_file(argv[i], &vm, &parser, &buf);
        if (status) {
            fprintf(
                stderr,
                "Error processing file '%s': %s\n",
                argv[i],
                fshp_status_lookup[status]
            );
            break;
        }
    }
    DArrayChar_finalize(&buf);
    vm_finalize(&vm);
    parser_finalize(&parser);
    return 0;
}
