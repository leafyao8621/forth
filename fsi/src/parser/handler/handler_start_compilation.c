#include <fsi/util/parser_handlers.h>
#include <fsi/util/parser_operations.h>

ForthVMErr parser_handle_start_compilation(ForthParser *parser, ForthVM *vm) {
    if (!parser || !vm) {
        return FORTHVM_ERR_NULL_PTR;
    }
    switch (parser->state) {
    case FORTHPARSER_STATE_INTERPRET:
    case FORTHPARSER_STATE_COMPILE:
        parser->state = FORTHPARSER_STATE_DEFINE;
        break;
    case FORTHPARSER_STATE_DEFINE:
        parser->offset = 4;
        vm->offset.data[4] = vm->compiled.size;
        vm->offset_flags.data[4] = OFFSET_PENDING;
        parser->state = FORTHPARSER_STATE_COMPILE;
        break;
    }

    return FORTHVM_ERR_OK;
}
