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
    uint8_t *meta = 0;
    uintptr_t *addr = 0;
    uintptr_t num = 0;
    char *iter = 0;
    parser_status = PARSER_STATUS_RUNNING;
    for (next_token(line, fin); !parser_status; next_token(line, fin)) {
        ret = parse_token(buf, buf_end, line, fin);
        if (!ret) {
            return PARSER_STATUS_TOKEN_TOO_LONG;
        }
        ret = lookup_token(buf, &meta, &addr);
        if (ret) {
            if (*meta == VM_LOOKUP_META_BUILTIN) {
                if (parser_state & PARSER_STATE_NAME) {
                    *meta ^= VM_LOOKUP_META_BUILTIN;
                    *meta |= VM_LOOKUP_META_CALL;
                    *addr = (uintptr_t)vm_compiled_cur;
                    parser_state ^= PARSER_STATE_NAME;
                } else {
                    switch (*addr) {
                    case PARSER_HANDLER_DOT:
                        ret_int = parser_handler_dot();
                        break;
                    case PARSER_HANDLER_EMIT:
                        ret_int = parser_handler_emit();
                        break;
                    case PARSER_HANDLER_CR:
                        ret_int = parser_handler_cr();
                        break;
                    case PARSER_HANDLER_COLON:
                        ret_int = parser_handler_colon();
                        break;
                    case PARSER_HANDLER_SEMI_COLON:
                        ret_int = parser_handler_semi_colon();
                        break;
                    }
                }
            } else {
                if (parser_state & PARSER_STATE_NAME) {
                    *addr = (uintptr_t)vm_compiled_cur;
                    parser_state ^= PARSER_STATE_NAME;
                } else {
                    if (*meta & VM_LOOKUP_META_CALL) {
                        if (parser_state & PARSER_STATE_INTERPRET) {
                            if (vm_interpreted_cur == vm_interpreted_end) {
                                parser_status = PARSER_STATUS_END;
                                return PARSER_STATUS_INTERPRETED_OVERFLOW;
                            }
                            *(vm_interpreted_cur++) = VM_INSTRUCTION_CALL;
                            if (
                                vm_interpreted_cur + sizeof(uintptr_t) >=
                                vm_interpreted_end) {
                                parser_status = PARSER_STATUS_END;
                                return PARSER_STATUS_INTERPRETED_OVERFLOW;
                            }
                            *(uint8_t**)vm_interpreted_cur =
                                (uint8_t*)(*addr - 1);
                            vm_interpreted_cur += sizeof(uintptr_t);
                            if (vm_interpreted_cur == vm_interpreted_end) {
                                parser_status = PARSER_STATUS_END;
                                return PARSER_STATUS_INTERPRETED_OVERFLOW;
                            }
                        }
                        if (parser_state & PARSER_STATE_COMPILE) {
                            if (vm_compiled_cur == vm_interpreted_end) {
                                parser_status = PARSER_STATUS_END;
                                return PARSER_STATUS_COMPILED_OVERFLOW;
                            }
                            *(vm_compiled_cur++) = VM_INSTRUCTION_CALL;
                            if (
                                vm_compiled_cur + sizeof(uintptr_t) >=
                                vm_compiled_end) {
                                parser_status = PARSER_STATUS_END;
                                return PARSER_STATUS_COMPILED_OVERFLOW;
                            }
                        *(uint8_t**)vm_compiled_cur = (uint8_t*)(*addr - 1);
                            vm_compiled_cur += sizeof(uintptr_t);
                            if (vm_compiled_cur == vm_compiled_end) {
                                parser_status = PARSER_STATUS_END;
                                return PARSER_STATUS_COMPILED_OVERFLOW;
                            }
                        }
                    }
                }

            }
        } else {
            if (parser_state & PARSER_STATE_NAME) {
                if (vm_lookup_cur == vm_lookup_end) {
                    return PARSER_STATUS_LOOKUP_OVERFLOW;
                }
                *(vm_lookup_cur++) = VM_LOOKUP_META_CALL;
                if (vm_lookup_cur + sizeof(uintptr_t) >= vm_lookup_end) {
                    return PARSER_STATUS_LOOKUP_OVERFLOW;
                }
                *(uintptr_t*)vm_lookup_cur = (uintptr_t)vm_compiled_cur;
                vm_lookup_cur += sizeof(uintptr_t);
                for (iter = buf; *iter; ++iter) {
                    if (vm_lookup_cur == vm_lookup_end) {
                        return PARSER_STATUS_LOOKUP_OVERFLOW;
                    }
                    *(vm_lookup_cur++) = *iter;
                }
                if (vm_lookup_cur == vm_lookup_end) {
                    return PARSER_STATUS_LOOKUP_OVERFLOW;
                }
                *(vm_lookup_cur++) = 0;
                parser_state ^= PARSER_STATE_NAME;
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
