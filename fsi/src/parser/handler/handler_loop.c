#include <fsi/util/parser_handlers.h>

ForthVMErr parser_handle_loop(ForthParser *parser, ForthVM *vm) {
    if (!parser || !vm) {
        return FORTHVM_ERR_NULL_PTR;
    }
    char opcode = OPCODE_INCR;
    int ret = 0;
    switch (parser->state) {
    case FORTHPARSER_STATE_INTERPRET:
        return FORTHVM_ERR_NOT_IN_COMPILATION_MODE;
    case FORTHPARSER_STATE_COMPILE:
        ret = DArrayChar_pop_back(&parser->loop_type);
        if (
            ret ||
            parser->loop_type.data[parser->loop_type.size] !=
            FORTHPARSER_LOOP_TYPE_DO) {
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
        opcode = OPCODE_JGTR;
        ret = DArrayChar_push_back(&vm->compiled, &opcode);
        if (ret) {
            return FORTHVM_ERR_OUT_OF_MEMORY;
        }
        ret =
            DArrayChar_push_back_batch(
                &vm->compiled,
                (char*)
                    (
                        parser->loop_offset.data +
                        parser->loop_offset.size
                    ),
                sizeof(size_t)
            );
        if (ret) {
            return FORTHVM_ERR_OUT_OF_MEMORY;
        }
        opcode = OPCODE_2POPR;
        ret = DArrayChar_push_back(&vm->compiled, &opcode);
        if (ret) {
            return FORTHVM_ERR_OUT_OF_MEMORY;
        }
        break;
    case FORTHPARSER_STATE_DEFINE:
        parser->offset = 30;
        vm->offset.data[30] = vm->compiled.size;
        vm->offset_flags.data[30] = OFFSET_PENDING;
        parser->state = FORTHPARSER_STATE_COMPILE;
        break;
    }
    return FORTHVM_ERR_OK;
}
