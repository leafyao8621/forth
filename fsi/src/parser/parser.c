#include <stdio.h>

#include <fsi/parser/parser.h>
#include <fsi/util/parser_operations.h>

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
    parser->str = str;
    parser->iter = str;
    int ret = 0;
    for (; *parser->iter; ++parser->iter) {
        DArrayChar_clear(&parser->token_buf);
        ret = get_token(parser);
        if (ret) {
            return FORTHVM_ERR_OUT_OF_MEMORY;
        }
        puts(parser->token_buf.data);
    }
    return FORTHVM_ERR_OK;
}
