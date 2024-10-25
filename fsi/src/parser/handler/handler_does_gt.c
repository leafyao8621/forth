#include <string.h>

#include <fsi/util/status.h>

#include "handler.h"

int parser_handler_does_gt(ForthParser *parser, ForthVM *vm) {
    if (parser->state & PARSER_STATE_INTERPRET) {
        parser->status = PARSER_STATUS_END;
        return PARSER_STATUS_NOT_IN_COMPILATION_MODE;
    }
    parser->state |= PARSER_STATE_DOES;
    *parser->pending |= VM_LOOKUP_META_DOES;
    if (vm->compiled_cur == vm->compiled_end) {
        parser->status = PARSER_STATUS_END;
        return PARSER_STATUS_COMPILED_OVERFLOW;
    }
    *(vm->compiled_cur++) = VM_INSTRUCTION_RET;
    if (vm->compiled_cur == vm->compiled_end) {
        parser->status = PARSER_STATUS_END;
        return PARSER_STATUS_COMPILED_OVERFLOW;
    }
    *(vm->compiled_cur++) = VM_INSTRUCTION_HALT;
    if (vm->lookup_cur + sizeof(uintptr_t) >= vm->lookup_end) {
        parser->status = PARSER_STATUS_END;
        return PARSER_STATUS_LOOKUP_OVERFLOW;
    }
    memmove(
        parser->pending + sizeof(uintptr_t) + 1 + sizeof(uintptr_t),
        parser->pending + sizeof(uintptr_t) + 1,
        vm->lookup_cur - (parser->pending + sizeof(uintptr_t) + 1)
    );
    vm->lookup_cur += sizeof(uintptr_t);
    *(uint8_t**)(parser->pending + sizeof(uintptr_t) + 1) =
        vm->compiled_cur;
    return PARSER_STATUS_OK;
}
