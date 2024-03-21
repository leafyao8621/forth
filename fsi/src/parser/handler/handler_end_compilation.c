#include <fsi/util/parser_handlers.h>

ForthVMErr parser_handle_end_compilation(ForthParser *parser, ForthVM *vm) {
    if (!parser || !vm) {
        return FORTHVM_ERR_NULL_PTR;
    }
    char opcode = OPCODE_RETURN;
    int ret = 0;
    switch (parser->state) {
    case FORTHPARSER_STATE_INTERPRET:
        return FORTHVM_ERR_NOT_IN_COMPILATION_MODE;
    case FORTHPARSER_STATE_DEFINE:
        parser->offset = 4;
        vm->offset.data[4] = vm->compiled.size;
        vm->offset_flags.data[4] = OFFSET_PENDING;
        break;
    case FORTHPARSER_STATE_COMPILE:
        ret = DArrayChar_push_back(&vm->compiled, &opcode);
        if (ret) {
            return FORTHVM_ERR_OUT_OF_MEMORY;
        }
        vm->offset_flags.data[parser->offset] = 0;
        parser->state = FORTHPARSER_STATE_INTERPRET;
        break;
    }
    return FORTHVM_ERR_OK;
}
