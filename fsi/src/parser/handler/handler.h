#ifndef PARSER_HANDLER_H_
#define PARSER_HANDLER_H_

#include <fsi/parser/parser.h>
#include <fsi/vm/vm.h>

#define PARSER_HANDLER_DOT 0
#define PARSER_HANDLER_EMIT 1
#define PARSER_HANDLER_CR 2
#define PARSER_HANDLER_BL 3
#define PARSER_HANDLER_SPACE 4
#define PARSER_HANDLER_SPACES 5
#define PARSER_HANDLER_COLON 6
#define PARSER_HANDLER_SEMI_COLON 7
#define PARSER_HANDLER_IF 8
#define PARSER_HANDLER_THEN 9
#define PARSER_HANDLER_ELSE 10
#define PARSER_HANDLER_CASE 11
#define PARSER_HANDLER_OF 12
#define PARSER_HANDLER_ENDOF 13
#define PARSER_HANDLER_ENDCASE 14
#define PARSER_HANDLER_DO 15
#define PARSER_HANDLER_LOOP 16
#define PARSER_HANDLER_PLUS_LOOP 17
#define PARSER_HANDLER_I 18
#define PARSER_HANDLER_LEAVE 19
#define PARSER_HANDLER_UNLOOP 20
#define PARSER_HANDLER_EXIT 21
#define PARSER_HANDLER_J 22
#define PARSER_HANDLER_BEGIN 23
#define PARSER_HANDLER_WHILE 24
#define PARSER_HANDLER_REPEAT 25
#define PARSER_HANDLER_UNTIL 26
#define PARSER_HANDLER_AGAIN 27
#define PARSER_HANDLER_CREATE 28
#define PARSER_HANDLER_VARIABLE 29
#define PARSER_HANDLER_ALLOT 30
#define PARSER_HANDLER_CELLS 31
#define PARSER_HANDLER_DOES_GT 32
#define PARSER_HANDLER_AT 33
#define PARSER_HANDLER_BANG 34
#define PARSER_HANDLER_CAT 35
#define PARSER_HANDLER_CBANG 36
#define PARSER_HANDLER_TWO_AT 37
#define PARSER_HANDLER_TWO_BANG 38
#define PARSER_HANDLER_PLUS_BANG 39
#define PARSER_HANDLER_COMMA 40
#define PARSER_HANDLER_CCOMMA 41
#define PARSER_HANDLER_PLUS 42
#define PARSER_HANDLER_DASH 43
#define PARSER_HANDLER_ASTERISK 44
#define PARSER_HANDLER_SLASH 45
#define PARSER_HANDLER_MOD 46
#define PARSER_HANDLER_ONE_PLUS 47
#define PARSER_HANDLER_CELL_PLUS 48
#define PARSER_HANDLER_ONE_DASH 49
#define PARSER_HANDLER_TWO_ASTERISK 50
#define PARSER_HANDLER_TWO_SLASH 51
#define PARSER_HANDLER_LSFHIFT 52
#define PARSER_HANDLER_RSFHIFT 53
#define PARSER_HANDLER_AND 54
#define PARSER_HANDLER_OR 55
#define PARSER_HANDLER_XOR 56
#define PARSER_HANDLER_INVERT 57
#define PARSER_HANDLER_NEGATE 58
#define PARSER_HANDLER_GT 59
#define PARSER_HANDLER_LT 60
#define PARSER_HANDLER_EQ 61
#define PARSER_HANDLER_ULT 62
#define PARSER_HANDLER_ZERO_LT 63
#define PARSER_HANDLER_ZERO_EQ 64
#define PARSER_HANDLER_MIN 65
#define PARSER_HANDLER_MAX 66
#define PARSER_HANDLER_DUP 67
#define PARSER_HANDLER_DROP 68
#define PARSER_HANDLER_SWAP 69
#define PARSER_HANDLER_OVER 70
#define PARSER_HANDLER_NIP 71
#define PARSER_HANDLER_TWO_DUP 72
#define PARSER_HANDLER_TWO_DROP 73
#define PARSER_HANDLER_TWO_SWAP 74
#define PARSER_HANDLER_TWO_OVER 75
#define PARSER_HANDLER_QUESTION_DUP 76
#define PARSER_HANDLER_ROT 77
#define PARSER_HANDLER_PICK 78
#define PARSER_HANDLER_DEPTH 79
#define PARSER_HANDLER_GTR 80
#define PARSER_HANDLER_RGT 81
#define PARSER_HANDLER_TWO_GTR 82
#define PARSER_HANDLER_TWO_RGT 83
#define PARSER_HANDLER_CHAR 84
#define PARSER_HANDLER_CHAR_PLUS 85
#define PARSER_HANDLER_CHARS 86


int parser_handler_dot(ForthParser *parser, ForthVM *vm);
int parser_handler_emit(ForthParser *parser, ForthVM *vm);
int parser_handler_cr(ForthParser *parser, ForthVM *vm);
int parser_handler_bl(ForthParser *parser, ForthVM *vm);
int parser_handler_space(ForthParser *parser, ForthVM *vm);
int parser_handler_spaces(ForthParser *parser, ForthVM *vm);
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
int parser_handler_two_at(ForthParser *parser, ForthVM *vm);
int parser_handler_two_bang(ForthParser *parser, ForthVM *vm);
int parser_handler_plus_bang(ForthParser *parser, ForthVM *vm);
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
int parser_handler_min(ForthParser *parser, ForthVM *vm);
int parser_handler_max(ForthParser *parser, ForthVM *vm);
int parser_handler_dup(ForthParser *parser, ForthVM *vm);
int parser_handler_drop(ForthParser *parser, ForthVM *vm);
int parser_handler_swap(ForthParser *parser, ForthVM *vm);
int parser_handler_over(ForthParser *parser, ForthVM *vm);
int parser_handler_nip(ForthParser *parser, ForthVM *vm);
int parser_handler_two_dup(ForthParser *parser, ForthVM *vm);
int parser_handler_two_drop(ForthParser *parser, ForthVM *vm);
int parser_handler_two_swap(ForthParser *parser, ForthVM *vm);
int parser_handler_two_over(ForthParser *parser, ForthVM *vm);
int parser_handler_question_dup(ForthParser *parser, ForthVM *vm);
int parser_handler_rot(ForthParser *parser, ForthVM *vm);
int parser_handler_pick(ForthParser *parser, ForthVM *vm);
int parser_handler_depth(ForthParser *parser, ForthVM *vm);
int parser_handler_gtr(ForthParser *parser, ForthVM *vm);
int parser_handler_rgt(ForthParser *parser, ForthVM *vm);
int parser_handler_two_gtr(ForthParser *parser, ForthVM *vm);
int parser_handler_two_rgt(ForthParser *parser, ForthVM *vm);
int parser_handler_char(ForthParser *parser);
int parser_handler_char_plus(ForthParser *parser, ForthVM *vm);

#endif
