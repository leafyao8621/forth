#ifndef UTIL_PARSER_UTIL_H_
#define UTIL_PARSER_UTIL_H_

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

bool parse_token(char *buf, char *buf_end, bool line, FILE *fin);
void next_token(bool line, FILE *fin);
bool lookup_token(char *buf, uint8_t **meta, uintptr_t **addr);
bool parser_int10(char *buf, uintptr_t *out);
bool parser_int16(char *buf, uintptr_t *out);

#endif
