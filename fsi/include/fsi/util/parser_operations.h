#ifndef UTIL_PARSER_OPERATIONS_H_
#define UTIL_PARSER_OPERATIONS_H_

#include <fsi/parser/parser.h>

int get_token(ForthParser *parser);
void next_token(ForthParser *parser);
int get_handler_offset(ForthParser *parser, ForthVM *vm, size_t *offset);
int get_literal(ForthParser *parser);

#endif
