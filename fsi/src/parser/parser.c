#include <stdio.h>

#include <fsi/parser/parser.h>
#include <fsi/util/parser_operations.h>
#include <fsi/util/parser_handlers.h>

ForthVMErr ForthParser_initialize(ForthParser *parser) {
    if (!parser) {
        return FORTHVM_ERR_NULL_PTR;
    }
    int ret = DArrayChar_initialize(&parser->token_buf, 20);
    if (ret) {
        return FORTHVM_ERR_OUT_OF_MEMORY;
    }
    ret = DArrayOffset_initialize(&parser->conditional_offset, 1);
    if (ret) {
        return FORTHVM_ERR_OUT_OF_MEMORY;
    }
    ret = DArrayChar_initialize(&parser->conditional_type, 1);
    if (ret) {
        return FORTHVM_ERR_OUT_OF_MEMORY;
    }
    ret = DArrayOffset_initialize(&parser->loop_offset, 1);
    if (ret) {
        return FORTHVM_ERR_OUT_OF_MEMORY;
    }
    ret = DArrayChar_initialize(&parser->loop_type, 1);
    if (ret) {
        return FORTHVM_ERR_OUT_OF_MEMORY;
    }
    return FORTHVM_ERR_OK;
}

void ForthParser_finalize(ForthParser *parser) {
    DArrayChar_finalize(&parser->token_buf);
    DArrayOffset_finalize(&parser->conditional_offset);
    DArrayChar_finalize(&parser->conditional_type);
    DArrayOffset_finalize(&parser->loop_offset);
    DArrayChar_finalize(&parser->loop_type);
}

ForthVMErr ForthParser_parse(ForthParser *parser, char *str, ForthVM *vm) {
    if (!parser || !str || !vm) {
        return FORTHVM_ERR_NULL_PTR;
    }
    static ForthParserHandler parser_handlers[] =
        {
            parser_handle_print_string,
            parser_handle_carriage_return,
            parser_handle_print_int,
            parser_handle_print_int_unsigned,
            parser_handle_start_compilation,
            parser_handle_end_compilation,
            parser_handle_add,
            parser_handle_subtract,
            parser_handle_multiply,
            parser_handle_divide,
            parser_handle_increment,
            parser_handle_decrement,
            parser_handle_gt,
            parser_handle_ugt,
            parser_handle_geq,
            parser_handle_ugeq,
            parser_handle_lt,
            parser_handle_ult,
            parser_handle_leq,
            parser_handle_uleq,
            parser_handle_eq,
            parser_handle_neq,
            parser_handle_if,
            parser_handle_else,
            parser_handle_then,
            parser_handle_dup,
            parser_handle_swap,
            parser_handle_drop,
            parser_handle_mod,
            parser_handle_do,
            parser_handle_loop,
            parser_handle_i,
            parser_handle_begin,
            parser_handle_while,
            parser_handle_repeat,
            parser_handle_variable,
            parser_handle_load,
            parser_handle_store,
            parser_handle_create,
            parser_handle_allot,
            parser_handle_cells,
            parser_handle_loadc,
            parser_handle_storec,
            parser_handle_pushm,
            parser_handle_pushmc,
            NULL
        };
    parser->str = str;
    parser->iter = str;
    parser->state = FORTHPARSER_STATE_INTERPRET;
    parser->base = 10;
    parser->offset = 0;
    int ret = 0;
    ForthVMErr err = FORTHVM_ERR_OK;
    size_t handler_offset = 0;
    size_t int_value = 0;
    for (; *parser->iter;) {
        DArrayChar_clear(&parser->token_buf);
        ret = get_token(parser);
        if (ret) {
            return FORTHVM_ERR_OUT_OF_MEMORY;
        }
        printf("token: %s\n", parser->token_buf.data);
        ret = get_handler_offset(parser, vm, &handler_offset);
        if (!ret) {
            err = parser_handlers[handler_offset](parser, vm);
            if (err) {
                return err;
            }
        } else if (ret == 1) {
            char opcode = 0;
            size_t offset = 0;
            char flags = 0;
            switch (parser->state) {
            case FORTHPARSER_STATE_INTERPRET:
                flags = vm->offset_flags.data[handler_offset];
                if (!(flags & OFFSET_MEMORY)) {
                    if (flags & OFFSET_PENDING) {
                        return FORTHVM_ERR_PENDING_DEFINITION;
                    }
                    offset =
                        (vm->offset.data[handler_offset] - 1) & ~IP_COMPILED;
                    opcode = OPCODE_CALL;
                    ret = DArrayChar_push_back(&vm->interpreted, &opcode);
                    if (ret) {
                        return FORTHVM_ERR_OUT_OF_MEMORY;
                    }
                    ret =
                        DArrayChar_push_back_batch(
                            &vm->interpreted,
                            (char*)&offset,
                            sizeof(size_t)
                        );
                    if (ret) {
                        return FORTHVM_ERR_OUT_OF_MEMORY;
                    }
                } else {
                    opcode = OPCODE_PUSHW;
                    ret = DArrayChar_push_back(&vm->interpreted, &opcode);
                    if (ret) {
                        return FORTHVM_ERR_OUT_OF_MEMORY;
                    }
                    ret =
                        DArrayChar_push_back_batch(
                            &vm->interpreted,
                            (char*)&handler_offset,
                            sizeof(size_t)
                        );
                    if (ret) {
                        return FORTHVM_ERR_OUT_OF_MEMORY;
                    }
                }
                break;
            case FORTHPARSER_STATE_COMPILE:
                flags = vm->offset_flags.data[handler_offset];
                if (!(flags & OFFSET_MEMORY)) {
                    if (flags & OFFSET_PENDING) {
                        return FORTHVM_ERR_PENDING_DEFINITION;
                    }
                    offset =
                        (vm->offset.data[handler_offset] - 1) & ~IP_COMPILED;
                    opcode = OPCODE_CALL;
                    ret = DArrayChar_push_back(&vm->compiled, &opcode);
                    if (ret) {
                        return FORTHVM_ERR_OUT_OF_MEMORY;
                    }
                    ret =
                        DArrayChar_push_back_batch(
                            &vm->compiled,
                            (char*)&offset,
                            sizeof(size_t)
                        );
                    if (ret) {
                        return FORTHVM_ERR_OUT_OF_MEMORY;
                    }
                } else {
                    opcode = OPCODE_PUSHW;
                    ret = DArrayChar_push_back(&vm->compiled, &opcode);
                    if (ret) {
                        return FORTHVM_ERR_OUT_OF_MEMORY;
                    }
                    ret =
                        DArrayChar_push_back_batch(
                            &vm->compiled,
                            (char*)&handler_offset,
                            sizeof(size_t)
                        );
                    if (ret) {
                        return FORTHVM_ERR_OUT_OF_MEMORY;
                    }
                }
                break;
            case FORTHPARSER_STATE_DEFINE:
                parser->offset = handler_offset;
                vm->offset.data[handler_offset] = vm->compiled.size;
                vm->offset_flags.data[handler_offset] = OFFSET_PENDING;
                parser->state = FORTHPARSER_STATE_COMPILE;
                break;
            case FORTHPARSER_STATE_VARIABLE:
            case FORTHPARSER_STATE_CREATE:
                vm->offset_flags.data[handler_offset] = OFFSET_MEMORY;
                opcode = OPCODE_DW;
                ret = DArrayChar_push_back(&vm->interpreted, &opcode);
                if (ret) {
                    return FORTHVM_ERR_OUT_OF_MEMORY;
                }
                ret =
                    DArrayChar_push_back_batch(
                        &vm->interpreted,
                        (char*)&handler_offset,
                        sizeof(size_t));
                if (ret) {
                    return FORTHVM_ERR_OUT_OF_MEMORY;
                }
                if (parser->state == FORTHPARSER_STATE_VARIABLE) {
                    opcode = OPCODE_ALLOTC;
                    ret = DArrayChar_push_back(&vm->interpreted, &opcode);
                    if (ret) {
                        return FORTHVM_ERR_OUT_OF_MEMORY;
                    }
                }
                parser->state = FORTHPARSER_STATE_INTERPRET;
                break;
            }
        } else {
            size_t offset = 0;
            char flags = 0;
            char opcode = OPCODE_DW;
            switch (parser->state) {
            case FORTHPARSER_STATE_INTERPRET:
            case FORTHPARSER_STATE_COMPILE:
                ret = get_int(parser, &int_value);
                switch (ret) {
                case 1:
                    return FORTHVM_ERR_INVALID_BASE;
                case 2:
                    return FORTHVM_ERR_INVALID_DECIMAL;
                }
                char chr = OPCODE_PUSH;
                switch (parser->state) {
                case FORTHPARSER_STATE_INTERPRET:
                    ret = DArrayChar_push_back(&vm->interpreted, &chr);
                    if (ret) {
                        return FORTHVM_ERR_OUT_OF_MEMORY;
                    }
                    ret =
                        DArrayChar_push_back_batch(
                            &vm->interpreted,
                            (char*)&int_value,
                            sizeof(size_t));
                    if (ret) {
                        return FORTHVM_ERR_OUT_OF_MEMORY;
                    }
                    break;
                case FORTHPARSER_STATE_COMPILE:
                    ret = DArrayChar_push_back(&vm->compiled, &chr);
                    if (ret) {
                        return FORTHVM_ERR_OUT_OF_MEMORY;
                    }
                    ret =
                        DArrayChar_push_back_batch(
                            &vm->compiled, (char*)&int_value, sizeof(size_t));
                    if (ret) {
                        return FORTHVM_ERR_OUT_OF_MEMORY;
                    }
                    break;
                }
                break;
            case FORTHPARSER_STATE_DEFINE:
                ret =
                    DArrayChar_push_back_batch(
                        &vm->words,
                        parser->token_buf.data,
                        parser->token_buf.size
                    );
                if (ret) {
                    return FORTHVM_ERR_OUT_OF_MEMORY;
                }
                parser->offset = vm->offset.size;
                offset = vm->compiled.size;
                ret = DArrayOffset_push_back(&vm->offset, &offset);
                if (ret) {
                    return FORTHVM_ERR_OUT_OF_MEMORY;
                }
                flags = OFFSET_PENDING;
                ret = DArrayChar_push_back(&vm->offset_flags, &flags);
                if (ret) {
                    return FORTHVM_ERR_OUT_OF_MEMORY;
                }
                parser->state = FORTHPARSER_STATE_COMPILE;
                break;
            case FORTHPARSER_STATE_VARIABLE:
            case FORTHPARSER_STATE_CREATE:
                ret =
                    DArrayChar_push_back_batch(
                        &vm->words,
                        parser->token_buf.data,
                        parser->token_buf.size
                    );
                if (ret) {
                    return FORTHVM_ERR_OUT_OF_MEMORY;
                }
                offset = 0;
                ret = DArrayOffset_push_back(&vm->offset, &offset);
                if (ret) {
                    return FORTHVM_ERR_OUT_OF_MEMORY;
                }
                flags = OFFSET_MEMORY;
                ret = DArrayChar_push_back(&vm->offset_flags, &flags);
                if (ret) {
                    return FORTHVM_ERR_OUT_OF_MEMORY;
                }
                ret = DArrayChar_push_back(&vm->interpreted, &opcode);
                if (ret) {
                    return FORTHVM_ERR_OUT_OF_MEMORY;
                }
                ret =
                    DArrayChar_push_back_batch(
                        &vm->interpreted,
                        (char*)&handler_offset,
                        sizeof(size_t));
                if (ret) {
                    return FORTHVM_ERR_OUT_OF_MEMORY;
                }
                if (parser->state == FORTHPARSER_STATE_VARIABLE) {
                    opcode = OPCODE_ALLOTC;
                    ret = DArrayChar_push_back(&vm->interpreted, &opcode);
                    if (ret) {
                        return FORTHVM_ERR_OUT_OF_MEMORY;
                    }
                }
                parser->state = FORTHPARSER_STATE_INTERPRET;
                break;
            }
        }
        next_token(parser);
    }
    return FORTHVM_ERR_OK;
}
