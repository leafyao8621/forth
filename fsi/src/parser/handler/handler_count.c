#include <fsi/util/status.h>

#include "handler.h"

int parser_handler_count(ForthParser *parser, ForthVM *vm) {
    // : count dup 1+ swap c@ ;
    // DUPD INCD SWAPD LDCD
    // DUPD
    if (
        (parser->state & PARSER_STATE_INTERPRET) &&
        (vm->interpreted_cur == vm->interpreted_end)) {
        parser->status = PARSER_STATUS_END;
        return PARSER_STATUS_INTERPRETED_OVERFLOW;
    }
    if (parser->state & PARSER_STATE_INTERPRET) {
        *(vm->interpreted_cur++) = VM_INSTRUCTION_DUPD;
    }
    if (
        (parser->state & PARSER_STATE_COMPILE) &&
        (vm->compiled_cur == vm->compiled_end)) {
        parser->status = PARSER_STATUS_END;
        return PARSER_STATUS_COMPILED_OVERFLOW;
    }
    if (parser->state & PARSER_STATE_COMPILE) {
        *(vm->compiled_cur++) = VM_INSTRUCTION_DUPD;
    }

    // INCD
    if (
        (parser->state & PARSER_STATE_INTERPRET) &&
        (vm->interpreted_cur == vm->interpreted_end)) {
        parser->status = PARSER_STATUS_END;
        return PARSER_STATUS_INTERPRETED_OVERFLOW;
    }
    if (parser->state & PARSER_STATE_INTERPRET) {
        *(vm->interpreted_cur++) = VM_INSTRUCTION_INCD;
    }
    if (
        (parser->state & PARSER_STATE_COMPILE) &&
        (vm->compiled_cur == vm->compiled_end)) {
        parser->status = PARSER_STATUS_END;
        return PARSER_STATUS_COMPILED_OVERFLOW;
    }
    if (parser->state & PARSER_STATE_COMPILE) {
        *(vm->compiled_cur++) = VM_INSTRUCTION_INCD;
    }

    // SWAPD
    if (
        (parser->state & PARSER_STATE_INTERPRET) &&
        (vm->interpreted_cur == vm->interpreted_end)) {
        parser->status = PARSER_STATUS_END;
        return PARSER_STATUS_INTERPRETED_OVERFLOW;
    }
    if (parser->state & PARSER_STATE_INTERPRET) {
        *(vm->interpreted_cur++) = VM_INSTRUCTION_SWAPD;
    }
    if (
        (parser->state & PARSER_STATE_COMPILE) &&
        (vm->compiled_cur == vm->compiled_end)) {
        parser->status = PARSER_STATUS_END;
        return PARSER_STATUS_COMPILED_OVERFLOW;
    }
    if (parser->state & PARSER_STATE_COMPILE) {
        *(vm->compiled_cur++) = VM_INSTRUCTION_SWAPD;
    }

    // LDCD
    if (
        (parser->state & PARSER_STATE_INTERPRET) &&
        (vm->interpreted_cur == vm->interpreted_end)) {
        parser->status = PARSER_STATUS_END;
        return PARSER_STATUS_INTERPRETED_OVERFLOW;
    }
    if (parser->state & PARSER_STATE_INTERPRET) {
        *(vm->interpreted_cur++) = VM_INSTRUCTION_LDCD;
    }
    if (
        (parser->state & PARSER_STATE_COMPILE) &&
        (vm->compiled_cur == vm->compiled_end)) {
        parser->status = PARSER_STATUS_END;
        return PARSER_STATUS_COMPILED_OVERFLOW;
    }
    if (parser->state & PARSER_STATE_COMPILE) {
        *(vm->compiled_cur++) = VM_INSTRUCTION_LDCD;
    }
    return PARSER_STATUS_OK;
}
