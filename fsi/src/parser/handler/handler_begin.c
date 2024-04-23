#include <fsi/util/parser_handlers.h>

ForthVMErr parser_handle_begin(ForthParser *parser, ForthVM *vm) {
    if (!parser || !vm) {
        return FORTHVM_ERR_NULL_PTR;
    }
    int ret = 0;
    char loop_type = FORTHPARSER_LOOP_TYPE_BEGIN;
    size_t offset = vm->compiled.size - 1;
    switch (parser->state) {
    case FORTHPARSER_STATE_INTERPRET:
        return FORTHVM_ERR_NOT_IN_COMPILATION_MODE;
    case FORTHPARSER_STATE_COMPILE:
        ret =
            DArrayChar_push_back(&parser->loop_type, &loop_type);
        if (ret) {
            return FORTHVM_ERR_OUT_OF_MEMORY;
        }
        ret = DArrayOffset_push_back(&parser->loop_offset, &offset);
        if (ret) {
            return FORTHVM_ERR_OUT_OF_MEMORY;
        }
        break;
    case FORTHPARSER_STATE_DEFINE:
        parser->offset = 32;
        vm->offset.data[32] = vm->compiled.size;
        vm->offset_flags.data[32] = OFFSET_PENDING;
        parser->state = FORTHPARSER_STATE_COMPILE;
        break;
    }
    return FORTHVM_ERR_OK;
}
