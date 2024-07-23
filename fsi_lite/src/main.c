#include <string.h>

#include "vm/vm.h"
#include "parser/parser.h"
#include "util/status.h"

int main(int argc, char **argv) {
    int ret = 0;
    bool debug = false;
    parser_initialize();
    vm_initialize();
    if (argc > 1) {
        FILE *fin = 0;
        if (argc == 2) {
            fin = fopen(argv[1], "r");
        } else {
            if (!strcmp(argv[1], "-d")) {
                debug = true;
                fin = fopen(argv[2], "r");
            } else {
                fin = fopen(argv[1], "r");
            }
        }
        if (!fin) {
            perror("Cannot open ");
            perror(argv[1]);
            perror("\n");
            return 1;
        }
        ret = parser_parse(false, fin);
        fclose(fin);
        if (ret) {
            perror("Error parsing\n");
            return 1;
        }
        if (debug) {
            vm_log();
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
            perror("Error parsing\n");
            perror(parser_status_lookup[ret]);
            return 1;
        }
        if (debug) {
            vm_log();
        }
    }
    return 0;
}
