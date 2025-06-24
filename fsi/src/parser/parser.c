#include <fsi/parser/parser.h>
#include <fsi/util/status.h>
#include <fsi/util/parser_util.h>

#include "handler/handler.h"

ForthParserStatus parser_initialize(ForthParser *parser) {
    if (DArrayChar_initialize(&parser->buf, 100)) {
        return PARSER_STATUS_OUT_OF_MEMORY;
    }
    parser->state = PARSER_STATE_INTERPRET;
    parser->status = PARSER_STATUS_END;
    parser->pending = 0;
    parser->conditional_stack_cur = parser->conditional_stack;
    parser->conditional_stack_end =
        parser->conditional_stack + (sizeof(uintptr_t) + 1) * 20;
    parser->case_stack_cur = parser->case_stack;
    parser->case_stack_end =
        parser->case_stack + (sizeof(uintptr_t) + 1) * 20;
    parser->loop_stack_cur = parser->loop_stack;
    parser->loop_stack_end =
        parser->loop_stack + (sizeof(uintptr_t) + 1) * 20;
    return PARSER_STATUS_OK;
}

void parser_finalize(ForthParser *parser) {
    DArrayChar_finalize(&parser->buf);
}

ForthParserStatus parser_parse(
    ForthVM *vm, ForthParser *parser, bool debug, bool line, char *str) {
    bool ret = false;
    int ret_int = 0;
    uint8_t *meta = 0;
    uintptr_t *addr = 0;
    uintptr_t num = 0;
    char *iter = str;
    bool indirect = false;
    parser->status = PARSER_STATUS_RUNNING;
    for (
        next_token(parser, line, &iter);
        !parser->status;
        next_token(parser, line, &iter)) {
        ret = parse_token(parser, line, &iter);
        if (debug) {
            printf("token: %s\n", parser->buf.data);
        }
        if (!ret) {
            return PARSER_STATUS_OUT_OF_MEMORY;
        }
        ret = lookup_token(vm, parser->buf.data, &meta, &addr);
        if (ret) {
            if (*meta & VM_LOOKUP_META_BUILTIN) {
                if (parser->state & PARSER_STATE_NAME) {
                    parser->pending = meta;
                    *meta = VM_LOOKUP_META_CALL;
                    *addr = (uintptr_t)vm->compiled_cur;
                    parser->state ^= PARSER_STATE_NAME;
                } else if (parser->state & PARSER_STATE_CREATE) {
                    if (
                        vm->interpreted_cur + sizeof(uintptr_t) >
                        vm->interpreted_end) {
                        parser->status = PARSER_STATUS_END;
                        ret_int = PARSER_STATUS_INTERPRETED_OVERFLOW;
                        break;
                    }
                    indirect = *meta & VM_LOOKUP_META_INDIRECT;
                    *meta = VM_LOOKUP_META_MEMORY;
                    *(uintptr_t*)vm->interpreted_cur = (uintptr_t)addr;
                    vm->interpreted_cur += sizeof(uintptr_t);
                    parser->state ^= PARSER_STATE_CREATE;
                    if (parser->pending) {
                        if (vm->interpreted_cur == vm->interpreted_end) {
                            parser->status = PARSER_STATUS_END;
                            ret_int = PARSER_STATUS_INTERPRETED_OVERFLOW;
                            break;
                        }
                        *(vm->interpreted_cur++) = VM_INSTRUCTION_CALL;
                        if (
                            vm->interpreted_cur + sizeof(uintptr_t) >=
                            vm->interpreted_end) {
                            parser->status = PARSER_STATUS_END;
                            ret_int =  PARSER_STATUS_INTERPRETED_OVERFLOW;
                            break;
                        }
                        *(uint8_t**)vm->interpreted_cur =
                            (uint8_t*)(parser->pending - 1);
                        vm->interpreted_cur += sizeof(uintptr_t);
                        if (vm->interpreted_cur == vm->interpreted_end) {
                            parser->status = PARSER_STATUS_END;
                            ret_int = PARSER_STATUS_INTERPRETED_OVERFLOW;
                            break;
                        }
                        if (parser->state & PARSER_STATE_DOES) {
                            if (!indirect) {
                                if (
                                    vm->lookup_cur + sizeof(uintptr_t) >=
                                    vm->lookup_end) {
                                    parser->status = PARSER_STATUS_END;
                                    return PARSER_STATUS_LOOKUP_OVERFLOW;
                                }
                                memmove(
                                    addr + 1,
                                    addr,
                                    vm->lookup_cur - (uint8_t*)addr
                                );
                                vm->lookup_cur += sizeof(uintptr_t);
                            }
                            *meta |=
                                VM_LOOKUP_META_CALL | VM_LOOKUP_META_INDIRECT;
                            next_function(&parser->pending);
                            *(uint8_t**)(addr + 1) = parser->pending;
                            parser->state ^= PARSER_STATE_DOES;
                        }
                        parser->pending = 0;
                    }
                } else if (parser->state & PARSER_STATE_CHAR) {
                    if (parser->state & PARSER_STATE_INTERPRET) {
                        if (vm->interpreted_cur == vm->interpreted_end) {
                            parser->status = PARSER_STATUS_END;
                            ret_int = PARSER_STATUS_INTERPRETED_OVERFLOW;
                            break;
                        }
                        *(vm->interpreted_cur++) = VM_INSTRUCTION_PUSHD;
                        if (
                            vm->interpreted_cur + sizeof(uintptr_t) >=
                            vm->interpreted_end) {
                            parser->status = PARSER_STATUS_END;
                            ret_int =  PARSER_STATUS_INTERPRETED_OVERFLOW;
                            break;
                        }
                        *(uintptr_t*)vm->interpreted_cur =
                            *parser->buf.data;
                        vm->interpreted_cur += sizeof(uintptr_t);
                    } else {
                        if (vm->compiled_cur == vm->compiled_end) {
                            parser->status = PARSER_STATUS_END;
                            ret_int = PARSER_STATUS_COMPILED_OVERFLOW;
                            break;
                        }
                        *(vm->compiled_cur++) = VM_INSTRUCTION_PUSHD;
                        if (
                            vm->compiled_cur + sizeof(uintptr_t) >=
                            vm->compiled_end) {
                            parser->status = PARSER_STATUS_END;
                            ret_int =  PARSER_STATUS_COMPILED_OVERFLOW;
                            break;
                        }
                        *(uintptr_t*)vm->compiled_cur =
                            *parser->buf.data;
                        vm->compiled_cur += sizeof(uintptr_t);
                    }
                    parser->state ^= PARSER_STATE_CHAR;
                } else {
                    if (*meta & VM_LOOKUP_META_CALLEXT) {
                        if (parser->state & PARSER_STATE_INTERPRET) {
                            if (vm->interpreted_cur == vm->interpreted_end) {
                                parser->status = PARSER_STATUS_END;
                                ret_int = PARSER_STATUS_INTERPRETED_OVERFLOW;
                                break;
                            }
                            *(vm->interpreted_cur++) = VM_INSTRUCTION_CALLEXT;
                            if (
                                vm->interpreted_cur + sizeof(uintptr_t) >=
                                vm->interpreted_end) {
                                parser->status = PARSER_STATUS_END;
                                ret_int =  PARSER_STATUS_INTERPRETED_OVERFLOW;
                                break;
                            }
                            *(uintptr_t*)vm->interpreted_cur =
                                *(uintptr_t*)addr;
                            vm->interpreted_cur += sizeof(uintptr_t);
                        } else {
                            if (vm->compiled_cur == vm->compiled_end) {
                                parser->status = PARSER_STATUS_END;
                                ret_int = PARSER_STATUS_COMPILED_OVERFLOW;
                                break;
                            }
                            *(vm->compiled_cur++) = VM_INSTRUCTION_CALLEXT;
                            if (
                                vm->compiled_cur + sizeof(uintptr_t) >=
                                vm->compiled_end) {
                                parser->status = PARSER_STATUS_END;
                                ret_int =  PARSER_STATUS_COMPILED_OVERFLOW;
                                break;
                            }
                            *(uintptr_t*)vm->compiled_cur =
                                *(uintptr_t*)addr;
                            vm->compiled_cur += sizeof(uintptr_t);
                        }
                    } else {
                        switch (*addr) {
                        case PARSER_HANDLER_DOT:
                            ret_int = parser_handler_dot(parser, vm);
                            break;
                        case PARSER_HANDLER_EMIT:
                            ret_int = parser_handler_emit(parser, vm);
                            break;
                        case PARSER_HANDLER_CR:
                            ret_int = parser_handler_cr(parser, vm);
                            break;
                        case PARSER_HANDLER_BL:
                            ret_int = parser_handler_bl(parser, vm);
                            break;
                        case PARSER_HANDLER_SPACE:
                            ret_int = parser_handler_space(parser, vm);
                            break;
                        case PARSER_HANDLER_SPACES:
                            ret_int = parser_handler_spaces(parser, vm);
                            break;
                        case PARSER_HANDLER_COLON:
                            ret_int = parser_handler_colon(parser);
                            break;
                        case PARSER_HANDLER_SEMI_COLON:
                            ret_int = parser_handler_semi_colon(parser, vm);
                            break;
                        case PARSER_HANDLER_IF:
                            ret_int = parser_handler_if(parser, vm);
                            break;
                        case PARSER_HANDLER_THEN:
                            ret_int = parser_handler_then(parser, vm);
                            break;
                        case PARSER_HANDLER_ELSE:
                            ret_int = parser_handler_else(parser, vm);
                            break;
                        case PARSER_HANDLER_CASE:
                            ret_int = parser_handler_case(parser, vm);
                            break;
                        case PARSER_HANDLER_OF:
                            ret_int = parser_handler_of(parser, vm);
                            break;
                        case PARSER_HANDLER_ENDOF:
                            ret_int = parser_handler_endof(parser, vm);
                            break;
                        case PARSER_HANDLER_ENDCASE:
                            ret_int = parser_handler_endcase(parser, vm);
                            break;
                        case PARSER_HANDLER_DO:
                            ret_int = parser_handler_do(parser, vm);
                            break;
                        case PARSER_HANDLER_LOOP:
                            ret_int = parser_handler_loop(parser, vm);
                            break;
                        case PARSER_HANDLER_PLUS_LOOP:
                            ret_int = parser_handler_plus_loop(parser, vm);
                            break;
                        case PARSER_HANDLER_I:
                            ret_int = parser_handler_i(parser, vm);
                            break;
                        case PARSER_HANDLER_LEAVE:
                            ret_int = parser_handler_leave(parser, vm);
                            break;
                        case PARSER_HANDLER_UNLOOP:
                            ret_int = parser_handler_unloop(parser, vm);
                            break;
                        case PARSER_HANDLER_EXIT:
                            ret_int = parser_handler_exit(parser, vm);
                            break;
                        case PARSER_HANDLER_J:
                            ret_int = parser_handler_j(parser, vm);
                            break;
                        case PARSER_HANDLER_BEGIN:
                            ret_int = parser_handler_begin(parser, vm);
                            break;
                        case PARSER_HANDLER_WHILE:
                            ret_int = parser_handler_while(parser, vm);
                            break;
                        case PARSER_HANDLER_REPEAT:
                            ret_int = parser_handler_repeat(parser, vm);
                            break;
                        case PARSER_HANDLER_UNTIL:
                            ret_int = parser_handler_until(parser, vm);
                            break;
                        case PARSER_HANDLER_AGAIN:
                            ret_int = parser_handler_again(parser, vm);
                            break;
                        case PARSER_HANDLER_CREATE:
                            ret_int = parser_handler_create(parser, vm);
                            break;
                        case PARSER_HANDLER_VARIABLE:
                            ret_int = parser_handler_variable(parser, vm);
                            break;
                        case PARSER_HANDLER_ALLOT:
                            ret_int = parser_handler_allot(parser, vm);
                            break;
                        case PARSER_HANDLER_FILL:
                            ret_int = parser_handler_fill(parser, vm);
                            break;
                        case PARSER_HANDLER_CELLS:
                            ret_int = parser_handler_cells(parser, vm);
                            break;
                        case PARSER_HANDLER_DOES_GT:
                            ret_int = parser_handler_does_gt(parser, vm);
                            break;
                        case PARSER_HANDLER_AT:
                            ret_int = parser_handler_at(parser, vm);
                            break;
                        case PARSER_HANDLER_BANG:
                            ret_int = parser_handler_bang(parser, vm);
                            break;
                        case PARSER_HANDLER_CAT:
                            ret_int = parser_handler_cat(parser, vm);
                            break;
                        case PARSER_HANDLER_CBANG:
                            ret_int = parser_handler_cbang(parser, vm);
                            break;
                        case PARSER_HANDLER_TWO_AT:
                            ret_int = parser_handler_two_at(parser, vm);
                            break;
                        case PARSER_HANDLER_TWO_BANG:
                            ret_int = parser_handler_two_bang(parser, vm);
                            break;
                        case PARSER_HANDLER_PLUS_BANG:
                            ret_int = parser_handler_plus_bang(parser, vm);
                            break;
                        case PARSER_HANDLER_COMMA:
                            ret_int = parser_handler_comma(parser, vm);
                            break;
                        case PARSER_HANDLER_CCOMMA:
                            ret_int = parser_handler_ccomma(parser, vm);
                            break;
                        case PARSER_HANDLER_PLUS:
                            ret_int = parser_handler_plus(parser, vm);
                            break;
                        case PARSER_HANDLER_DASH:
                            ret_int = parser_handler_dash(parser, vm);
                            break;
                        case PARSER_HANDLER_ASTERISK:
                            ret_int = parser_handler_asterisk(parser, vm);
                            break;
                        case PARSER_HANDLER_SLASH:
                            ret_int = parser_handler_slash(parser, vm);
                            break;
                        case PARSER_HANDLER_MOD:
                            ret_int = parser_handler_mod(parser, vm);
                            break;
                        case PARSER_HANDLER_ONE_PLUS:
                            ret_int = parser_handler_one_plus(parser, vm);
                            break;
                        case PARSER_HANDLER_CELL_PLUS:
                            ret_int = parser_handler_cell_plus(parser, vm);
                            break;
                        case PARSER_HANDLER_ONE_DASH:
                            ret_int = parser_handler_one_dash(parser, vm);
                            break;
                        case PARSER_HANDLER_TWO_ASTERISK:
                            ret_int = parser_handler_two_asterisk(parser, vm);
                            break;
                        case PARSER_HANDLER_TWO_SLASH:
                            ret_int = parser_handler_two_slash(parser, vm);
                            break;
                        case PARSER_HANDLER_LSFHIFT:
                            ret_int = parser_handler_lshift(parser, vm);
                            break;
                        case PARSER_HANDLER_RSFHIFT:
                            ret_int = parser_handler_rshift(parser, vm);
                            break;
                        case PARSER_HANDLER_AND:
                            ret_int = parser_handler_and(parser, vm);
                            break;
                        case PARSER_HANDLER_OR:
                            ret_int = parser_handler_or(parser, vm);
                            break;
                        case PARSER_HANDLER_XOR:
                            ret_int = parser_handler_xor(parser, vm);
                            break;
                        case PARSER_HANDLER_INVERT:
                            ret_int = parser_handler_invert(parser, vm);
                            break;
                        case PARSER_HANDLER_NEGATE:
                            ret_int = parser_handler_negate(parser, vm);
                            break;
                        case PARSER_HANDLER_GT:
                            ret_int = parser_handler_gt(parser, vm);
                            break;
                        case PARSER_HANDLER_LT:
                            ret_int = parser_handler_lt(parser, vm);
                            break;
                        case PARSER_HANDLER_EQ:
                            ret_int = parser_handler_eq(parser, vm);
                            break;
                        case PARSER_HANDLER_ULT:
                            ret_int = parser_handler_ult(parser, vm);
                            break;
                        case PARSER_HANDLER_ZERO_LT:
                            ret_int = parser_handler_zero_lt(parser, vm);
                            break;
                        case PARSER_HANDLER_ZERO_EQ:
                            ret_int = parser_handler_zero_eq(parser, vm);
                            break;
                        case PARSER_HANDLER_MIN:
                            ret_int = parser_handler_min(parser, vm);
                            break;
                        case PARSER_HANDLER_MAX:
                            ret_int = parser_handler_max(parser, vm);
                            break;
                        case PARSER_HANDLER_DUP:
                            ret_int = parser_handler_dup(parser, vm);
                            break;
                        case PARSER_HANDLER_DROP:
                            ret_int = parser_handler_drop(parser, vm);
                            break;
                        case PARSER_HANDLER_SWAP:
                            ret_int = parser_handler_swap(parser, vm);
                            break;
                        case PARSER_HANDLER_OVER:
                            ret_int = parser_handler_over(parser, vm);
                            break;
                        case PARSER_HANDLER_NIP:
                            ret_int = parser_handler_nip(parser, vm);
                            break;
                        case PARSER_HANDLER_TWO_DUP:
                            ret_int = parser_handler_two_dup(parser, vm);
                            break;
                        case PARSER_HANDLER_TWO_DROP:
                            ret_int = parser_handler_two_drop(parser, vm);
                            break;
                        case PARSER_HANDLER_TWO_SWAP:
                            ret_int = parser_handler_two_swap(parser, vm);
                            break;
                        case PARSER_HANDLER_TWO_OVER:
                            ret_int = parser_handler_two_over(parser, vm);
                            break;
                        case PARSER_HANDLER_QUESTION_DUP:
                            ret_int = parser_handler_question_dup(parser, vm);
                            break;
                        case PARSER_HANDLER_ROT:
                            ret_int = parser_handler_rot(parser, vm);
                            break;
                        case PARSER_HANDLER_PICK:
                            ret_int = parser_handler_pick(parser, vm);
                            break;
                        case PARSER_HANDLER_DEPTH:
                            ret_int = parser_handler_depth(parser, vm);
                            break;
                        case PARSER_HANDLER_GTR:
                            ret_int = parser_handler_gtr(parser, vm);
                            break;
                        case PARSER_HANDLER_RGT:
                            ret_int = parser_handler_rgt(parser, vm);
                            break;
                        case PARSER_HANDLER_TWO_GTR:
                            ret_int = parser_handler_two_gtr(parser, vm);
                            break;
                        case PARSER_HANDLER_TWO_RGT:
                            ret_int = parser_handler_two_rgt(parser, vm);
                            break;
                        case PARSER_HANDLER_CHAR:
                            ret_int = parser_handler_char(parser);
                            break;
                        case PARSER_HANDLER_CHAR_PLUS:
                            ret_int = parser_handler_char_plus(parser, vm);
                            break;
                        case PARSER_HANDLER_CHARS:
                            break;
                        case PARSER_HANDLER_TYPE:
                            ret_int = parser_handler_type(parser, vm);
                            break;
                        case PARSER_HANDLER_S_DOUBLE_QUOTE:
                            ret_int =
                                parser_handler_s_double_quote(parser, &iter, vm);
                            break;
                        case PARSER_HANDLER_DOT_DOUBLE_QUOTE:
                            ret_int =
                                parser_handler_dot_double_quote(parser, &iter, vm);
                            break;
                        case PARSER_HANDLER_C_DOUBLE_QUOTE:
                            ret_int =
                                parser_handler_c_double_quote(parser, &iter, vm);
                            break;
                        case PARSER_HANDLER_COUNT:
                            ret_int = parser_handler_count(parser, vm);
                            break;
                        case PARSER_HANDLER_D_GT_F:
                            ret_int =
                                parser_handler_d_gt_f(parser, vm);
                            break;
                        case PARSER_HANDLER_F_DOT:
                            ret_int = parser_handler_f_dot(parser, vm);
                            break;
                        case PARSER_HANDLER_F_AT:
                            ret_int =
                                parser_handler_f_at(parser, vm);
                            break;
                        case PARSER_HANDLER_F_BANG:
                            ret_int = parser_handler_f_bang(parser, vm);
                            break;
                        case PARSER_HANDLER_FLOATS:
                            ret_int = parser_handler_floats(parser, vm);
                            break;
                        case PARSER_HANDLER_F_PLUS:
                            ret_int = parser_handler_f_plus(parser, vm);
                            break;
                        case PARSER_HANDLER_F_DASH:
                            ret_int = parser_handler_f_dash(parser, vm);
                            break;
                        case PARSER_HANDLER_F_ASTERISK:
                            ret_int = parser_handler_f_asterisk(parser, vm);
                            break;
                        case PARSER_HANDLER_F_SLASH:
                            ret_int = parser_handler_f_slash(parser, vm);
                            break;
                        case PARSER_HANDLER_FDUP:
                            ret_int = parser_handler_fdup(parser, vm);
                            break;
                        case PARSER_HANDLER_FDROP:
                            ret_int = parser_handler_fdrop(parser, vm);
                            break;
                        case PARSER_HANDLER_FSWAP:
                            ret_int = parser_handler_fswap(parser, vm);
                            break;
                        case PARSER_HANDLER_FOVER:
                            ret_int = parser_handler_fover(parser, vm);
                            break;
                        case PARSER_HANDLER_FROT:
                            ret_int = parser_handler_frot(parser, vm);
                            break;
                        case PARSER_HANDLER_FDEPTH:
                            ret_int = parser_handler_fdepth(parser, vm);
                            break;
                        }
                    }
                }
            } else {
                if (parser->state & PARSER_STATE_NAME) {
                    parser->pending = meta;
                    if (*meta & VM_LOOKUP_META_INDIRECT) {
                        memmove(
                            addr,
                            addr + 1,
                            vm->lookup_cur - (uint8_t*)(addr + 1)
                        );
                        vm->lookup_cur -= sizeof(uintptr_t);
                    }
                    *meta = VM_LOOKUP_META_CALL;
                    *addr = (uintptr_t)vm->compiled_cur;
                    parser->state ^= PARSER_STATE_NAME;
                } else if (parser->state & PARSER_STATE_CREATE) {
                    if (
                        vm->interpreted_cur + sizeof(uintptr_t) >
                        vm->interpreted_end) {
                        parser->status = PARSER_STATUS_END;
                        ret_int = PARSER_STATUS_INTERPRETED_OVERFLOW;
                        break;
                    }
                    indirect = *meta & VM_LOOKUP_META_INDIRECT;
                    *meta = VM_LOOKUP_META_MEMORY;
                    *(uintptr_t*)vm->interpreted_cur = (uintptr_t)addr;
                    vm->interpreted_cur += sizeof(uintptr_t);
                    parser->state ^= PARSER_STATE_CREATE;
                    if (parser->pending) {
                        if (vm->interpreted_cur == vm->interpreted_end) {
                            parser->status = PARSER_STATUS_END;
                            ret_int = PARSER_STATUS_INTERPRETED_OVERFLOW;
                            break;
                        }
                        *(vm->interpreted_cur++) = VM_INSTRUCTION_CALL;
                        if (
                            vm->interpreted_cur + sizeof(uintptr_t) >=
                            vm->interpreted_end) {
                            parser->status = PARSER_STATUS_END;
                            ret_int =  PARSER_STATUS_INTERPRETED_OVERFLOW;
                            break;
                        }
                        *(uint8_t**)vm->interpreted_cur =
                            (uint8_t*)(parser->pending - 1);
                        vm->interpreted_cur += sizeof(uintptr_t);
                        if (vm->interpreted_cur == vm->interpreted_end) {
                            parser->status = PARSER_STATUS_END;
                            ret_int = PARSER_STATUS_INTERPRETED_OVERFLOW;
                            break;
                        }
                        if (parser->state & PARSER_STATE_DOES) {
                            if (!indirect) {
                                if (
                                    vm->lookup_cur + sizeof(uintptr_t) >=
                                    vm->lookup_end) {
                                    parser->status = PARSER_STATUS_END;
                                    return PARSER_STATUS_LOOKUP_OVERFLOW;
                                }
                                memmove(
                                    addr + 1,
                                    addr,
                                    vm->lookup_cur - (uint8_t*)addr
                                );
                                vm->lookup_cur += sizeof(uintptr_t);
                            }
                            *meta |=
                                VM_LOOKUP_META_CALL | VM_LOOKUP_META_INDIRECT;
                            next_function(&parser->pending);
                            *(uint8_t**)(addr + 1) = parser->pending;
                            parser->state ^= PARSER_STATE_DOES;
                        } else {
                            if (indirect) {
                                memmove(
                                    addr,
                                    addr + 1,
                                    vm->lookup_cur - (uint8_t*)(addr + 1)
                                );
                                vm->lookup_cur -= sizeof(uintptr_t);
                            }
                        }
                        parser->pending = 0;
                    } else {
                        if (indirect) {
                            memmove(
                                addr,
                                addr + 1,
                                vm->lookup_cur - (uint8_t*)(addr + 1)
                            );
                            vm->lookup_cur -= sizeof(uintptr_t);
                        }
                    }
                } else if (parser->state & PARSER_STATE_CHAR) {
                    if (parser->state & PARSER_STATE_INTERPRET) {
                        if (vm->interpreted_cur == vm->interpreted_end) {
                            parser->status = PARSER_STATUS_END;
                            ret_int = PARSER_STATUS_INTERPRETED_OVERFLOW;
                            break;
                        }
                        *(vm->interpreted_cur++) = VM_INSTRUCTION_PUSHD;
                        if (
                            vm->interpreted_cur + sizeof(uintptr_t) >=
                            vm->interpreted_end) {
                            parser->status = PARSER_STATUS_END;
                            ret_int =  PARSER_STATUS_INTERPRETED_OVERFLOW;
                            break;
                        }
                        *(uintptr_t*)vm->interpreted_cur =
                            *parser->buf.data;
                        vm->interpreted_cur += sizeof(uintptr_t);
                    } else {
                        if (vm->compiled_cur == vm->compiled_end) {
                            parser->status = PARSER_STATUS_END;
                            ret_int = PARSER_STATUS_COMPILED_OVERFLOW;
                            break;
                        }
                        *(vm->compiled_cur++) = VM_INSTRUCTION_PUSHD;
                        if (
                            vm->compiled_cur + sizeof(uintptr_t) >=
                            vm->compiled_end) {
                            parser->status = PARSER_STATUS_END;
                            ret_int =  PARSER_STATUS_COMPILED_OVERFLOW;
                            break;
                        }
                        *(uintptr_t*)vm->compiled_cur =
                            *parser->buf.data;
                        vm->compiled_cur += sizeof(uintptr_t);
                    }
                    parser->state ^= PARSER_STATE_CHAR;
                } else {
                    if (meta == parser->pending) {
                        parser->status = PARSER_STATUS_END;
                        parser->pending = 0;
                        parser->state = PARSER_STATE_INTERPRET;
                        ret_int = PARSER_STATUS_PENDING_DEFINITION;
                        break;
                    }
                    if (*meta & VM_LOOKUP_META_MEMORY) {
                        if (parser->state & PARSER_STATE_INTERPRET) {
                            if (vm->interpreted_cur == vm->interpreted_end) {
                                parser->status = PARSER_STATUS_END;
                                ret_int = PARSER_STATUS_INTERPRETED_OVERFLOW;
                                break;
                            }
                            *(vm->interpreted_cur++) = VM_INSTRUCTION_PUSHID;
                            if (
                                vm->interpreted_cur + sizeof(uintptr_t) >=
                                vm->interpreted_end) {
                                parser->status = PARSER_STATUS_END;
                                ret_int =  PARSER_STATUS_INTERPRETED_OVERFLOW;
                                break;
                            }
                            *(uint8_t**)vm->interpreted_cur = (uint8_t*)addr;
                            vm->interpreted_cur += sizeof(uintptr_t);
                            if (vm->interpreted_cur == vm->interpreted_end) {
                                parser->status = PARSER_STATUS_END;
                                ret_int =  PARSER_STATUS_INTERPRETED_OVERFLOW;
                                break;
                            }
                        }
                        if (parser->state & PARSER_STATE_COMPILE) {
                            if (vm->compiled_cur == vm->interpreted_end) {
                                parser->status = PARSER_STATUS_END;
                                ret_int =  PARSER_STATUS_COMPILED_OVERFLOW;
                                break;
                            }
                            *(vm->compiled_cur++) = VM_INSTRUCTION_PUSHID;
                            if (
                                vm->compiled_cur + sizeof(uintptr_t) >=
                                vm->compiled_end) {
                                parser->status = PARSER_STATUS_END;
                                ret_int =  PARSER_STATUS_COMPILED_OVERFLOW;
                                break;
                            }
                            *(uint8_t**)vm->compiled_cur = (uint8_t*)addr;
                            vm->compiled_cur += sizeof(uintptr_t);
                            if (vm->compiled_cur == vm->compiled_end) {
                                parser->status = PARSER_STATUS_END;
                                ret_int =  PARSER_STATUS_COMPILED_OVERFLOW;
                                break;
                            }
                        }
                    }
                    if (*meta & VM_LOOKUP_META_CALL) {
                        if (*meta & VM_LOOKUP_META_CREATE) {
                            if (!(parser->state & PARSER_STATE_INTERPRET)) {
                                parser->status = PARSER_STATUS_END;
                                ret_int = PARSER_STATUS_INTERPRETED_OVERFLOW;
                                break;
                            }
                            parser->state |= PARSER_STATE_CREATE;
                            if (vm->interpreted_cur == vm->interpreted_end) {
                                parser->status = PARSER_STATUS_END;
                                return PARSER_STATUS_INTERPRETED_OVERFLOW;
                            }
                            *(vm->interpreted_cur++) = VM_INSTRUCTION_DEF;
                            parser->pending = (uint8_t*)*addr;
                            if (*meta & VM_LOOKUP_META_DOES) {
                                parser->state |= PARSER_STATE_DOES;
                            }
                            continue;
                        }
                        if (parser->state & PARSER_STATE_INTERPRET) {
                            if (vm->interpreted_cur == vm->interpreted_end) {
                                parser->status = PARSER_STATUS_END;
                                ret_int = PARSER_STATUS_INTERPRETED_OVERFLOW;
                                break;
                            }
                            *(vm->interpreted_cur++) = VM_INSTRUCTION_CALL;
                            if (
                                vm->interpreted_cur + sizeof(uintptr_t) >=
                                vm->interpreted_end) {
                                parser->status = PARSER_STATUS_END;
                                ret_int =  PARSER_STATUS_INTERPRETED_OVERFLOW;
                                break;
                            }
                            if (*meta & VM_LOOKUP_META_INDIRECT) {
                                *(uint8_t**)vm->interpreted_cur =
                                    (uint8_t*)(addr[1] - 1);
                            } else {
                                *(uint8_t**)vm->interpreted_cur =
                                    (uint8_t*)(*addr - 1);
                            }
                            vm->interpreted_cur += sizeof(uintptr_t);
                            if (vm->interpreted_cur == vm->interpreted_end) {
                                parser->status = PARSER_STATUS_END;
                                ret_int = PARSER_STATUS_INTERPRETED_OVERFLOW;
                                break;
                            }
                        }
                        if (parser->state & PARSER_STATE_COMPILE) {
                            if (vm->compiled_cur == vm->interpreted_end) {
                                parser->status = PARSER_STATUS_END;
                                ret_int = PARSER_STATUS_COMPILED_OVERFLOW;
                                break;
                            }
                            *(vm->compiled_cur++) = VM_INSTRUCTION_CALL;
                            if (
                                vm->compiled_cur + sizeof(uintptr_t) >=
                                vm->compiled_end) {
                                parser->status = PARSER_STATUS_END;
                                ret_int = PARSER_STATUS_COMPILED_OVERFLOW;
                                break;
                            }
                            if (*meta & VM_LOOKUP_META_INDIRECT) {
                                *(uint8_t**)vm->compiled_cur =
                                    (uint8_t*)(addr[1] - 1);
                            } else {
                                *(uint8_t**)vm->compiled_cur =
                                    (uint8_t*)(*addr - 1);
                            }
                            vm->compiled_cur += sizeof(uintptr_t);
                            if (vm->compiled_cur == vm->compiled_end) {
                                parser->status = PARSER_STATUS_END;
                                ret_int = PARSER_STATUS_COMPILED_OVERFLOW;
                                break;
                            }
                        }
                    }
                }

            }
        } else {
            if (parser->state & PARSER_STATE_NAME) {
                if (vm->lookup_cur == vm->lookup_end) {
                    parser->status = PARSER_STATUS_END;
                    ret_int = PARSER_STATUS_LOOKUP_OVERFLOW;
                    break;
                }
                parser->pending = vm->lookup_cur;
                *(vm->lookup_cur++) = VM_LOOKUP_META_CALL;
                if (vm->lookup_cur + sizeof(uintptr_t) >= vm->lookup_end) {
                    parser->status = PARSER_STATUS_END;
                    ret_int = PARSER_STATUS_LOOKUP_OVERFLOW;
                    break;
                }
                *(uintptr_t*)vm->lookup_cur = (uintptr_t)vm->compiled_cur;
                vm->lookup_cur += sizeof(uintptr_t);
                for (char *iter_buf = parser->buf.data; *iter_buf; ++iter_buf) {
                    if (vm->lookup_cur == vm->lookup_end) {
                        parser->status = PARSER_STATUS_END;
                        ret_int = PARSER_STATUS_LOOKUP_OVERFLOW;
                        break;
                    }
                    *(vm->lookup_cur++) = *iter_buf;
                }
                if (vm->lookup_cur == vm->lookup_end) {
                    parser->status = PARSER_STATUS_END;
                    ret_int = PARSER_STATUS_LOOKUP_OVERFLOW;
                    break;
                }
                *(vm->lookup_cur++) = 0;
                parser->state ^= PARSER_STATE_NAME;
            } else if (parser->state & PARSER_STATE_CREATE) {
                if (vm->lookup_cur == vm->lookup_end) {
                    parser->status = PARSER_STATUS_END;
                    ret_int = PARSER_STATUS_LOOKUP_OVERFLOW;
                    break;
                }
                *(vm->lookup_cur++) = VM_LOOKUP_META_MEMORY;
                if (vm->lookup_cur + sizeof(uintptr_t) >= vm->lookup_end) {
                    parser->status = PARSER_STATUS_END;
                    ret_int = PARSER_STATUS_LOOKUP_OVERFLOW;
                    break;
                }
                if (
                    vm->interpreted_cur + sizeof(uintptr_t) >
                    vm->interpreted_end) {
                    parser->status = PARSER_STATUS_END;
                    ret_int = PARSER_STATUS_INTERPRETED_OVERFLOW;
                    break;
                }
                *(uint8_t**)vm->interpreted_cur = vm->lookup_cur;
                vm->interpreted_cur += sizeof(uintptr_t);
                vm->lookup_cur += sizeof(uintptr_t);
                parser->state ^= PARSER_STATE_CREATE;
                if (parser->pending) {
                    if (vm->interpreted_cur == vm->interpreted_end) {
                        parser->status = PARSER_STATUS_END;
                        ret_int = PARSER_STATUS_INTERPRETED_OVERFLOW;
                        break;
                    }
                    *(vm->interpreted_cur++) = VM_INSTRUCTION_CALL;
                    if (
                        vm->interpreted_cur + sizeof(uintptr_t) >=
                        vm->interpreted_end) {
                        parser->status = PARSER_STATUS_END;
                        ret_int =  PARSER_STATUS_INTERPRETED_OVERFLOW;
                        break;
                    }
                    *(uint8_t**)vm->interpreted_cur =
                        (uint8_t*)(parser->pending - 1);
                    vm->interpreted_cur += sizeof(uintptr_t);
                    if (vm->interpreted_cur == vm->interpreted_end) {
                        parser->status = PARSER_STATUS_END;
                        ret_int = PARSER_STATUS_INTERPRETED_OVERFLOW;
                        break;
                    }
                    if (parser->state & PARSER_STATE_DOES) {
                        vm->lookup_cur[-1 - sizeof(uintptr_t)] |=
                            VM_LOOKUP_META_CALL | VM_LOOKUP_META_INDIRECT;
                        if (
                            vm->lookup_cur + sizeof(uintptr_t) >=
                            vm->lookup_end) {
                            parser->status = PARSER_STATUS_END;
                            return PARSER_STATUS_LOOKUP_OVERFLOW;
                        }
                        next_function(&parser->pending);
                        *(uint8_t**)vm->lookup_cur = parser->pending;
                        vm->lookup_cur += sizeof(uintptr_t);
                        parser->state ^= PARSER_STATE_DOES;
                    }
                    parser->pending = 0;
                }
                for (char *iter_buf = parser->buf.data; *iter_buf; ++iter_buf) {
                    if (vm->lookup_cur == vm->lookup_end) {
                        parser->status = PARSER_STATUS_END;
                        ret_int = PARSER_STATUS_LOOKUP_OVERFLOW;
                        break;
                    }
                    *(vm->lookup_cur++) = *iter_buf;
                }
                if (vm->lookup_cur == vm->lookup_end) {
                    parser->status = PARSER_STATUS_END;
                    ret_int = PARSER_STATUS_LOOKUP_OVERFLOW;
                    break;
                }
                *(vm->lookup_cur++) = 0;
            } else if (parser->state & PARSER_STATE_CHAR) {
                if (parser->state & PARSER_STATE_INTERPRET) {
                    if (vm->interpreted_cur == vm->interpreted_end) {
                        parser->status = PARSER_STATUS_END;
                        ret_int = PARSER_STATUS_INTERPRETED_OVERFLOW;
                        break;
                    }
                    *(vm->interpreted_cur++) = VM_INSTRUCTION_PUSHD;
                    if (
                        vm->interpreted_cur + sizeof(uintptr_t) >=
                        vm->interpreted_end) {
                        parser->status = PARSER_STATUS_END;
                        ret_int =  PARSER_STATUS_INTERPRETED_OVERFLOW;
                        break;
                    }
                    *(uintptr_t*)vm->interpreted_cur =
                        *parser->buf.data;
                    vm->interpreted_cur += sizeof(uintptr_t);
                } else {
                    if (vm->compiled_cur == vm->compiled_end) {
                        parser->status = PARSER_STATUS_END;
                        ret_int = PARSER_STATUS_COMPILED_OVERFLOW;
                        break;
                    }
                    *(vm->compiled_cur++) = VM_INSTRUCTION_PUSHD;
                    if (
                        vm->compiled_cur + sizeof(uintptr_t) >=
                        vm->compiled_end) {
                        parser->status = PARSER_STATUS_END;
                        ret_int =  PARSER_STATUS_COMPILED_OVERFLOW;
                        break;
                    }
                    *(uintptr_t*)vm->compiled_cur =
                        *parser->buf.data;
                    vm->compiled_cur += sizeof(uintptr_t);
                }
                parser->state ^= PARSER_STATE_CHAR;
            } else {
                switch (*vm->memory) {
                case 10:
                    ret = parser_int10(parser->buf.data, &num);
                    if (!ret) {
                        ret_int = PARSER_STATUS_INVALID_BASE10;
                        break;
                    }
                    break;
                case 16:
                    ret = parser_int16(parser->buf.data, &num);
                    if (!ret) {
                        ret_int = PARSER_STATUS_INVALID_BASE16;
                        break;
                    }
                    break;
                default:
                    ret_int = PARSER_STATUS_INVALID_BASE;
                    break;
                }
                if (ret_int) {
                    break;
                }
                if (parser->state & PARSER_STATE_INTERPRET) {
                    if (vm->interpreted_cur == vm->interpreted_end) {
                        parser->status = PARSER_STATUS_END;
                        return PARSER_STATUS_INTERPRETED_OVERFLOW;
                    }
                    *(vm->interpreted_cur++) = VM_INSTRUCTION_PUSHD;
                    if (
                        vm->interpreted_cur + sizeof(uintptr_t) >
                        vm->interpreted_end) {
                        parser->status = PARSER_STATUS_END;
                        return PARSER_STATUS_INTERPRETED_OVERFLOW;
                    }
                    *(uintptr_t*)vm->interpreted_cur = num;
                    vm->interpreted_cur += sizeof(uintptr_t);
                }
                if (parser->state & PARSER_STATE_COMPILE) {
                    if (vm->compiled_cur == vm->compiled_end) {
                        parser->status = PARSER_STATUS_END;
                        return PARSER_STATUS_COMPILED_OVERFLOW;
                    }
                    *(vm->compiled_cur++) = VM_INSTRUCTION_PUSHD;
                    if (
                        vm->compiled_cur + sizeof(uintptr_t) >
                        vm->compiled_end) {
                        parser->status = PARSER_STATUS_END;
                        return PARSER_STATUS_COMPILED_OVERFLOW;
                    }
                    *(uintptr_t*)vm->compiled_cur = num;
                    vm->compiled_cur += sizeof(uintptr_t);
                }
            }
        }
        if (parser->status) {
            break;
        }
    }
    if (ret_int) {
        return ret_int;
    }
    if (parser->status == PARSER_STATUS_REPL_END) {
        return PARSER_STATUS_END_REPL;
    }
    if (vm->interpreted_cur == vm->interpreted_end) {
        return PARSER_STATUS_INTERPRETED_OVERFLOW;
    }
    *(vm->interpreted_cur++) = VM_INSTRUCTION_HALT;
    return PARSER_STATUS_OK;
}
