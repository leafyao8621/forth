#include <fsi/util/parser_handlers.h>

ForthVMErr parser_handle_repeat(ForthParser *parser, ForthVM *vm) {
    if (!parser || !vm) {
        return FORTHVM_ERR_NULL_PTR;
    }
    int ret = 0;
    char opcode = OPCODE_JMP;
    switch (parser->state) {
    case FORTHPARSER_STATE_INTERPRET:
        return FORTHVM_ERR_NOT_IN_COMPILATION_MODE;
    case FORTHPARSER_STATE_COMPILE:
        ret = DArrayChar_pop_back(&parser->loop_type);
        if (ret) {
            return FORTHVM_ERR_NOT_BALANCED;
        }
        if (
            parser->loop_type.data[parser->loop_type.size] !=
            FORTHPARSER_LOOP_TYPE_WHILE) {
            return FORTHVM_ERR_NOT_BALANCED;
        }
        ret = DArrayOffset_pop_back(&parser->loop_offset);
        if (ret) {
            return FORTHVM_ERR_NOT_BALANCED;
        }
        vm->compiled.data[parser->loop_offset.data[parser->loop_offset.size]] =
            vm->compiled.size + 8;
        ret = DArrayChar_pop_back(&parser->loop_type);
        if (ret) {
            return FORTHVM_ERR_NOT_BALANCED;
        }
        if (
            parser->loop_type.data[parser->loop_type.size] !=
            FORTHPARSER_LOOP_TYPE_BEGIN) {
            return FORTHVM_ERR_NOT_BALANCED;
        }
        ret = DArrayOffset_pop_back(&parser->loop_offset);
        if (ret) {
            return FORTHVM_ERR_NOT_BALANCED;
        }
        ret = DArrayChar_push_back(&vm->compiled, &opcode);
        if (ret) {
            return FORTHVM_ERR_OUT_OF_MEMORY;
        }
        ret =
            DArrayChar_push_back_batch(
                &vm->compiled,
                (char*)(parser->loop_offset.data + parser->loop_offset.size),
                sizeof(size_t));
        if (ret) {
            return FORTHVM_ERR_OUT_OF_MEMORY;
        }
        break;
    case FORTHPARSER_STATE_DEFINE:
        parser->offset = 34;
        vm->offset.data[34] = vm->compiled.size;
        vm->offset_flags.data[34] = OFFSET_PENDING;
        parser->state = FORTHPARSER_STATE_COMPILE;
        break;
    }
    return FORTHVM_ERR_OK;
}
