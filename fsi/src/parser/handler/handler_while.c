#include <fsi/util/parser_handlers.h>

ForthVMErr parser_handle_while(ForthParser *parser, ForthVM *vm) {
    if (!parser || !vm) {
        return FORTHVM_ERR_NULL_PTR;
    }
    int ret = 0;
    char opcode = OPCODE_JZD;
    char loop_type = FORTHPARSER_LOOP_TYPE_WHILE;
    size_t offset = 0;
    switch (parser->state) {
    case FORTHPARSER_STATE_INTERPRET:
        return FORTHVM_ERR_NOT_IN_COMPILATION_MODE;
    case FORTHPARSER_STATE_COMPILE:
        if (
            parser->loop_type.data[parser->loop_type.size - 1] !=
            FORTHPARSER_LOOP_TYPE_BEGIN) {
            return FORTHVM_ERR_NOT_BALANCED;
        }
        ret =
            DArrayChar_push_back(&parser->loop_type, &loop_type);
        if (ret) {
            return FORTHVM_ERR_OUT_OF_MEMORY;
        }
        ret = DArrayChar_push_back(&vm->compiled, &opcode);
        if (ret) {
            return FORTHVM_ERR_OUT_OF_MEMORY;
        }
        offset = vm->compiled.size;
        ret = DArrayOffset_push_back(&parser->loop_offset, &offset);
        if (ret) {
            return FORTHVM_ERR_OUT_OF_MEMORY;
        }
        offset = 0;
        ret =
            DArrayChar_push_back_batch(
                &vm->compiled, (char*)&offset, sizeof(size_t));
        if (ret) {
            return FORTHVM_ERR_OUT_OF_MEMORY;
        }
        break;
    case FORTHPARSER_STATE_DEFINE:
        parser->offset = 33;
        vm->offset.data[33] = vm->compiled.size;
        vm->offset_flags.data[33] = OFFSET_PENDING;
        parser->state = FORTHPARSER_STATE_COMPILE;
        break;
    }
    return FORTHVM_ERR_OK;
}
