#include <fsi/util/parser_handlers.h>
#include <fsi/util/parser_operations.h>

ForthVMErr parser_handle_variable(ForthParser *parser, ForthVM *vm) {
    if (!parser || !vm) {
        return FORTHVM_ERR_NULL_PTR;
    }
    switch (parser->state) {
    case FORTHPARSER_STATE_INTERPRET:
        parser->state = FORTHPARSER_STATE_VARIABLE;
        break;
    case FORTHPARSER_STATE_COMPILE:
        return FORTHVM_ERR_NOT_IN_INTERPRETATION_MODE;
    case FORTHPARSER_STATE_DEFINE:
        parser->offset = 35;
        vm->offset.data[35] = vm->compiled.size;
        vm->offset_flags.data[35] = OFFSET_PENDING;
        parser->state = FORTHPARSER_STATE_COMPILE;
        break;
    }
    return FORTHVM_ERR_OK;
}
