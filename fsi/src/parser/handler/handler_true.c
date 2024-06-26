#include <fsi/util/parser_handlers.h>

ForthVMErr parser_handle_true(ForthParser *parser, ForthVM *vm) {
    if (!parser || !vm) {
        return FORTHVM_ERR_NULL_PTR;
    }
    char opcode = OPCODE_PUSH;
    size_t val = -1;
    int ret = 0;
    switch (parser->state) {
    case FORTHPARSER_STATE_INTERPRET:
        ret = DArrayChar_push_back(&vm->interpreted, &opcode);
        if (ret) {
            return FORTHVM_ERR_OUT_OF_MEMORY;
        }
        ret =
            DArrayChar_push_back_batch(
                &vm->interpreted, (char*)&val, sizeof(size_t));
        if (ret) {
            return FORTHVM_ERR_OUT_OF_MEMORY;
        }
        break;
    case FORTHPARSER_STATE_COMPILE:
        ret = DArrayChar_push_back(&vm->compiled, &opcode);
        if (ret) {
            return FORTHVM_ERR_OUT_OF_MEMORY;
        }
        ret =
            DArrayChar_push_back_batch(
                &vm->compiled, (char*)&val, sizeof(size_t));
        if (ret) {
            return FORTHVM_ERR_OUT_OF_MEMORY;
        }
        break;
    case FORTHPARSER_STATE_DEFINE:
        parser->offset = 53;
        vm->offset.data[53] = vm->compiled.size;
        vm->offset_flags.data[53] = OFFSET_PENDING;
        parser->state = FORTHPARSER_STATE_COMPILE;
        break;
    }
    return FORTHVM_ERR_OK;
}
