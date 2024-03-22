#include <fsi/util/parser_handlers.h>

ForthVMErr parser_handle_ugeq(ForthParser *parser, ForthVM *vm) {
    if (!parser || !vm) {
        return FORTHVM_ERR_NULL_PTR;
    }
    char opcode = OPCODE_UGEQ;
    int ret = 0;
    switch (parser->state) {
    case FORTHPARSER_STATE_INTERPRET:
        ret = DArrayChar_push_back(&vm->interpreted, &opcode);
        if (ret) {
            return FORTHVM_ERR_OUT_OF_MEMORY;
        }
        break;
    case FORTHPARSER_STATE_COMPILE:
        ret = DArrayChar_push_back(&vm->compiled, &opcode);
        if (ret) {
            return FORTHVM_ERR_OUT_OF_MEMORY;
        }
        break;
    case FORTHPARSER_STATE_DEFINE:
        parser->offset = 15;
        vm->offset.data[15] = vm->compiled.size;
        vm->offset_flags.data[15] = OFFSET_PENDING;
        parser->state = FORTHPARSER_STATE_COMPILE;
        break;
    }
    return FORTHVM_ERR_OK;
}
