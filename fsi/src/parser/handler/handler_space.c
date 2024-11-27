#include <fsi/util/status.h>

#include "handler.h"

int parser_handler_space(ForthParser *parser, ForthVM *vm) {
    if (parser->state & PARSER_STATE_INTERPRET) {
        if (vm->interpreted_cur == vm->interpreted_end) {
            parser->status = PARSER_STATUS_END;
            return PARSER_STATUS_INTERPRETED_OVERFLOW;
        }
        *(vm->interpreted_cur++) = VM_INSTRUCTION_PUSHD;
        if (vm->interpreted_cur + sizeof(uintptr_t) > vm->interpreted_end) {
            parser->status = PARSER_STATUS_END;
            return PARSER_STATUS_INTERPRETED_OVERFLOW;
        }
        *(uintptr_t*)vm->interpreted_cur = 32;
        vm->interpreted_cur += sizeof(uintptr_t);
        if (vm->interpreted_cur == vm->interpreted_end) {
            parser->status = PARSER_STATUS_END;
            return PARSER_STATUS_INTERPRETED_OVERFLOW;
        }
        *(vm->interpreted_cur++) = VM_INSTRUCTION_EMIT;
    }
    if (parser->state & PARSER_STATE_COMPILE) {
        if (vm->compiled_cur == vm->compiled_end) {
            parser->status = PARSER_STATUS_END;
            return PARSER_STATUS_COMPILED_OVERFLOW;
        }
        *(vm->compiled_cur++) = VM_INSTRUCTION_PUSHD;
        if (vm->compiled_cur + sizeof(uintptr_t) >= vm->compiled_end) {
            parser->status = PARSER_STATUS_END;
            return PARSER_STATUS_COMPILED_OVERFLOW;
        }
        *(uintptr_t*)vm->compiled_cur = 32;
        vm->compiled_cur += sizeof(uintptr_t);
        if (vm->compiled_cur + sizeof(uintptr_t) >= vm->compiled_end) {
            parser->status = PARSER_STATUS_END;
            return PARSER_STATUS_COMPILED_OVERFLOW;
        }
        *(vm->compiled_cur++) = VM_INSTRUCTION_EMIT;
    }
    return PARSER_STATUS_OK;
}
