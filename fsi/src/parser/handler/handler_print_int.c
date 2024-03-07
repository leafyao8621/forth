#include <fsi/util/parser_handlers.h>

ForthVMErr parser_handle_print_int(ForthParser *parser, ForthVM *vm) {
    if (!parser || !vm) {
        return FORTHVM_ERR_NULL_PTR;
    }
    char opcode = OPCODE_PRINT_INT;
    if (!parser->compile) {
        int ret = DArrayChar_push_back(&vm->interpreted, &opcode);
        if (ret) {
            return FORTHVM_ERR_OUT_OF_MEMORY;
        }
    }
    return FORTHVM_ERR_OK;
}
