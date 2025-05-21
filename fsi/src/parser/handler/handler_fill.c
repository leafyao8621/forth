#include <fsi/util/status.h>

#include "handler.h"

int parser_handler_fill(ForthParser *parser, ForthVM *vm) {
    // : fill rot rot 0 do 2dup c! 1+ loop 2drop ;
    // ROTD ROTD PUSHD 0 2PUSHR 2DUPD STCD INCD INCR JNER (CUR - 6) 2RMR 2DROPD
    // ROTD
    if (
        (parser->state & PARSER_STATE_INTERPRET) &&
        (vm->interpreted_cur == vm->interpreted_end)) {
        parser->status = PARSER_STATUS_END;
        return PARSER_STATUS_INTERPRETED_OVERFLOW;
    }
    if (parser->state & PARSER_STATE_INTERPRET) {
        *(vm->interpreted_cur++) = VM_INSTRUCTION_ROTD;
    }
    if (
        (parser->state & PARSER_STATE_COMPILE) &&
        (vm->compiled_cur == vm->compiled_end)) {
        parser->status = PARSER_STATUS_END;
        return PARSER_STATUS_COMPILED_OVERFLOW;
    }
    if (parser->state & PARSER_STATE_COMPILE) {
        *(vm->compiled_cur++) = VM_INSTRUCTION_ROTD;
    }

    // ROTD
    if (
        (parser->state & PARSER_STATE_INTERPRET) &&
        (vm->interpreted_cur == vm->interpreted_end)) {
        parser->status = PARSER_STATUS_END;
        return PARSER_STATUS_INTERPRETED_OVERFLOW;
    }
    if (parser->state & PARSER_STATE_INTERPRET) {
        *(vm->interpreted_cur++) = VM_INSTRUCTION_ROTD;
    }
    if (
        (parser->state & PARSER_STATE_COMPILE) &&
        (vm->compiled_cur == vm->compiled_end)) {
        parser->status = PARSER_STATUS_END;
        return PARSER_STATUS_COMPILED_OVERFLOW;
    }
    if (parser->state & PARSER_STATE_COMPILE) {
        *(vm->compiled_cur++) = VM_INSTRUCTION_ROTD;
    }

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

    // 2DUPD
    if (
        (parser->state & PARSER_STATE_INTERPRET) &&
        (vm->interpreted_cur == vm->interpreted_end)) {
        parser->status = PARSER_STATUS_END;
        return PARSER_STATUS_INTERPRETED_OVERFLOW;
    }
    if (parser->state & PARSER_STATE_INTERPRET) {
        *(vm->interpreted_cur++) = VM_INSTRUCTION_2DUPD;
    }
    if (
        (parser->state & PARSER_STATE_COMPILE) &&
        (vm->compiled_cur == vm->compiled_end)) {
        parser->status = PARSER_STATUS_END;
        return PARSER_STATUS_COMPILED_OVERFLOW;
    }
    if (parser->state & PARSER_STATE_COMPILE) {
        *(vm->compiled_cur++) = VM_INSTRUCTION_2DUPD;
    }

    // STCD
    if (
        (parser->state & PARSER_STATE_INTERPRET) &&
        (vm->interpreted_cur == vm->interpreted_end)) {
        parser->status = PARSER_STATUS_END;
        return PARSER_STATUS_INTERPRETED_OVERFLOW;
    }
    if (parser->state & PARSER_STATE_INTERPRET) {
        *(vm->interpreted_cur++) = VM_INSTRUCTION_STCD;
    }
    if (
        (parser->state & PARSER_STATE_COMPILE) &&
        (vm->compiled_cur == vm->compiled_end)) {
        parser->status = PARSER_STATUS_END;
        return PARSER_STATUS_COMPILED_OVERFLOW;
    }
    if (parser->state & PARSER_STATE_COMPILE) {
        *(vm->compiled_cur++) = VM_INSTRUCTION_STCD;
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

    // (CUR - 6)
    if (
        (parser->state & PARSER_STATE_INTERPRET) &&
        (vm->interpreted_cur + sizeof(uintptr_t) >= vm->interpreted_end)) {
        parser->status = PARSER_STATUS_END;
        return PARSER_STATUS_INTERPRETED_OVERFLOW;
    }
    if (parser->state & PARSER_STATE_INTERPRET) {
        *(uint8_t**)vm->interpreted_cur = vm->interpreted_cur - 6;
        vm->interpreted_cur += sizeof(uintptr_t);
    }
    if (
        (parser->state & PARSER_STATE_COMPILE) &&
        (vm->compiled_cur == vm->compiled_end)) {
        parser->status = PARSER_STATUS_END;
        return PARSER_STATUS_COMPILED_OVERFLOW;
    }
    if (parser->state & PARSER_STATE_COMPILE) {
        *(uint8_t**)vm->compiled_cur = vm->compiled_cur - 6;
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

    // 2DROPD
    if (
        (parser->state & PARSER_STATE_INTERPRET) &&
        (vm->interpreted_cur == vm->interpreted_end)) {
        parser->status = PARSER_STATUS_END;
        return PARSER_STATUS_INTERPRETED_OVERFLOW;
    }
    if (parser->state & PARSER_STATE_INTERPRET) {
        *(vm->interpreted_cur++) = VM_INSTRUCTION_2DROPD;
    }
    if (
        (parser->state & PARSER_STATE_COMPILE) &&
        (vm->compiled_cur == vm->compiled_end)) {
        parser->status = PARSER_STATUS_END;
        return PARSER_STATUS_COMPILED_OVERFLOW;
    }
    if (parser->state & PARSER_STATE_COMPILE) {
        *(vm->compiled_cur++) = VM_INSTRUCTION_2DROPD;
    }
    return PARSER_STATUS_OK;
}
