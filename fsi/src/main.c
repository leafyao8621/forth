#include <string.h>

#include <fsi/vm/vm.h>
#include <fsi/parser/parser.h>
#include <fsi/util/parser_util.h>

int main(int argc, char **argv) {
    bool debug = false;
    bool script = false;
    size_t memory = 65536;
    size_t lookup = 9500;
    size_t literal = 2000;
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
                float_stack,
                return_stack,
                compiled
            );
        for (; i < argc; ++i) {
            vm_reset(&vm);
            fin = fopen(argv[i], "r");
            if (!fin) {
                fprintf(stderr, "Cannot open %s\n", argv[i]);
                DArrayChar_finalize(&buf);
                vm_finalize(&vm);
                parser_finalize(&parser);
                return 1;
            }
            success = read_file(fin, &buf);
            fclose(fin);
            if (!success) {
                fprintf(stderr, "%s\n", "Out of memory");
                DArrayChar_finalize(&buf);
                vm_finalize(&vm);
                parser_finalize(&parser);
                return 1;
            }
            ret = parser_parse(&vm, &parser, debug, false, buf.data);
            if (ret) {
                fprintf(
                    stderr,
                    "Error parsing\n%s\n",
                    parser_status_lookup[ret]
                );
                DArrayChar_finalize(&buf);
                vm_finalize(&vm);
                parser_finalize(&parser);
                return 1;
            }
            if (debug) {
                vm_log(&vm);
            }
            ret_vm = vm_run(&vm, debug);
            if (ret_vm) {
                fprintf(
                    stderr,
                    "Error running\n%s\n",
                    vm_status_lookup[ret_vm]
                );
                DArrayChar_finalize(&buf);
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
                float_stack,
                return_stack,
                compiled
            );
    }
    if (script) {
        return 0;
    }
    for (;;) {
        vm_reset(&vm);
        printf("%s", "> ");
        ret = get_input(&buf);
        if (ret == 1) {
            break;
        }
        if (ret == 2) {
            fprintf(stderr, "%s\n", "Out of memory");
            DArrayChar_finalize(&buf);
            vm_finalize(&vm);
            parser_finalize(&parser);
            return 1;
        }
        ret_parser = parser_parse(&vm, &parser, debug, true, buf.data);
        if (ret_parser == PARSER_STATUS_END_REPL) {
            break;
        }
        if (ret_parser) {
            fprintf(
                stderr,
                "Error parsing\n%s\n",
                parser_status_lookup[ret_parser]
            );
            continue;
        }
        if (debug) {
            vm_log(&vm);
        }
        ret_vm = vm_run(&vm, debug);
        if (ret_vm) {
            fprintf(stderr, "Error running\n%s\n", vm_status_lookup[ret_vm]);
            continue;
        }
    }
    DArrayChar_finalize(&buf);
    vm_finalize(&vm);
    parser_finalize(&parser);
    return 0;
}
