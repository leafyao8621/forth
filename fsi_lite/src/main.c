#include <string.h>

#include "vm/vm.h"
#include "parser/parser.h"
#include "util/status.h"

int main(int argc, char **argv) {
    int ret = 0;
    bool debug = false;
    parser_initialize();
    vm_initialize();
    vm_reset();
    if (argc > 1) {
        FILE *fin = 0;
        if (argc == 2) {
            fin = fopen(argv[1], "r");
            if (!fin) {
                fprintf(stderr, "Cannot open %s\n", argv[1]);
                return 1;
            }
        } else {
            if (!strcmp(argv[1], "-d")) {
                debug = true;
                fin = fopen(argv[2], "r");
                if (!fin) {
                    fprintf(stderr, "Cannot open %s\n", argv[2]);
                    return 1;
                }
            } else {
                fin = fopen(argv[1], "r");
                if (!fin) {
                    fprintf(stderr, "Cannot open %s\n", argv[1]);
                    return 1;
                }
            }
        }
        ret = parser_parse(false, fin);
        fclose(fin);
        if (ret) {
            fprintf(stderr, "Error parsing\n%s\n", parser_status_lookup[ret]);
            return 1;
        }
        if (debug) {
            vm_log();
        }
        ret = vm_run(debug);
        if (ret) {
            fprintf(stderr, "Error running\n%s\n", vm_status_lookup[ret]);
            return 1;
        }
    }
    for (;;) {
        vm_reset();
        printf("%s", "> ");
        ret = parser_parse(true, stdin);
        if (ret == PARSER_STATUS_END_REPL) {
            break;
        }
        if (ret) {
            fprintf(stderr, "Error parsing\n%s\n", parser_status_lookup[ret]);
            continue;
        }
        if (debug) {
            vm_log();
        }
        ret = vm_run(debug);
        if (ret) {
            fprintf(stderr, "Error running\n%s\n", vm_status_lookup[ret]);
        }
    }
    return 0;
}
