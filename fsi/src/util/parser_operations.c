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
    for (
        *offset = 0;
        *offset < vm->offset.size;
        ++(*offset), ++iter, ++iter_offset) {
        if (!strcmp(iter, (const char*)parser->token_buf.data)) {
            return (*iter_offset & OFFSET_BUILTIN) == 0;
        }
        for (; *iter; ++iter);
    }
    return 2;
}

static int get_int_decimal(char *str, size_t *out) {
    bool minus = false;
    char *iter = str;
    *out = 0;
    for (
        size_t i = 0;
        *iter &&
        (
            (*iter >= '0' && *iter <= '9') ||
            *iter == '-'
        );
        ++i, ++iter) {
        if (*iter == '-') {
            if (minus || i) {
                break;
            } else {
                minus = true;
            }
        } else {
            *out *= 10;
            *out += *iter - '0';
        }
    }
    if (*iter) {
        return 1;
    }
    if (minus) {
        *out = -*out;
    }
    return 0;
}

int get_int(ForthParser *parser, size_t *out) {
    if (!parser || !out) {
        return FORTHVM_ERR_NULL_PTR;
    }
    int ret = 0;
    switch (parser->base) {
    case 10:
        ret = get_int_decimal(parser->token_buf.data, out);
        if (ret) {
            return 2;
        }
        break;
    default:
        return 1;
    }
    return 0;
}
