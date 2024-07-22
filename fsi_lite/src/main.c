#include "vm/vm.h"
#include "parser/parser.h"
#include "util/status.h"

int main(int argc, char **argv) {
    int ret = 0;
    parser_initialize();
    vm_initialize();
    if (argc > 1) {
        FILE *fin = fopen(argv[1], "r");
        if (!fin) {
            perror("Cannot open ");
            perror(argv[1]);
            perror("\n");
        }
        ret = parser_parse(false, fin);
        fclose(fin);
        if (ret) {
            perror("Error parsing\n");
        }
    }
    for (;;) {
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
    }
    return 0;
}
