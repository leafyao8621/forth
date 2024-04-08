#include <fsi/util/parser_handlers.h>

ForthVMErr parser_handle_then(ForthParser *parser, ForthVM *vm) {
    if (!parser || !vm) {
        return FORTHVM_ERR_NULL_PTR;
    }
    int ret = 0;
    switch (parser->state) {
    case FORTHPARSER_STATE_INTERPRET:
        return FORTHVM_ERR_NOT_IN_COMPILATION_MODE;
    case FORTHPARSER_STATE_COMPILE:
        ret = DArrayChar_pop_back(&parser->conditional_type);
        if (ret) {
            return FORTHVM_ERR_NOT_BALANCED;
        }
        ret = DArrayOffset_pop_back(&parser->conditional_offset);
        if (ret) {
            return FORTHVM_ERR_NOT_BALANCED;
        }
        vm->compiled.data
            [
                parser->conditional_offset.data[parser->conditional_offset.size]
            ] = vm->compiled.size - 1;
        break;
    case FORTHPARSER_STATE_DEFINE:
        parser->offset = 24;
        vm->offset.data[24] = vm->compiled.size;
        vm->offset_flags.data[24] = OFFSET_PENDING;
        parser->state = FORTHPARSER_STATE_COMPILE;
        break;
    }
    return FORTHVM_ERR_OK;
}
