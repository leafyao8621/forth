#ifndef UTIL_PARSER_UTIL_H_
#define UTIL_PARSER_UTIL_H_

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

int parse_token(char *buf, char *buf_end, bool line, FILE *fin);
int next_token(bool line, FILE *fin);

#endif
