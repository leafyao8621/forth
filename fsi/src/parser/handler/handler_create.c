#include <fsi/util/parser_handlers.h>
#include <fsi/util/parser_operations.h>

ForthVMErr parser_handle_create(ForthParser *parser, ForthVM *vm) {
    if (!parser || !vm) {
        return FORTHVM_ERR_NULL_PTR;
    }
    switch (parser->state) {
    case FORTHPARSER_STATE_INTERPRET:
    case FORTHPARSER_STATE_COMPILE:
        parser->prev_state = parser->state;
        parser->state = FORTHPARSER_STATE_CREATE;
        break;
    case FORTHPARSER_STATE_DEFINE:
        parser->offset = 38;
        vm->offset.data[38] = vm->compiled.size;
        vm->offset_flags.data[38] = OFFSET_PENDING;
        parser->state = FORTHPARSER_STATE_COMPILE;
        break;
    case FORTHPARSER_STATE_CREATE:
    case FORTHPARSER_STATE_VARIABLE:
        return FORTHVM_ERR_UNDEFINED_BEHAVIOR;
    }
    return FORTHVM_ERR_OK;
}
