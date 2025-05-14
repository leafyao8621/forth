#include <fsi/util/status.h>

#include "handler.h"

int parser_handler_type(ForthParser *parser, ForthVM *vm) {
    // : type 0 dup c@ emit 1+ loop drop ;
    // PUSHD 0 DUPD LDCD EMIT INCD INCR JNER (CUR - 7) 2RMR
    // PUSHD
    if (
        (parser->state & PARSER_STATE_INTERPRET) &&
        (vm->interpreted_cur == vm->interpreted_end)) {
        parser->status = PARSER_STATUS_END;
        return PARSER_STATUS_INTERPRETED_OVERFLOW;
    }
    if (parser->state & PARSER_STATE_INTERPRET) {
        *(vm->interpreted_cur++) = VM_INSTRUCTION_PUSHD;
    }
    if (
        (parser->state & PARSER_STATE_COMPILE) &&
        (vm->compiled_cur == vm->compiled_end)) {
        parser->status = PARSER_STATUS_END;
        return PARSER_STATUS_COMPILED_OVERFLOW;
    }
    if (parser->state & PARSER_STATE_COMPILE) {
        *(vm->compiled_cur++) = VM_INSTRUCTION_PUSHD;
    }

    // 0
    if (
        (parser->state & PARSER_STATE_INTERPRET) &&
        (vm->interpreted_cur + sizeof(uintptr_t) >= vm->interpreted_end)) {
        parser->status = PARSER_STATUS_END;
        return PARSER_STATUS_INTERPRETED_OVERFLOW;
    }
    if (parser->state & PARSER_STATE_INTERPRET) {
        *(uintptr_t*)vm->interpreted_cur = 0;
        vm->interpreted_cur += sizeof(uintptr_t);
    }
    if (
        (parser->state & PARSER_STATE_COMPILE) &&
        (vm->compiled_cur == vm->compiled_end)) {
        parser->status = PARSER_STATUS_END;
        return PARSER_STATUS_COMPILED_OVERFLOW;
    }
    if (parser->state & PARSER_STATE_COMPILE) {
        *(uintptr_t*)vm->compiled_cur = 0;
        vm->compiled_cur += sizeof(uintptr_t);
    }

    // 2PUSHR
    if (
        (parser->state & PARSER_STATE_INTERPRET) &&
        (vm->interpreted_cur == vm->interpreted_end)) {
        parser->status = PARSER_STATUS_END;
        return PARSER_STATUS_INTERPRETED_OVERFLOW;
    }
    if (parser->state & PARSER_STATE_INTERPRET) {
        *(vm->interpreted_cur++) = VM_INSTRUCTION_2PUSHR;
    }
    if (
        (parser->state & PARSER_STATE_COMPILE) &&
        (vm->compiled_cur == vm->compiled_end)) {
        parser->status = PARSER_STATUS_END;
        return PARSER_STATUS_COMPILED_OVERFLOW;
    }
    if (parser->state & PARSER_STATE_COMPILE) {
        *(vm->compiled_cur++) = VM_INSTRUCTION_2PUSHR;
    }

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

    // EMIT
    if (
        (parser->state & PARSER_STATE_INTERPRET) &&
        (vm->interpreted_cur == vm->interpreted_end)) {
        parser->status = PARSER_STATUS_END;
        return PARSER_STATUS_INTERPRETED_OVERFLOW;
    }
    if (parser->state & PARSER_STATE_INTERPRET) {
        *(vm->interpreted_cur++) = VM_INSTRUCTION_EMIT;
    }
    if (
        (parser->state & PARSER_STATE_COMPILE) &&
        (vm->compiled_cur == vm->compiled_end)) {
        parser->status = PARSER_STATUS_END;
        return PARSER_STATUS_COMPILED_OVERFLOW;
    }
    if (parser->state & PARSER_STATE_COMPILE) {
        *(vm->compiled_cur++) = VM_INSTRUCTION_EMIT;
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

    // INCR
    if (
        (parser->state & PARSER_STATE_INTERPRET) &&
        (vm->interpreted_cur == vm->interpreted_end)) {
        parser->status = PARSER_STATUS_END;
        return PARSER_STATUS_INTERPRETED_OVERFLOW;
    }
    if (parser->state & PARSER_STATE_INTERPRET) {
        *(vm->interpreted_cur++) = VM_INSTRUCTION_INCR;
    }
    if (
        (parser->state & PARSER_STATE_COMPILE) &&
        (vm->compiled_cur == vm->compiled_end)) {
        parser->status = PARSER_STATUS_END;
        return PARSER_STATUS_COMPILED_OVERFLOW;
    }
    if (parser->state & PARSER_STATE_COMPILE) {
        *(vm->compiled_cur++) = VM_INSTRUCTION_INCR;
    }

    // JNER
    if (
        (parser->state & PARSER_STATE_INTERPRET) &&
        (vm->interpreted_cur == vm->interpreted_end)) {
        parser->status = PARSER_STATUS_END;
        return PARSER_STATUS_INTERPRETED_OVERFLOW;
    }
    if (parser->state & PARSER_STATE_INTERPRET) {
        *(vm->interpreted_cur++) = VM_INSTRUCTION_JNER;
    }
    if (
        (parser->state & PARSER_STATE_COMPILE) &&
        (vm->compiled_cur == vm->compiled_end)) {
        parser->status = PARSER_STATUS_END;
        return PARSER_STATUS_COMPILED_OVERFLOW;
    }
    if (parser->state & PARSER_STATE_COMPILE) {
        *(vm->compiled_cur++) = VM_INSTRUCTION_JNER;
    }

    // (CUR - 7)
    if (
        (parser->state & PARSER_STATE_INTERPRET) &&
        (vm->interpreted_cur + sizeof(uintptr_t) >= vm->interpreted_end)) {
        parser->status = PARSER_STATUS_END;
        return PARSER_STATUS_INTERPRETED_OVERFLOW;
    }
    if (parser->state & PARSER_STATE_INTERPRET) {
        *(uint8_t**)vm->interpreted_cur = vm->interpreted_cur - 7;
        vm->interpreted_cur += sizeof(uintptr_t);
    }
    if (
        (parser->state & PARSER_STATE_COMPILE) &&
        (vm->compiled_cur == vm->compiled_end)) {
        parser->status = PARSER_STATUS_END;
        return PARSER_STATUS_COMPILED_OVERFLOW;
    }
    if (parser->state & PARSER_STATE_COMPILE) {
        *(uint8_t**)vm->compiled_cur = vm->compiled_cur - 7;
        vm->compiled_cur += sizeof(uintptr_t);
    }

    // 2RMR
    if (
        (parser->state & PARSER_STATE_INTERPRET) &&
        (vm->interpreted_cur == vm->interpreted_end)) {
        parser->status = PARSER_STATUS_END;
        return PARSER_STATUS_INTERPRETED_OVERFLOW;
    }
    if (parser->state & PARSER_STATE_INTERPRET) {
        *(vm->interpreted_cur++) = VM_INSTRUCTION_2RMR;
    }
    if (
        (parser->state & PARSER_STATE_COMPILE) &&
        (vm->compiled_cur == vm->compiled_end)) {
        parser->status = PARSER_STATUS_END;
        return PARSER_STATUS_COMPILED_OVERFLOW;
    }
    if (parser->state & PARSER_STATE_COMPILE) {
        *(vm->compiled_cur++) = VM_INSTRUCTION_2RMR;
    }
    return PARSER_STATUS_OK;
}
