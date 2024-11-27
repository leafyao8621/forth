#include <fsi/util/status.h>

#include "handler.h"

int parser_handler_spaces(ForthParser *parser, ForthVM *vm) {
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
        *(uintptr_t*)vm->interpreted_cur = 0;
        vm->interpreted_cur += sizeof(uintptr_t);
        if (vm->interpreted_cur == vm->interpreted_end) {
            parser->status = PARSER_STATUS_END;
            return PARSER_STATUS_INTERPRETED_OVERFLOW;
        }
        *(vm->interpreted_cur++) = VM_INSTRUCTION_2PUSHR;
        uint8_t* temp = vm->interpreted_cur;
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
        if (vm->interpreted_cur == vm->interpreted_end) {
            parser->status = PARSER_STATUS_END;
            return PARSER_STATUS_INTERPRETED_OVERFLOW;
        }
        *(vm->interpreted_cur++) = VM_INSTRUCTION_INCR;
        if (vm->interpreted_cur == vm->interpreted_end) {
            parser->status = PARSER_STATUS_END;
            return PARSER_STATUS_INTERPRETED_OVERFLOW;
        }
        *(vm->interpreted_cur++) = VM_INSTRUCTION_JNER;
        *(uint8_t**)(vm->interpreted_cur) = temp - 1;
        if (vm->interpreted_cur + sizeof(uintptr_t) > vm->interpreted_end) {
            parser->status = PARSER_STATUS_END;
            return PARSER_STATUS_INTERPRETED_OVERFLOW;
        }
        vm->interpreted_cur += sizeof(uintptr_t);
        if (vm->interpreted_cur == vm->interpreted_end) {
            parser->status = PARSER_STATUS_END;
            return PARSER_STATUS_INTERPRETED_OVERFLOW;
        }
        *(vm->interpreted_cur++) = VM_INSTRUCTION_2RMR;
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
        *(uintptr_t*)vm->compiled_cur = 0;
        vm->compiled_cur += sizeof(uintptr_t);
        if (vm->compiled_cur == vm->compiled_end) {
            parser->status = PARSER_STATUS_END;
            return PARSER_STATUS_COMPILED_OVERFLOW;
        }
        *(vm->compiled_cur++) = VM_INSTRUCTION_2PUSHR;
        uint8_t* temp = vm->compiled_cur;
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
        if (vm->compiled_cur == vm->compiled_end) {
            parser->status = PARSER_STATUS_END;
            return PARSER_STATUS_COMPILED_OVERFLOW;
        }
        *(vm->compiled_cur++) = VM_INSTRUCTION_EMIT;
        if (vm->compiled_cur == vm->compiled_end) {
            parser->status = PARSER_STATUS_END;
            return PARSER_STATUS_COMPILED_OVERFLOW;
        }
        *(vm->compiled_cur++) = VM_INSTRUCTION_INCR;
        if (vm->compiled_cur == vm->compiled_end) {
            parser->status = PARSER_STATUS_END;
            return PARSER_STATUS_COMPILED_OVERFLOW;
        }
        *(vm->compiled_cur++) = VM_INSTRUCTION_JNER;
        *(uint8_t**)(vm->compiled_cur) = temp - 1;
        if (vm->compiled_cur + sizeof(uintptr_t) > vm->compiled_end) {
            parser->status = PARSER_STATUS_END;
            return PARSER_STATUS_COMPILED_OVERFLOW;
        }
        vm->compiled_cur += sizeof(uintptr_t);
        if (vm->compiled_cur == vm->compiled_end) {
            parser->status = PARSER_STATUS_END;
            return PARSER_STATUS_COMPILED_OVERFLOW;
        }
        *(vm->compiled_cur++) = VM_INSTRUCTION_2RMR;
    }
    return PARSER_STATUS_OK;
}
