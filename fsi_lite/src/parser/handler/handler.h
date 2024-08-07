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
#define PARSER_HANDLER_MOD 25
#define PARSER_HANDLER_ONE_PLUS 26
#define PARSER_HANDLER_ONE_DASH 27
#define PARSER_HANDLER_TWO_ASTERISK 28
#define PARSER_HANDLER_TWO_SLASH 29
#define PARSER_HANDLER_LSFHIFT 30
#define PARSER_HANDLER_RSFHIFT 31
#define PARSER_HANDLER_AND 32
#define PARSER_HANDLER_OR 33
#define PARSER_HANDLER_XOR 34
#define PARSER_HANDLER_INVERT 35
#define PARSER_HANDLER_NEGATE 36
#define PARSER_HANDLER_GT 37
#define PARSER_HANDLER_LT 38
#define PARSER_HANDLER_EQ 39
#define PARSER_HANDLER_ULT 40
#define PARSER_HANDLER_ZERO_LT 41
#define PARSER_HANDLER_ZERO_EQ 42
#define PARSER_HANDLER_DUP 43
#define PARSER_HANDLER_DROP 44
#define PARSER_HANDLER_SWAP 45


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
int parser_handler_plus(void);
int parser_handler_dash(void);
int parser_handler_asterisk(void);
int parser_handler_slash(void);
int parser_handler_mod(void);
int parser_handler_one_plus(void);
int parser_handler_one_dash(void);
int parser_handler_two_asterisk(void);
int parser_handler_two_slash(void);
int parser_handler_lshift(void);
int parser_handler_rshift(void);
int parser_handler_and(void);
int parser_handler_or(void);
int parser_handler_xor(void);
int parser_handler_invert(void);
int parser_handler_negate(void);
int parser_handler_gt(void);
int parser_handler_lt(void);
int parser_handler_eq(void);
int parser_handler_ult(void);
int parser_handler_zero_lt(void);
int parser_handler_zero_eq(void);
int parser_handler_dup(void);
int parser_handler_drop(void);
int parser_handler_swap(void);

#endif
