#ifndef UTIL_PARSER_UTIL_H_
#define UTIL_PARSER_UTIL_H_

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#include <fsi/parser/parser.h>
#include <fsi/vm/vm.h>

bool read_file(FILE *fin, String *buf);
int get_input(String *buf);
bool parse_token(ForthParser *parser, bool line, char **str);
void next_token(ForthParser *parser, bool line, char **str);
bool lookup_token(ForthVM *vm, char *buf, uint8_t **meta, uintptr_t **addr);
bool parser_int10(char *buf, uintptr_t *out);
bool parser_int16(char *buf, uintptr_t *out);
void next_function(uint8_t **addr);

#endif
