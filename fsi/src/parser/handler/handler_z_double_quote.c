#include <fsi/util/status.h>

#include "handler.h"

int parser_handler_z_double_quote(
    ForthParser *parser, char **iter, ForthVM *vm) {
    if (parser->state & PARSER_STATE_INTERPRET) {
        parser->status = PARSER_STATUS_END;
        return PARSER_STATUS_NOT_IN_COMPILATION_MODE;
    }
    uint8_t *base = vm->literal_cur;
    for (++(*iter); **iter && **iter != '"'; ++(*iter)) {
        if (!**iter) {
            parser->status = PARSER_STATUS_END;
            return PARSER_STATUS_INCOMPLETE_STRING;
        }
        if (vm->literal_cur == vm->literal_end) {
            parser->status = PARSER_STATUS_END;
            return PARSER_STATUS_LITERAL_OVERFLOW;
        }
        *(vm->literal_cur++) = **iter;
    }
    if (vm->literal_cur == vm->literal_end) {
        parser->status = PARSER_STATUS_END;
        return PARSER_STATUS_LITERAL_OVERFLOW;
    }
    *(vm->literal_cur++) = 0;

    if (vm->compiled_cur == vm->compiled_end) {
        parser->status = PARSER_STATUS_END;
        return PARSER_STATUS_COMPILED_OVERFLOW;
    }
    *(vm->compiled_cur++) = VM_INSTRUCTION_PUSHD;

    if (vm->compiled_cur + sizeof(uintptr_t) >= vm->compiled_end) {
        parser->status = PARSER_STATUS_END;
        return PARSER_STATUS_COMPILED_OVERFLOW;
    }
    *(uint8_t**)vm->compiled_cur = base;
    vm->compiled_cur += sizeof(uintptr_t);

    ++(*iter);
    return PARSER_STATUS_OK;
}
