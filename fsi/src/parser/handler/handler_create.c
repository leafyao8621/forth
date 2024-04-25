#include <fsi/util/parser_handlers.h>
#include <fsi/util/parser_operations.h>

ForthVMErr parser_handle_create(ForthParser *parser, ForthVM *vm) {
    if (!parser || !vm) {
        return FORTHVM_ERR_NULL_PTR;
    }
    switch (parser->state) {
    case FORTHPARSER_STATE_INTERPRET:
        parser->state = FORTHPARSER_STATE_CREATE;
        break;
    case FORTHPARSER_STATE_COMPILE:
        return FORTHVM_ERR_NOT_IN_INTERPRETATION_MODE;
    case FORTHPARSER_STATE_DEFINE:
        parser->offset = 38;
        vm->offset.data[38] = vm->compiled.size;
        vm->offset_flags.data[38] = OFFSET_PENDING;
        parser->state = FORTHPARSER_STATE_COMPILE;
        break;
    }
    return FORTHVM_ERR_OK;
}
