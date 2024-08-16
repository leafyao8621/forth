#ifndef PARSER_PARSER_H_
#define PARSER_PARSER_H_

#include <stdio.h>
#include "src\util\stdbool.h"
#include "src\util\stdint.h"

#define PARSER_STATE_INTERPRET 0x1
#define PARSER_STATE_COMPILE 0x2
#define PARSER_STATE_NAME 0x4

#define PARSER_STATUS_RUNNING 0
#define PARSER_STATUS_END 1
#define PARSER_STATUS_REPL_END 2

#define PARSER_CONTROL_IF 0
#define PARSER_CONTROL_ELSE 1
#define PARSER_CONTROL_DO 2
#define PARSER_CONTROL_DO_LEAVE 3
#define PARSER_CONTROL_BEGIN 4
#define PARSER_CONTROL_WHILE 5

extern uint8_t parser_state;
extern uint8_t parser_status;

extern uint8_t *parser_pending;

extern uint8_t parser_conditional_stack[(sizeof(uintptr_t) + 1) * 20];
extern uint8_t *parser_conditional_stack_cur;
extern uint8_t *parser_conditional_stack_end;

extern uint8_t parser_loop_stack[(sizeof(uintptr_t) + 1) * 20];
extern uint8_t *parser_loop_stack_cur;
extern uint8_t *parser_loop_stack_end;

extern bool parser_eos;

void parser_initialize(void);
int parser_parse(bool debug, bool line, FILE *fin);

#endif
