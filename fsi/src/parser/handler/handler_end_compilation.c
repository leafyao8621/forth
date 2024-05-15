#include <fsi/util/parser_handlers.h>

ForthVMErr parser_handle_end_compilation(ForthParser *parser, ForthVM *vm) {
    if (!parser || !vm) {
        return FORTHVM_ERR_NULL_PTR;
    }
    char opcode = OPCODE_RETURN;
    int ret = 0;
    size_t handler_offset = 5;
    switch (parser->state) {
    case FORTHPARSER_STATE_INTERPRET:
        return FORTHVM_ERR_NOT_IN_COMPILATION_MODE;
    case FORTHPARSER_STATE_DEFINE:
        parser->offset = handler_offset;
        vm->offset.data[handler_offset] = vm->compiled.size;
        vm->offset_flags.data[handler_offset] = OFFSET_PENDING;
        parser->state = FORTHPARSER_STATE_COMPILE;
        break;
    case FORTHPARSER_STATE_COMPILE:
        if (parser->conditional_type.size || parser->loop_type.size) {
            return FORTHVM_ERR_NOT_BALANCED;
        }
        ret = DArrayChar_push_back(&vm->compiled, &opcode);
        if (ret) {
            return FORTHVM_ERR_OUT_OF_MEMORY;
        }
        vm->offset_flags.data[parser->offset] &= ~OFFSET_PENDING;
        parser->state = FORTHPARSER_STATE_INTERPRET;
        break;
    case FORTHPARSER_STATE_CREATE:
    case FORTHPARSER_STATE_VARIABLE:
        switch (parser->prev_state) {
        case FORTHPARSER_STATE_INTERPRET:
            vm->offset_flags.data[handler_offset] = OFFSET_MEMORY;
            opcode = OPCODE_DW;
            ret = DArrayChar_push_back(&vm->interpreted, &opcode);
            if (ret) {
                return FORTHVM_ERR_OUT_OF_MEMORY;
            }
            ret =
                DArrayChar_push_back_batch(
                    &vm->interpreted,
                    (char*)&handler_offset,
                    sizeof(size_t));
            if (ret) {
                return FORTHVM_ERR_OUT_OF_MEMORY;
            }
            if (parser->state == FORTHPARSER_STATE_VARIABLE) {
                opcode = OPCODE_ALLOTC;
                ret = DArrayChar_push_back(&vm->interpreted, &opcode);
                if (ret) {
                    return FORTHVM_ERR_OUT_OF_MEMORY;
                }
            }
            if (parser->deferred) {
                opcode = OPCODE_CALL;
                ret = DArrayChar_push_back(&vm->interpreted, &opcode);
                if (ret) {
                    return FORTHVM_ERR_OUT_OF_MEMORY;
                }
                ret =
                    DArrayChar_push_back_batch(
                        &vm->interpreted,
                        (char*)&parser->deferred_offset,
                        sizeof(size_t)
                    );
                if (ret) {
                    return FORTHVM_ERR_OUT_OF_MEMORY;
                }
                parser->deferred = false;
            }
            break;
        case FORTHPARSER_STATE_COMPILE:
            vm->offset_flags.data[parser->offset] |=
                parser->state == FORTHPARSER_STATE_CREATE ?
                OFFSET_CREATE :
                OFFSET_VARIABLE;
            if (parser->conditional_type.size || parser->loop_type.size) {
                return FORTHVM_ERR_NOT_BALANCED;
            }
            ret = DArrayChar_push_back(&vm->compiled, &opcode);
            if (ret) {
                return FORTHVM_ERR_OUT_OF_MEMORY;
            }
            vm->offset_flags.data[parser->offset] &= ~OFFSET_PENDING;
            break;
        }
        parser->state = FORTHPARSER_STATE_INTERPRET;
        break;
    }
    return FORTHVM_ERR_OK;
}
