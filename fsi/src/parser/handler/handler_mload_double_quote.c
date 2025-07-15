#include <fsi/util/status.h>

#include "handler.h"

int parser_handler_mload_double_quote(
    ForthParser *parser, char **iter, ForthVM *vm) {
    if (!vm) {
        return PARSER_STATUS_OK;
    }
    if (parser->state & PARSER_STATE_COMPILE) {
        parser->status = PARSER_STATUS_END;
        return PARSER_STATUS_NOT_IN_INTERPRETATION_MODE;
    }
    DArrayChar_clear(&parser->buf);
    for (++(*iter); **iter && **iter != '"'; ++(*iter)) {
        if (!**iter) {
            parser->status = PARSER_STATUS_END;
            return PARSER_STATUS_INCOMPLETE_STRING;
        }
        if (DArrayChar_push_back(&parser->buf, *iter)) {
            parser->status = PARSER_STATUS_END;
            return PARSER_STATUS_MODULE_NAME_OVERFLOW;
        }
    }
    char zero = 0;
    if (DArrayChar_push_back(&parser->buf, &zero)) {
        parser->status = PARSER_STATUS_END;
        return PARSER_STATUS_MODULE_NAME_OVERFLOW;
    }
    ++(*iter);
    printf("Loading %s\n", parser->buf.data);
    return PARSER_STATUS_OK;
}
