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

#endif
