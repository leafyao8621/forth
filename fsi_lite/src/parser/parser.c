#include "parser.h"
#include "../util/status.h"
#include "../util/parser_util.h"
#include "../vm/vm.h"
#include "handler/handler.h"

static char buf[100];
static char *buf_end;

uint8_t parser_state;
uint8_t parser_status;

void parser_initialize(void) {
    buf_end = buf + 100;
    parser_state = PARSER_STATE_INTERPRET;
    parser_status = PARSER_STATUS_END;
}

int parser_parse(bool line, FILE *fin) {
    bool ret = false;
    int ret_int = 0;
    uint8_t meta = 0;
    uintptr_t addr = 0;
    uintptr_t num = 0;
    parser_status = PARSER_STATUS_RUNNING;
    for (next_token(line, fin); !parser_status; next_token(line, fin)) {
        ret = parse_token(buf, buf_end, line, fin);
        if (!ret) {
            return PARSER_STATUS_TOKEN_TOO_LONG;
        }
        ret = lookup_token(buf, &meta, &addr);
        if (ret) {
            switch (meta) {
            case VM_LOOKUP_META_BUILTIN:
                switch (addr) {
                case PARSER_HANDLER_DOT:
                    ret_int = parser_handler_dot();
                    break;
                case PARSER_HANDLER_EMIT:
                    ret_int = parser_handler_emit();
                    break;
                case PARSER_HANDLER_CR:
                    ret_int = parser_handler_cr();
                    break;
                }
                break;
            }
        } else {
            switch (*vm_memory) {
            case 10:
                ret = parser_int10(buf, &num);
                if (parser_state & PARSER_STATE_INTERPRET) {
                    *(vm_interpreted_cur++) = VM_INSTRUCTION_PUSHD;
                    *(uintptr_t*)vm_interpreted_cur = num;
                    vm_interpreted_cur += sizeof(uintptr_t);
                }
                if (parser_state & PARSER_STATE_COMPILE) {
                    *(vm_compiled_cur++) = VM_INSTRUCTION_PUSHD;
                    *(uintptr_t*)vm_compiled_cur = num;
                    vm_compiled_cur += sizeof(uintptr_t);
                }
                break;
            default:
                return PARSER_STATUS_INVALID_BASE;
            }
            if (!ret) {
                return PARSER_STATUS_INVALID_BASE10;
            }
        }
        if (parser_status) {
            break;
        }
    }
    if (ret_int) {
        return ret_int;
    }
    if (parser_status == PARSER_STATUS_REPL_END) {
        return PARSER_STATUS_END_REPL;
    }
    if (vm_interpreted_cur == vm_interpreted_end) {
        return PARSER_STATUS_INTERPRETED_OVERFLOW;
    }
    *(vm_interpreted_cur++) = VM_INSTRUCTION_HALT;
    return PARSER_STATUS_OK;
}
