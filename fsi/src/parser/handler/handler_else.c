#include <fsi/util/parser_handlers.h>

ForthVMErr parser_handle_else(ForthParser *parser, ForthVM *vm) {
    if (!parser || !vm) {
        return FORTHVM_ERR_NULL_PTR;
    }
    char opcode = OPCODE_JMP;
    size_t dummy = 0;
    char conditional_type = FORTHPARSER_CONDITIONAL_TYPE_ELSE;
    int ret = 0;
    switch (parser->state) {
    case FORTHPARSER_STATE_INTERPRET:
        return FORTHVM_ERR_NOT_IN_COMPILATION_MODE;
    case FORTHPARSER_STATE_COMPILE:
        ret = DArrayChar_push_back(&vm->compiled, &opcode);
        if (ret) {
            return FORTHVM_ERR_OUT_OF_MEMORY;
        }
        ret = DArrayChar_pop_back(&parser->conditional_type);
        if (ret) {
            return FORTHVM_ERR_NOT_BALANCED;
        }
        ret = DArrayOffset_pop_back(&parser->conditional_offset);
        if (ret) {
            return FORTHVM_ERR_NOT_BALANCED;
        }
        if (
            parser->conditional_type.data[parser->conditional_type.size] !=
            FORTHPARSER_CONDITIONAL_TYPE_IF) {
            return FORTHVM_ERR_NOT_BALANCED;
        }
        vm->compiled.data
            [
                parser->conditional_offset.data[parser->conditional_offset.size]
            ] = vm->compiled.size + 7;
        ret =
            DArrayOffset_push_back(
                &parser->conditional_offset, &vm->compiled.size);
        if (ret) {
            return FORTHVM_ERR_OUT_OF_MEMORY;
        }
        ret =
            DArrayChar_push_back(&parser->conditional_type, &conditional_type);
        if (ret) {
            return FORTHVM_ERR_OUT_OF_MEMORY;
        }
        ret =
            DArrayChar_push_back_batch(
                &vm->compiled, (char*)&dummy, sizeof(size_t));
        if (ret) {
            return FORTHVM_ERR_OUT_OF_MEMORY;
        }
        break;
    case FORTHPARSER_STATE_DEFINE:
        parser->offset = 23;
        vm->offset.data[23] = vm->compiled.size;
        vm->offset_flags.data[23] = OFFSET_PENDING;
        parser->state = FORTHPARSER_STATE_COMPILE;
        break;
    }
    return FORTHVM_ERR_OK;
}
