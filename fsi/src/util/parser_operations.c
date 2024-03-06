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
