#include <fsi/util/status.h>

#include "handler.h"

int parser_handler_recurse(ForthParser *parser, ForthVM *vm) {
    if (parser->state & PARSER_STATE_INTERPRET) {
        parser->status = PARSER_STATUS_END;
        return PARSER_STATUS_NOT_IN_COMPILATION_MODE;
    }
    if (vm->compiled_cur == vm->interpreted_end) {
        parser->status = PARSER_STATUS_END;
        return PARSER_STATUS_COMPILED_OVERFLOW;
    }
    *(vm->compiled_cur++) = VM_INSTRUCTION_CALL;
    if (
        vm->compiled_cur + sizeof(uintptr_t) >=
        vm->compiled_end) {
        parser->status = PARSER_STATUS_END;
        return PARSER_STATUS_COMPILED_OVERFLOW;
    }
    *(uint8_t**)vm->compiled_cur = *(uint8_t**)(parser->pending + 1) - 1;
    vm->compiled_cur += sizeof(uintptr_t);
    if (vm->compiled_cur == vm->compiled_end) {
        parser->status = PARSER_STATUS_END;
        return PARSER_STATUS_COMPILED_OVERFLOW;
    }
    return PARSER_STATUS_OK;
}
