#include "parser.h"
#include "../util/status.h"
#include "../util/parser_util.h"
#include "../vm/vm.h"
#include "handler/handler.h"

static char buf[100];
static char *buf_end;

uint8_t parser_state;
uint8_t parser_status;

uint8_t *parser_pending;

uint8_t parser_conditional_stack[(sizeof(uintptr_t) + 1) * 20];
uint8_t *parser_conditional_stack_cur;
uint8_t *parser_conditional_stack_end;

uint8_t parser_loop_stack[(sizeof(uintptr_t) + 1) * 20];
uint8_t *parser_loop_stack_cur;
uint8_t *parser_loop_stack_end;

bool parser_eos;

void parser_initialize(void) {
    buf_end = buf + 100;
    parser_state = PARSER_STATE_INTERPRET;
    parser_status = PARSER_STATUS_END;
    parser_pending = 0;
    parser_conditional_stack_cur = parser_conditional_stack;
    parser_conditional_stack_end =
        parser_conditional_stack + (sizeof(uintptr_t) + 1) * 20;
    parser_loop_stack_cur = parser_loop_stack;
    parser_loop_stack_end =
        parser_loop_stack + (sizeof(uintptr_t) + 1) * 20;
}

int parser_parse(bool debug, bool line, FILE *fin) {
    bool ret = false;
    int ret_int = 0;
    uint8_t *meta = 0;
    uintptr_t *addr = 0;
    uintptr_t num = 0;
    char *iter = 0;
    parser_status = PARSER_STATUS_RUNNING;
    parser_eos = false;
    for (next_token(line, fin); !parser_status; next_token(line, fin)) {
        ret = parse_token(buf, buf_end, line, fin);
        if (debug) {
            printf("token: %s\n", buf);
        }
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
                    case PARSER_HANDLER_IF:
                        ret_int = parser_handler_if();
                        break;
                    case PARSER_HANDLER_THEN:
                        ret_int = parser_handler_then();
                        break;
                    case PARSER_HANDLER_ELSE:
                        ret_int = parser_handler_else();
                        break;
                    case PARSER_HANDLER_DO:
                        ret_int = parser_handler_do();
                        break;
                    case PARSER_HANDLER_LOOP:
                        ret_int = parser_handler_loop();
                        break;
                    case PARSER_HANDLER_PLUS_LOOP:
                        ret_int = parser_handler_plus_loop();
                        break;
                    case PARSER_HANDLER_I:
                        ret_int = parser_handler_i();
                        break;
                    case PARSER_HANDLER_LEAVE:
                        ret_int = parser_handler_leave();
                        break;
                    case PARSER_HANDLER_UNLOOP:
                        ret_int = parser_handler_unloop();
                        break;
                    case PARSER_HANDLER_EXIT:
                        ret_int = parser_handler_exit();
                        break;
                    }
                }
            } else {
                if (parser_state & PARSER_STATE_NAME) {
                    parser_pending = meta;
                    *addr = (uintptr_t)vm_compiled_cur;
                    parser_state ^= PARSER_STATE_NAME;
                } else {
                    if (meta == parser_pending) {
                        parser_status = PARSER_STATUS_END;
                        parser_pending = 0;
                        parser_state = PARSER_STATE_INTERPRET;
                        ret_int = PARSER_STATUS_PENDING_DEFINITION;
                        break;
                    }
                    if (*meta & VM_LOOKUP_META_CALL) {
                        if (parser_state & PARSER_STATE_INTERPRET) {
                            if (vm_interpreted_cur == vm_interpreted_end) {
                                parser_status = PARSER_STATUS_END;
                                ret_int = PARSER_STATUS_INTERPRETED_OVERFLOW;
                                break;
                            }
                            *(vm_interpreted_cur++) = VM_INSTRUCTION_CALL;
                            if (
                                vm_interpreted_cur + sizeof(uintptr_t) >=
                                vm_interpreted_end) {
                                parser_status = PARSER_STATUS_END;
                                ret_int =  PARSER_STATUS_INTERPRETED_OVERFLOW;
                                break;
                            }
                            *(uint8_t**)vm_interpreted_cur =
                                (uint8_t*)(*addr - 1);
                            vm_interpreted_cur += sizeof(uintptr_t);
                            if (vm_interpreted_cur == vm_interpreted_end) {
                                parser_status = PARSER_STATUS_END;
                                ret_int =  PARSER_STATUS_INTERPRETED_OVERFLOW;
                                break;
                            }
                        }
                        if (parser_state & PARSER_STATE_COMPILE) {
                            if (vm_compiled_cur == vm_interpreted_end) {
                                parser_status = PARSER_STATUS_END;
                                ret_int =  PARSER_STATUS_COMPILED_OVERFLOW;
                                break;
                            }
                            *(vm_compiled_cur++) = VM_INSTRUCTION_CALL;
                            if (
                                vm_compiled_cur + sizeof(uintptr_t) >=
                                vm_compiled_end) {
                                parser_status = PARSER_STATUS_END;
                                ret_int =  PARSER_STATUS_COMPILED_OVERFLOW;
                                break;
                            }
                        *(uint8_t**)vm_compiled_cur = (uint8_t*)(*addr - 1);
                            vm_compiled_cur += sizeof(uintptr_t);
                            if (vm_compiled_cur == vm_compiled_end) {
                                parser_status = PARSER_STATUS_END;
                                ret_int =  PARSER_STATUS_COMPILED_OVERFLOW;
                                break;
                            }
                        }
                    }
                }

            }
        } else {
            if (parser_state & PARSER_STATE_NAME) {
                if (vm_lookup_cur == vm_lookup_end) {
                    ret_int = PARSER_STATUS_LOOKUP_OVERFLOW;
                    break;
                }
                parser_pending = vm_lookup_cur;
                *(vm_lookup_cur++) = VM_LOOKUP_META_CALL;
                if (vm_lookup_cur + sizeof(uintptr_t) >= vm_lookup_end) {
                    ret_int = PARSER_STATUS_LOOKUP_OVERFLOW;
                    break;
                }
                *(uintptr_t*)vm_lookup_cur = (uintptr_t)vm_compiled_cur;
                vm_lookup_cur += sizeof(uintptr_t);
                for (iter = buf; *iter; ++iter) {
                    if (vm_lookup_cur == vm_lookup_end) {
                        ret_int = PARSER_STATUS_LOOKUP_OVERFLOW;
                        break;
                    }
                    *(vm_lookup_cur++) = *iter;
                }
                if (vm_lookup_cur == vm_lookup_end) {
                    ret_int = PARSER_STATUS_LOOKUP_OVERFLOW;
                    break;
                }
                *(vm_lookup_cur++) = 0;
                parser_state ^= PARSER_STATE_NAME;
            } else {
                switch (*vm_memory) {
                case 10:
                    ret = parser_int10(buf, &num);
                    if (parser_state & PARSER_STATE_INTERPRET) {
                        if (vm_interpreted_cur == vm_interpreted_end) {
                            parser_status = PARSER_STATUS_END;
                            return PARSER_STATUS_INTERPRETED_OVERFLOW;
                        }
                        *(vm_interpreted_cur++) = VM_INSTRUCTION_PUSHD;
                        if (
                            vm_interpreted_cur + sizeof(uintptr_t) >
                            vm_interpreted_end) {
                            parser_status = PARSER_STATUS_END;
                            return PARSER_STATUS_INTERPRETED_OVERFLOW;
                        }
                        *(uintptr_t*)vm_interpreted_cur = num;
                        vm_interpreted_cur += sizeof(uintptr_t);
                    }
                    if (parser_state & PARSER_STATE_COMPILE) {
                        if (vm_compiled_cur == vm_compiled_end) {
                            parser_status = PARSER_STATUS_END;
                            return PARSER_STATUS_COMPILED_OVERFLOW;
                        }
                        *(vm_compiled_cur++) = VM_INSTRUCTION_PUSHD;
                        if (
                            vm_compiled_cur + sizeof(uintptr_t) >
                            vm_compiled_end) {
                            parser_status = PARSER_STATUS_END;
                            return PARSER_STATUS_COMPILED_OVERFLOW;
                        }
                        *(uintptr_t*)vm_compiled_cur = num;
                        vm_compiled_cur += sizeof(uintptr_t);
                    }
                    break;
                default:
                    ret_int = PARSER_STATUS_INVALID_BASE;
                    break;
                }
                if (ret_int) {
                    break;
                }
                if (!ret) {
                    ret_int = PARSER_STATUS_INVALID_BASE10;
                    break;
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
