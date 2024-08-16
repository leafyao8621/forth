#include <string.h>

#include "vm/vm.h"
#include "parser/parser.h"
#include "util/status.h"

int main(int argc, char **argv) {
    int ret = 0;
    bool debug = false;
    bool script = false;
    parser_initialize();
    vm_initialize();
    if (argc > 1) {
        FILE *fin = 0;
        int i = 1;
        for (; i < argc; ++i) {
            if (!strcmp(argv[i], "-d")) {
                debug = true;
                continue;
            }
            if (!strcmp(argv[i], "-s")) {
                script = true;
                continue;
            }
            break;
        }
        for (; i < argc; ++i) {
            vm_reset();
            fin = fopen(argv[i], "r");
            if (!fin) {
                fprintf(stderr, "Cannot open %s\n", argv[i]);
                return 1;
            }
            ret = parser_parse(debug, false, fin);
            fclose(fin);
            if (ret) {
                fprintf(
                    stderr,
                    "Error parsing\n%s\n",
                    parser_status_lookup[ret]
                );
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
    }
    if (script) {
        return 0;
    }
    for (;;) {
        vm_reset();
        printf("%s", "> ");
        ret = parser_parse(debug, true, stdin);
        if (ret == PARSER_STATUS_END_REPL) {
            break;
        }
        if (ret) {
            fprintf(stderr, "Error parsing\n%s\n", parser_status_lookup[ret]);
            if (!parser_eos) {
                for (ret = fgetc(stdin); ret != '\n'; ret = fgetc(stdin));
            }
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
