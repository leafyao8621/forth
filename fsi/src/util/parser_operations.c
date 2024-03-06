#include <fsi/util/parser_operations.h>

int get_token(ForthParser *parser) {
    int ret = 0;
    for (
        ;
        *parser->iter &&
        *parser->iter != ' ' &&
        *parser->iter != '\t' &&
        *parser->iter != '\n';
        ++parser->iter) {
        ret = DArrayChar_push_back(&parser->token_buf, parser->iter);
        if (ret) {
            return 1;
        }
    }
    char chr = 0;
    ret = DArrayChar_push_back(&parser->token_buf, &chr);
    if (ret) {
        return 1;
    }
    return 0;
}

void next_token(ForthParser *parser) {
    for (
        ;
        *parser->iter == ' ' ||
        *parser->iter == '\t' ||
        *parser->iter == '\n';
        ++parser->iter);
}

int get_literal(ForthParser *parser) {
    int ret = 0;
    for (
        ;
        *parser->iter &&
        *parser->iter != '"' &&
        *parser->iter != '\n';
        ++parser->iter) {
        ret = DArrayChar_push_back(&parser->token_buf, parser->iter);
        if (ret) {
            return 1;
        }
    }
    if (*parser->iter != '"') {
        return 2;
    }
    ++parser->iter;
    char chr = 0;
    ret = DArrayChar_push_back(&parser->token_buf, &chr);
    if (ret) {
        return 1;
    }
    return 0;
}

int get_handler_offset(ForthParser *parser, ForthVM *vm, size_t *offset) {
    const char *iter = vm->words.data;
    size_t *iter_offset = vm->offset.data;
    for (*offset = 0; *offset < 2; ++(*offset), ++iter, ++iter_offset) {
        if (!strcmp(iter, (const char*)parser->token_buf.data)) {
            return (*iter_offset & OFFSET_BUILTIN) == 0;
        }
        for (; *iter; ++iter);
    }
    return 2;
}
