#include <fsi/util/status.h>

#include "handler.h"

int parser_handler_c_double_quote(
    ForthParser *parser, char **iter, ForthVM *vm) {
    if (parser->state & PARSER_STATE_INTERPRET) {
        parser->status = PARSER_STATUS_END;
        return PARSER_STATUS_NOT_IN_COMPILATION_MODE;
    }
    uint8_t *base = vm->literal_cur;
    uintptr_t len = 0;
    for (++(*iter), ++vm->literal_cur; **iter && **iter != '"'; ++(*iter), ++len) {
        if (!**iter) {
            parser->status = PARSER_STATUS_END;
            return PARSER_STATUS_INCOMPLETE_STRING;
        }
        if (vm->literal_cur == vm->literal_end) {
            parser->status = PARSER_STATUS_END;
            return PARSER_STATUS_LITERAL_OVERFLOW;
        }
        if (len > 255) {
            parser->status = PARSER_STATUS_END;
            return PARSER_STATUS_COUNTED_STRING_OVERFLOW;
        }
        *(vm->literal_cur++) = **iter;
    }
    *base = len;
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
