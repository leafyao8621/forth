#ifndef PARSER_PARSER_H_
#define PARSER_PARSER_H_

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

void parser_initialize(void);
int parser_parse(bool line, FILE *fin);

#endif
