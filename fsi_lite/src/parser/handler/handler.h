#ifndef PARSER_HANDLER_H_
#define PARSER_HANDLER_H_

#define PARSER_HANDLER_DOT 0
#define PARSER_HANDLER_EMIT 1
#define PARSER_HANDLER_CR 2
#define PARSER_HANDLER_COLON 3
#define PARSER_HANDLER_SEMI_COLON 4
#define PARSER_HANDLER_SEMI_IF 5
#define PARSER_HANDLER_SEMI_THEN 6
#define PARSER_HANDLER_SEMI_ELSE 7

int parser_handler_dot(void);
int parser_handler_emit(void);
int parser_handler_cr(void);
int parser_handler_colon(void);
int parser_handler_semi_colon(void);
int parser_handler_if(void);
int parser_handler_then(void);
int parser_handler_else(void);

#endif
