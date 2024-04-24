#include <fsi/util/parser_handlers.h>
#include <fsi/util/parser_operations.h>

ForthVMErr parser_handle_load(ForthParser *parser, ForthVM *vm) {
    if (!parser || !vm) {
        return FORTHVM_ERR_NULL_PTR;
    }
    ForthVMErr err = FORTHVM_ERR_OK;
    switch (parser->state) {
    case FORTHPARSER_STATE_INTERPRET:
        err = ForthVM_execute(vm);
        if (err) {
            return err;
        }
        parser->state = FORTHPARSER_STATE_VARIABLE;
        break;
    case FORTHPARSER_STATE_COMPILE:
        return FORTHVM_ERR_NOT_IN_INTERPRETATION_MODE;
    case FORTHPARSER_STATE_DEFINE:
        parser->offset = 37;
        vm->offset.data[37] = vm->compiled.size;
        vm->offset_flags.data[37] = OFFSET_PENDING;
        parser->state = FORTHPARSER_STATE_COMPILE;
        break;
    }
    return FORTHVM_ERR_OK;
}
