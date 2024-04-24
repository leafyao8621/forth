#include <fsi/util/parser_handlers.h>
#include <fsi/util/parser_operations.h>

ForthVMErr parser_handle_start_compilation(ForthParser *parser, ForthVM *vm) {
    if (!parser || !vm) {
        return FORTHVM_ERR_NULL_PTR;
    }
    ForthVMErr err = FORTHVM_ERR_OK;
    char opcode = OPCODE_TERMINATE;
    int ret = 0;
    switch (parser->state) {
    case FORTHPARSER_STATE_INTERPRET:
        ret = DArrayChar_push_back(&vm->interpreted, &opcode);
        if (ret) {
            return FORTHVM_ERR_OUT_OF_MEMORY;
        }
        err = ForthVM_execute(vm);
        if (err) {
            return err;
        }
        parser->state = FORTHPARSER_STATE_DEFINE;
        break;
    case FORTHPARSER_STATE_COMPILE:
        return FORTHVM_ERR_NOT_IN_INTERPRETATION_MODE;
    case FORTHPARSER_STATE_DEFINE:
        parser->offset = 4;
        vm->offset.data[4] = vm->compiled.size;
        vm->offset_flags.data[4] = OFFSET_PENDING;
        parser->state = FORTHPARSER_STATE_COMPILE;
        break;
    }
    return FORTHVM_ERR_OK;
}
