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
#define PARSER_HANDLER_FILL 31
#define PARSER_HANDLER_CELLS 32
#define PARSER_HANDLER_DOES_GT 33
#define PARSER_HANDLER_AT 34
#define PARSER_HANDLER_BANG 35
#define PARSER_HANDLER_CAT 36
#define PARSER_HANDLER_CBANG 37
#define PARSER_HANDLER_TWO_AT 38
#define PARSER_HANDLER_TWO_BANG 39
#define PARSER_HANDLER_PLUS_BANG 40
#define PARSER_HANDLER_COMMA 41
#define PARSER_HANDLER_CCOMMA 42
#define PARSER_HANDLER_PLUS 43
#define PARSER_HANDLER_DASH 44
#define PARSER_HANDLER_ASTERISK 45
#define PARSER_HANDLER_SLASH 46
#define PARSER_HANDLER_MOD 47
#define PARSER_HANDLER_ONE_PLUS 48
#define PARSER_HANDLER_CELL_PLUS 49
#define PARSER_HANDLER_ONE_DASH 50
#define PARSER_HANDLER_TWO_ASTERISK 51
#define PARSER_HANDLER_TWO_SLASH 52
#define PARSER_HANDLER_LSFHIFT 53
#define PARSER_HANDLER_RSFHIFT 54
#define PARSER_HANDLER_AND 55
#define PARSER_HANDLER_OR 56
#define PARSER_HANDLER_XOR 57
#define PARSER_HANDLER_INVERT 58
#define PARSER_HANDLER_NEGATE 59
#define PARSER_HANDLER_GT 60
#define PARSER_HANDLER_LT 61
#define PARSER_HANDLER_EQ 62
#define PARSER_HANDLER_ULT 63
#define PARSER_HANDLER_ZERO_LT 64
#define PARSER_HANDLER_ZERO_EQ 65
#define PARSER_HANDLER_MIN 66
#define PARSER_HANDLER_MAX 67
#define PARSER_HANDLER_DUP 68
#define PARSER_HANDLER_DROP 69
#define PARSER_HANDLER_SWAP 70
#define PARSER_HANDLER_OVER 71
#define PARSER_HANDLER_NIP 72
#define PARSER_HANDLER_TWO_DUP 73
#define PARSER_HANDLER_TWO_DROP 74
#define PARSER_HANDLER_TWO_SWAP 75
#define PARSER_HANDLER_TWO_OVER 76
#define PARSER_HANDLER_QUESTION_DUP 77
#define PARSER_HANDLER_ROT 78
#define PARSER_HANDLER_PICK 79
#define PARSER_HANDLER_DEPTH 80
#define PARSER_HANDLER_GTR 81
#define PARSER_HANDLER_RGT 82
#define PARSER_HANDLER_TWO_GTR 83
#define PARSER_HANDLER_TWO_RGT 84
#define PARSER_HANDLER_CHAR 85
#define PARSER_HANDLER_CHAR_PLUS 86
#define PARSER_HANDLER_CHARS 87
#define PARSER_HANDLER_TYPE 88
#define PARSER_HANDLER_S_DOUBLE_QUOTE 89
#define PARSER_HANDLER_DOT_DOUBLE_QUOTE 90
#define PARSER_HANDLER_C_DOUBLE_QUOTE 91
#define PARSER_HANDLER_COUNT 92
#define PARSER_HANDLER_D_GT_F 93
#define PARSER_HANDLER_F_GT_D 94
#define PARSER_HANDLER_F_DOT 95
#define PARSER_HANDLER_FS_DOT 96
#define PARSER_HANDLER_F_AT 97
#define PARSER_HANDLER_F_BANG 98
#define PARSER_HANDLER_FLOATS 99
#define PARSER_HANDLER_FLOAT_PLUS 100
#define PARSER_HANDLER_F_PLUS 101
#define PARSER_HANDLER_F_DASH 102
#define PARSER_HANDLER_F_ASTERISK 103
#define PARSER_HANDLER_F_SLASH 104
#define PARSER_HANDLER_FNEGATE 105
#define PARSER_HANDLER_F_ZERO_LT 106
#define PARSER_HANDLER_F_ZERO_EQ 107
#define PARSER_HANDLER_F_LT 108
#define PARSER_HANDLER_FMIN 109
#define PARSER_HANDLER_FMAX 110
#define PARSER_HANDLER_FDUP 111
#define PARSER_HANDLER_FDROP 112
#define PARSER_HANDLER_FSWAP 113
#define PARSER_HANDLER_FOVER 114
#define PARSER_HANDLER_FROT 115
#define PARSER_HANDLER_FDEPTH 116
#define PARSER_HANDLER_FVARIABLE 117

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
int parser_handler_fill(ForthParser *parser, ForthVM *vm);
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
int parser_handler_type(ForthParser *parser, ForthVM *vm);
int parser_handler_s_double_quote(
    ForthParser *parser, char **iter, ForthVM *vm);
int parser_handler_dot_double_quote(
    ForthParser *parser, char **iter, ForthVM *vm);
int parser_handler_c_double_quote(
    ForthParser *parser, char **iter, ForthVM *vm);
int parser_handler_count(ForthParser *parser, ForthVM *vm);
int parser_handler_d_gt_f(ForthParser *parser, ForthVM *vm);
int parser_handler_f_gt_d(ForthParser *parser, ForthVM *vm);
int parser_handler_f_dot(ForthParser *parser, ForthVM *vm);
int parser_handler_fs_dot(ForthParser *parser, ForthVM *vm);
int parser_handler_f_at(ForthParser *parser, ForthVM *vm);
int parser_handler_f_bang(ForthParser *parser, ForthVM *vm);
int parser_handler_floats(ForthParser *parser, ForthVM *vm);
int parser_handler_float_plus(ForthParser *parser, ForthVM *vm);
int parser_handler_f_plus(ForthParser *parser, ForthVM *vm);
int parser_handler_f_dash(ForthParser *parser, ForthVM *vm);
int parser_handler_f_asterisk(ForthParser *parser, ForthVM *vm);
int parser_handler_f_slash(ForthParser *parser, ForthVM *vm);
int parser_handler_fnegate(ForthParser *parser, ForthVM *vm);
int parser_handler_f_zero_lt(ForthParser *parser, ForthVM *vm);
int parser_handler_f_zero_eq(ForthParser *parser, ForthVM *vm);
int parser_handler_f_lt(ForthParser *parser, ForthVM *vm);
int parser_handler_fmin(ForthParser *parser, ForthVM *vm);
int parser_handler_fmax(ForthParser *parser, ForthVM *vm);
int parser_handler_fdup(ForthParser *parser, ForthVM *vm);
int parser_handler_fdrop(ForthParser *parser, ForthVM *vm);
int parser_handler_fswap(ForthParser *parser, ForthVM *vm);
int parser_handler_fover(ForthParser *parser, ForthVM *vm);
int parser_handler_frot(ForthParser *parser, ForthVM *vm);
int parser_handler_fdepth(ForthParser *parser, ForthVM *vm);
int parser_handler_fvariable(ForthParser *parser, ForthVM *vm);

#endif
