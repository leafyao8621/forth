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
    static ForthParserHandler parser_handlers[3] =
        {
            parser_handle_print_string,
            parser_handle_carriage_return,
            parser_handle_print_int
        };
    parser->str = str;
    parser->iter = str;
    parser->compile = false;
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

        } else {
            ret = get_int(parser, &int_value);
            switch (ret) {
            case 1:
                return FORTHVM_ERR_INVALID_BASE;
            case 2:
                return FORTHVM_ERR_INVALID_DECIMAL;
            }
            char chr = OPCODE_PUSH;
            if (!parser->compile) {
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
            }
        }
        next_token(parser);
    }
    return FORTHVM_ERR_OK;
}
