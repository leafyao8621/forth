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
    return FORTHVM_ERR_OK;
}

void ForthParser_finalize(ForthParser *parser) {
    DArrayChar_finalize(&parser->token_buf);
}

ForthVMErr ForthParser_parse(ForthParser *parser, char *str, ForthVM *vm) {
    if (!parser || !str || !vm) {
        return FORTHVM_ERR_NULL_PTR;
    }
    static ForthParserHandler parser_handlers[5] =
        {
            parser_handle_print_string,
            parser_handle_carriage_return,
            parser_handle_print_int,
            parser_handle_start_compilation,
            parser_handle_end_compilation
        };
    parser->str = str;
    parser->iter = str;
    parser->state = FORTHPARSER_STATE_INTERPRET;
    parser->base = 10;
    int ret = 0;
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
            parser_handlers[handler_offset](parser, vm);
        } else if (ret == 1) {
            char opcode = 0;
            size_t offset = 0;
            switch (parser->state) {
            case FORTHPARSER_STATE_INTERPRET:
                offset = vm->offset.data[handler_offset];
                if (!(offset & OFFSET_MEMORY)) {
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
                }
                break;
            case FORTHPARSER_STATE_COMPILE:
                offset = vm->offset.data[handler_offset];
                if (!(offset & OFFSET_MEMORY)) {
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
                }
                break;
            case FORTHPARSER_STATE_DEFINE:
                vm->offset.data[handler_offset] = vm->compiled.size;
                break;
            }
        } else {
            size_t offset = 0;
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
                            &vm->interpreted, (char*)&int_value, sizeof(size_t));
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
                offset = vm->compiled.size;
                ret = DArrayOffset_push_back(&vm->offset, &offset);
                if (ret) {
                    return FORTHVM_ERR_OUT_OF_MEMORY;
                }
                parser->state = FORTHPARSER_STATE_COMPILE;
                break;
            }
        }
        next_token(parser);
    }
    return FORTHVM_ERR_OK;
}