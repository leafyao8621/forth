#include <fsi/util/parser_handlers.h>

ForthVMErr parser_handle_do(ForthParser *parser, ForthVM *vm) {
    if (!parser || !vm) {
        return FORTHVM_ERR_NULL_PTR;
    }
    char opcode = OPCODE_2PUSHR;
    char loop_type = FORTHPARSER_LOOP_TYPE_DO;
    int ret = 0;
    switch (parser->state) {
    case FORTHPARSER_STATE_INTERPRET:
        return FORTHVM_ERR_NOT_IN_COMPILATION_MODE;
    case FORTHPARSER_STATE_COMPILE:
        ret =
            DArrayChar_push_back(&parser->loop_type, &loop_type);
        if (ret) {
            return FORTHVM_ERR_OUT_OF_MEMORY;
        }
        ret =
            DArrayOffset_push_back(
                &parser->loop_offset, &vm->compiled.size);
        if (ret) {
            return FORTHVM_ERR_OUT_OF_MEMORY;
        }
        ret = DArrayChar_push_back(&vm->compiled, &opcode);
        if (ret) {
            return FORTHVM_ERR_OUT_OF_MEMORY;
        }
        break;
    case FORTHPARSER_STATE_DEFINE:
        parser->offset = 29;
        vm->offset.data[29] = vm->compiled.size;
        vm->offset_flags.data[29] = OFFSET_PENDING;
        parser->state = FORTHPARSER_STATE_COMPILE;
        break;
    }
    return FORTHVM_ERR_OK;
}
