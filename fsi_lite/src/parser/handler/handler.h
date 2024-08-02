#ifndef PARSER_HANDLER_H_
#define PARSER_HANDLER_H_

#define PARSER_HANDLER_DOT 0
#define PARSER_HANDLER_EMIT 1
#define PARSER_HANDLER_CR 2
#define PARSER_HANDLER_COLON 3
#define PARSER_HANDLER_SEMI_COLON 4
#define PARSER_HANDLER_IF 5
#define PARSER_HANDLER_THEN 6
#define PARSER_HANDLER_ELSE 7
#define PARSER_HANDLER_DO 8
#define PARSER_HANDLER_LOOP 9
#define PARSER_HANDLER_PLUS_LOOP 10
#define PARSER_HANDLER_I 11
#define PARSER_HANDLER_LEAVE 12
#define PARSER_HANDLER_UNLOOP 13
#define PARSER_HANDLER_EXIT 14
#define PARSER_HANDLER_J 15
#define PARSER_HANDLER_BEGIN 16
#define PARSER_HANDLER_WHILE 17
#define PARSER_HANDLER_REPEAT 18
#define PARSER_HANDLER_UNTIL 19
#define PARSER_HANDLER_AGAIN 20
#define PARSER_HANDLER_PLUS 21
#define PARSER_HANDLER_DASH 22
#define PARSER_HANDLER_ASTERISK 23
#define PARSER_HANDLER_SLASH 24
#define PARSER_HANDLER_ONE_PLUS 25
#define PARSER_HANDLER_ONE_DASH 26
#define PARSER_HANDLER_TWO_ASTERISK 27
#define PARSER_HANDLER_TWO_SLASH 28
#define PARSER_HANDLER_LSFHIFT 29
#define PARSER_HANDLER_RSFHIFT 30
#define PARSER_HANDLER_AND 31
#define PARSER_HANDLER_OR 32
#define PARSER_HANDLER_XOR 33
#define PARSER_HANDLER_NEGATE 34
#define PARSER_HANDLER_GT 35
#define PARSER_HANDLER_LT 36
#define PARSER_HANDLER_EQ 37
#define PARSER_HANDLER_ULT 38
#define PARSER_HANDLER_ZERO_LT 39
#define PARSER_HANDLER_ZERO_EQ 40
#define PARSER_HANDLER_ZERO_DUP 41
#define PARSER_HANDLER_ZERO_DROP 42

int parser_handler_dot(void);
int parser_handler_emit(void);
int parser_handler_cr(void);
int parser_handler_colon(void);
int parser_handler_semi_colon(void);
int parser_handler_if(void);
int parser_handler_then(void);
int parser_handler_else(void);
int parser_handler_do(void);
int parser_handler_loop(void);
int parser_handler_plus_loop(void);
int parser_handler_i(void);
int parser_handler_leave(void);
int parser_handler_unloop(void);
int parser_handler_exit(void);
int parser_handler_j(void);

#endif
