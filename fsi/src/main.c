#include <string.h>

#include "vm/vm.h"
#include "parser/parser.h"
#include "util/status.h"

int main(int argc, char **argv) {
    int ret = 0;
    bool debug = false;
    bool script = false;
    size_t memory = 65536;
    size_t lookup = 9500;
    size_t literal = 2000;
    size_t interpreted = 2000;
    size_t data_stack = 2000;
    size_t control_stack = 2000;
    size_t compiled = 2000;
    ForthVM vm;
    ForthParser parser;
    ForthVMStatus ret_vm = VM_STATUS_OK;
    ForthParserStatus ret_parser = PARSER_STATUS_OK;
    parser_initialize(&parser);
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
        ret_vm =
            vm_initialize(
                &vm,
                memory,
                lookup,
                literal,
                interpreted,
                data_stack,
                control_stack,
                compiled);
        for (; i < argc; ++i) {
            vm_reset(&vm);
            fin = fopen(argv[i], "r");
            if (!fin) {
                fprintf(stderr, "Cannot open %s\n", argv[i]);
                vm_finalize(&vm);
                parser_finalize(&parser);
                return 1;
            }
            ret = parser_parse(&vm, &parser, debug, false, "");
            fclose(fin);
            if (ret) {
                fprintf(
                    stderr,
                    "Error parsing\n%s\n",
                    parser_status_lookup[ret]
                );
                vm_finalize(&vm);
                parser_finalize(&parser);
                return 1;
            }
            if (debug) {
                vm_log(&vm);
            }
            ret_vm = vm_run(&vm, debug);
            if (ret_vm) {
                fprintf(stderr, "Error running\n%s\n", vm_status_lookup[ret]);
                vm_finalize(&vm);
                parser_finalize(&parser);
                return 1;
            }
        }
    } else {
        ret_vm =
            vm_initialize(
                &vm,
                memory,
                lookup,
                literal,
                interpreted,
                data_stack,
                control_stack,
                compiled);
    }
    if (script) {
        return 0;
    }
    for (;;) {
        vm_reset(&vm);
        printf("%s", "> ");
        ret = parser_parse(&vm, &parser, debug, true, "");
        if (ret == PARSER_STATUS_END_REPL) {
            break;
        }
        if (ret) {
            fprintf(stderr, "Error parsing\n%s\n", parser_status_lookup[ret]);
            if (!parser.eos) {
                for (ret = fgetc(stdin); ret != '\n'; ret = fgetc(stdin));
            }
            continue;
        }
        if (debug) {
            vm_log(&vm);
        }
        ret = vm_run(&vm, debug);
        if (ret) {
            fprintf(stderr, "Error running\n%s\n", vm_status_lookup[ret]);
        }
    }
    vm_finalize(&vm);
    parser_finalize(&parser);
    return 0;
}
