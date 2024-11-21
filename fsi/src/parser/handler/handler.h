#ifndef PARSER_HANDLER_H_
#define PARSER_HANDLER_H_

#include <fsi/parser/parser.h>
#include <fsi/vm/vm.h>

#define PARSER_HANDLER_DOT 0
#define PARSER_HANDLER_EMIT 1
#define PARSER_HANDLER_CR 2
#define PARSER_HANDLER_COLON 3
#define PARSER_HANDLER_SEMI_COLON 4
#define PARSER_HANDLER_IF 5
#define PARSER_HANDLER_THEN 6
#define PARSER_HANDLER_ELSE 7
#define PARSER_HANDLER_CASE 8
#define PARSER_HANDLER_OF 9
#define PARSER_HANDLER_ENDOF 10
#define PARSER_HANDLER_ENDCASE 11
#define PARSER_HANDLER_DO 12
#define PARSER_HANDLER_LOOP 13
#define PARSER_HANDLER_PLUS_LOOP 14
#define PARSER_HANDLER_I 15
#define PARSER_HANDLER_LEAVE 16
#define PARSER_HANDLER_UNLOOP 17
#define PARSER_HANDLER_EXIT 18
#define PARSER_HANDLER_J 19
#define PARSER_HANDLER_BEGIN 20
#define PARSER_HANDLER_WHILE 21
#define PARSER_HANDLER_REPEAT 22
#define PARSER_HANDLER_UNTIL 23
#define PARSER_HANDLER_AGAIN 24
#define PARSER_HANDLER_CREATE 25
#define PARSER_HANDLER_VARIABLE 26
#define PARSER_HANDLER_ALLOT 27
#define PARSER_HANDLER_CELLS 28
#define PARSER_HANDLER_DOES_GT 29
#define PARSER_HANDLER_AT 30
#define PARSER_HANDLER_BANG 31
#define PARSER_HANDLER_CAT 32
#define PARSER_HANDLER_CBANG 33
#define PARSER_HANDLER_COMMA 34
#define PARSER_HANDLER_CCOMMA 35
#define PARSER_HANDLER_PLUS 36
#define PARSER_HANDLER_DASH 37
#define PARSER_HANDLER_ASTERISK 38
#define PARSER_HANDLER_SLASH 39
#define PARSER_HANDLER_MOD 40
#define PARSER_HANDLER_ONE_PLUS 41
#define PARSER_HANDLER_CELL_PLUS 42
#define PARSER_HANDLER_ONE_DASH 43
#define PARSER_HANDLER_TWO_ASTERISK 44
#define PARSER_HANDLER_TWO_SLASH 45
#define PARSER_HANDLER_LSFHIFT 46
#define PARSER_HANDLER_RSFHIFT 47
#define PARSER_HANDLER_AND 48
#define PARSER_HANDLER_OR 49
#define PARSER_HANDLER_XOR 50
#define PARSER_HANDLER_INVERT 51
#define PARSER_HANDLER_NEGATE 52
#define PARSER_HANDLER_GT 53
#define PARSER_HANDLER_LT 54
#define PARSER_HANDLER_EQ 55
#define PARSER_HANDLER_ULT 56
#define PARSER_HANDLER_ZERO_LT 57
#define PARSER_HANDLER_ZERO_EQ 58
#define PARSER_HANDLER_DUP 59
#define PARSER_HANDLER_DROP 60
#define PARSER_HANDLER_SWAP 61
#define PARSER_HANDLER_OVER 62
#define PARSER_HANDLER_NIP 63
#define PARSER_HANDLER_TWO_DUP 64
#define PARSER_HANDLER_TWO_DROP 65
#define PARSER_HANDLER_TWO_SWAP 66
#define PARSER_HANDLER_TWO_OVER 67


int parser_handler_dot(ForthParser *parser, ForthVM *vm);
int parser_handler_emit(ForthParser *parser, ForthVM *vm);
int parser_handler_cr(ForthParser *parser, ForthVM *vm);
int parser_handler_colon(ForthParser *parser);
int parser_handler_semi_colon(ForthParser *parser, ForthVM *vm);
int parser_handler_if(ForthParser *parser, ForthVM *vm);
int parser_handler_then(ForthParser *parser, ForthVM *vm);
int parser_handler_else(ForthParser *parser, ForthVM *vm);
int parser_handler_case(ForthParser *parser, ForthVM *vm);
int parser_handler_of(ForthParser *parser, ForthVM *vm);
int parser_handler_endof(ForthParser *parser, ForthVM *vm);
int parser_handler_endcase(ForthParser *parser, ForthVM *vm);
int parser_handler_do(ForthParser *parser, ForthVM *vm);
int parser_handler_loop(ForthParser *parser, ForthVM *vm);
int parser_handler_plus_loop(ForthParser *parser, ForthVM *vm);
int parser_handler_i(ForthParser *parser, ForthVM *vm);
int parser_handler_leave(ForthParser *parser, ForthVM *vm);
int parser_handler_unloop(ForthParser *parser, ForthVM *vm);
int parser_handler_exit(ForthParser *parser, ForthVM *vm);
int parser_handler_j(ForthParser *parser, ForthVM *vm);
int parser_handler_begin(ForthParser *parser, ForthVM *vm);
int parser_handler_while(ForthParser *parser, ForthVM *vm);
int parser_handler_repeat(ForthParser *parser, ForthVM *vm);
int parser_handler_until(ForthParser *parser, ForthVM *vm);
int parser_handler_again(ForthParser *parser, ForthVM *vm);
int parser_handler_create(ForthParser *parser, ForthVM *vm);
int parser_handler_variable(ForthParser *parser, ForthVM *vm);
int parser_handler_allot(ForthParser *parser, ForthVM *vm);
int parser_handler_cells(ForthParser *parser, ForthVM *vm);
int parser_handler_does_gt(ForthParser *parser, ForthVM *vm);
int parser_handler_bang(ForthParser *parser, ForthVM *vm);
int parser_handler_at(ForthParser *parser, ForthVM *vm);
int parser_handler_cat(ForthParser *parser, ForthVM *vm);
int parser_handler_cbang(ForthParser *parser, ForthVM *vm);
int parser_handler_comma(ForthParser *parser, ForthVM *vm);
int parser_handler_ccomma(ForthParser *parser, ForthVM *vm);
int parser_handler_plus(ForthParser *parser, ForthVM *vm);
int parser_handler_dash(ForthParser *parser, ForthVM *vm);
int parser_handler_asterisk(ForthParser *parser, ForthVM *vm);
int parser_handler_slash(ForthParser *parser, ForthVM *vm);
int parser_handler_mod(ForthParser *parser, ForthVM *vm);
int parser_handler_one_plus(ForthParser *parser, ForthVM *vm);
int parser_handler_cell_plus(ForthParser *parser, ForthVM *vm);
int parser_handler_one_dash(ForthParser *parser, ForthVM *vm);
int parser_handler_two_asterisk(ForthParser *parser, ForthVM *vm);
int parser_handler_two_slash(ForthParser *parser, ForthVM *vm);
int parser_handler_lshift(ForthParser *parser, ForthVM *vm);
int parser_handler_rshift(ForthParser *parser, ForthVM *vm);
int parser_handler_and(ForthParser *parser, ForthVM *vm);
int parser_handler_or(ForthParser *parser, ForthVM *vm);
int parser_handler_xor(ForthParser *parser, ForthVM *vm);
int parser_handler_invert(ForthParser *parser, ForthVM *vm);
int parser_handler_negate(ForthParser *parser, ForthVM *vm);
int parser_handler_gt(ForthParser *parser, ForthVM *vm);
int parser_handler_lt(ForthParser *parser, ForthVM *vm);
int parser_handler_eq(ForthParser *parser, ForthVM *vm);
int parser_handler_ult(ForthParser *parser, ForthVM *vm);
int parser_handler_zero_lt(ForthParser *parser, ForthVM *vm);
int parser_handler_zero_eq(ForthParser *parser, ForthVM *vm);
int parser_handler_dup(ForthParser *parser, ForthVM *vm);
int parser_handler_drop(ForthParser *parser, ForthVM *vm);
int parser_handler_swap(ForthParser *parser, ForthVM *vm);
int parser_handler_over(ForthParser *parser, ForthVM *vm);
int parser_handler_nip(ForthParser *parser, ForthVM *vm);
int parser_handler_two_dup(ForthParser *parser, ForthVM *vm);
int parser_handler_two_drop(ForthParser *parser, ForthVM *vm);
int parser_handler_two_swap(ForthParser *parser, ForthVM *vm);
int parser_handler_two_over(ForthParser *parser, ForthVM *vm);

#endif
