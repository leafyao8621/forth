#include <stdio.h>
#include <string.h>

typedef unsigned char uint8_t;
typedef unsigned uintptr_t;
typedef int intptr_t;
typedef unsigned char bool;

#define true 1
#define false 0

#define VM_LOOKUP_META_BUILTIN 0x1
#define VM_LOOKUP_META_MEMORY 0x2
#define VM_LOOKUP_META_CALL 0x4
#define VM_LOOKUP_META_CREATE 0x8
#define VM_LOOKUP_META_VARIABLE 0x10

#define VM_INSTRUCTION_HALT 0
#define VM_INSTRUCTION_RET 1
#define VM_INSTRUCTION_PUSHD 2
#define VM_INSTRUCTION_PUSHID 3
#define VM_INSTRUCTION_PINT 4
#define VM_INSTRUCTION_EMIT 5
#define VM_INSTRUCTION_CALL 6
#define VM_INSTRUCTION_JZD 7
#define VM_INSTRUCTION_JNZD 8
#define VM_INSTRUCTION_JMP 9
#define VM_INSTRUCTION_2PUSHC 10
#define VM_INSTRUCTION_PEEKC 11
#define VM_INSTRUCTION_2RMC 12
#define VM_INSTRUCTION_JNEC 13
#define VM_INSTRUCTION_INCC 14
#define VM_INSTRUCTION_ADDC 15
#define VM_INSTRUCTION_3PEEKC 16
#define VM_INSTRUCTION_DEF 17
#define VM_INSTRUCTION_DEFA 18
#define VM_INSTRUCTION_ALLOC 19
#define VM_INSTRUCTION_MULTCSD 20
#define VM_INSTRUCTION_LDD 21
#define VM_INSTRUCTION_STD 22
#define VM_INSTRUCTION_LDCD 23
#define VM_INSTRUCTION_STCD 24
#define VM_INSTRUCTION_ADDD 25
#define VM_INSTRUCTION_SUBD 26
#define VM_INSTRUCTION_MULTD 27
#define VM_INSTRUCTION_DIVD 28
#define VM_INSTRUCTION_MODD 29
#define VM_INSTRUCTION_INCD 30
#define VM_INSTRUCTION_DECD 31
#define VM_INSTRUCTION_DUBD 32
#define VM_INSTRUCTION_HALFD 33
#define VM_INSTRUCTION_LSLD 34
#define VM_INSTRUCTION_LSRD 35
#define VM_INSTRUCTION_ANDD 36
#define VM_INSTRUCTION_ORD 37
#define VM_INSTRUCTION_XORD 38
#define VM_INSTRUCTION_NOTD 39
#define VM_INSTRUCTION_NEGD 40
#define VM_INSTRUCTION_GTD 41
#define VM_INSTRUCTION_LTD 42
#define VM_INSTRUCTION_EQD 43
#define VM_INSTRUCTION_ULTD 44
#define VM_INSTRUCTION_LTZD 45
#define VM_INSTRUCTION_EQZD 46
#define VM_INSTRUCTION_DUPD 47
#define VM_INSTRUCTION_DROPD 48
#define VM_INSTRUCTION_SWAPD 49

#define VM_STATE_RUNNING 0
#define VM_STATE_HALTED 1

#define BUILTIN_SIZE 55
#define MEMORY_SIZE 2

uint8_t mem[32768];

uint8_t *vm_lookup;
uint8_t *vm_lookup_cur;
uint8_t *vm_lookup_end;
uint8_t *vm_literal;
uint8_t *vm_literal_cur;
uint8_t *vm_literal_end;
uint8_t *vm_interpreted;
uint8_t *vm_interpreted_cur;
uint8_t *vm_interpreted_end;
uint8_t *vm_data_stack;
uint8_t *vm_data_stack_cur;
uint8_t *vm_data_stack_end;
uint8_t *vm_control_stack;
uint8_t *vm_control_stack_cur;
uint8_t *vm_control_stack_end;
uint8_t *vm_compiled;
uint8_t *vm_compiled_cur;
uint8_t *vm_compiled_end;
uint8_t *vm_memory;
uint8_t *vm_memory_cur;
uint8_t *vm_memory_end;

uint8_t *vm_ip;
uint8_t vm_state;

void vm_initialize(void);
void vm_log(void);
void vm_reset(void);
int vm_run(bool debug);

#define PARSER_STATE_INTERPRET 0x1
#define PARSER_STATE_COMPILE 0x2
#define PARSER_STATE_NAME 0x4
#define PARSER_STATE_CREATE 0x8

#define PARSER_STATUS_RUNNING 0
#define PARSER_STATUS_END 1
#define PARSER_STATUS_REPL_END 2

#define PARSER_CONTROL_IF 0
#define PARSER_CONTROL_ELSE 1
#define PARSER_CONTROL_DO 2
#define PARSER_CONTROL_DO_LEAVE 3
#define PARSER_CONTROL_BEGIN 4
#define PARSER_CONTROL_WHILE 5

static char buf[100];
static char *buf_end;

uint8_t parser_state;
uint8_t parser_status;

uint8_t *parser_pending;

uint8_t parser_conditional_stack[(sizeof(uintptr_t) + 1) * 20];
uint8_t *parser_conditional_stack_cur;
uint8_t *parser_conditional_stack_end;

uint8_t parser_loop_stack[(sizeof(uintptr_t) + 1) * 20];
uint8_t *parser_loop_stack_cur;
uint8_t *parser_loop_stack_end;

bool parser_eos;

void parser_initialize(void);
int parser_parse(bool debug, bool line, FILE *fin);

#define PARSER_STATUS_OK 0
#define PARSER_STATUS_END_REPL 1
#define PARSER_STATUS_TOKEN_TOO_LONG 2
#define PARSER_STATUS_INTERPRETED_OVERFLOW 3
#define PARSER_STATUS_COMPILED_OVERFLOW 4
#define PARSER_STATUS_INVALID_BASE 5
#define PARSER_STATUS_INVALID_BASE10 6
#define PARSER_STATUS_INVALID_BASE16 7
#define PARSER_STATUS_NESTED_COMPILATION_MODE 8
#define PARSER_STATUS_NOT_IN_COMPILATION_MODE 9
#define PARSER_STATUS_LOOKUP_OVERFLOW 10
#define PARSER_STATUS_PENDING_DEFINITION 11
#define PARSER_STATUS_CONTROL_STACK_OVERFLOW 12
#define PARSER_STATUS_CONTROL_STACK_UNDERFLOW 13
#define PARSER_STATUS_CONTROL_STACK_MISMATCH 14

#define VM_STATUS_OK 0
#define VM_STATUS_DATA_STACK_OVERFLOW 1
#define VM_STATUS_DATA_STACK_UNDERFLOW 2
#define VM_STATUS_INVALID_BASE 3
#define VM_STATUS_CONTROL_STACK_OVERFLOW 4
#define VM_STATUS_CONTROL_STACK_UNDERFLOW 5
#define VM_STATUS_INVALID_RETURN_ADDRESS 6
#define VM_STATUS_INVALID_MEMORY_ADDRESS 7
#define VM_STATUS_INVALID_MEMORY_ALLOCATION 8

const char *parser_status_lookup[15] =
    {
        "PARSER_STATUS_OK",
        "PARSER_STATUS_END_REPL",
        "PARSER_STATUS_TOKEN_TOO_LONG",
        "PARSER_STATUS_INTERPRETED_OVERFLOW",
        "PARSER_STATUS_COMPILED_OVERFLOW",
        "PARSER_STATUS_INVALID_BASE",
        "PARSER_STATUS_INVALID_BASE10",
        "PARSER_STATUS_INVALID_BASE16",
        "PARSER_STATUS_NESTED_COMPILATION_MODE",
        "PARSER_STATUS_NOT_IN_COMPILATION_MODE",
        "PARSER_STATUS_LOOKUP_OVERFLOW",
        "PARSER_STATUS_PENDING_DEFINITION",
        "PARSER_STATUS_CONTROL_STACK_OVERFLOW",
        "PARSER_STATUS_CONTROL_STACK_UNDERFLOW",
        "PARSER_STATUS_CONTROL_STACK_MISMATCH"
    };

const char *vm_status_lookup[9] =
    {
        "VM_STATUS_OK",
        "VM_STATUS_DATA_STACK_OVERFLOW",
        "VM_STATUS_DATA_STACK_UNDERFLOW",
        "VM_STATUS_INVALID_BASE",
        "VM_STATUS_CONTROL_STACK_OVERFLOW",
        "VM_STATUS_CONTROL_STACK_UNDERFLOW",
        "VM_STATUS_INVALID_RETURN_ADDRESS",
        "VM_STATUS_INVALID_MEMORY_ADDRESS",
        "VM_STATUS_INVALID_MEMORY_ALLOCATION"
    };

int main(int argc, char **argv) {
    int ret = 0;
    bool debug = false;
    bool script = false;
    parser_initialize();
    vm_initialize();
    if (argc > 1) {
        FILE *fin = 0;
        int i = 1;
        for (; i < argc; ++i) {
            if (!strcmp(argv[i], "-d")) {
                debug = true;
                continue;
            }
            if (!strcmp(argv[i], "-s")) {
                script = true;
                continue;
            }
            break;
        }
        for (; i < argc; ++i) {
            vm_reset();
            fin = fopen(argv[i], "r");
            if (!fin) {
                fprintf(stderr, "Cannot open %s\n", argv[i]);
                return 1;
            }
            ret = parser_parse(debug, false, fin);
            fclose(fin);
            if (ret) {
                fprintf(
                    stderr,
                    "Error parsing\n%s\n",
                    parser_status_lookup[ret]
                );
                return 1;
            }
            if (debug) {
                vm_log();
            }
            ret = vm_run(debug);
            if (ret) {
                fprintf(stderr, "Error running\n%s\n", vm_status_lookup[ret]);
                return 1;
            }
        }
    }
    if (script) {
        return 0;
    }
    for (;;) {
        vm_reset();
        printf("%s", "> ");
        ret = parser_parse(debug, true, stdin);
        if (ret == PARSER_STATUS_END_REPL) {
            break;
        }
        if (ret) {
            fprintf(stderr, "Error parsing\n%s\n", parser_status_lookup[ret]);
            if (!parser_eos) {
                for (ret = fgetc(stdin); ret != '\n'; ret = fgetc(stdin));
            }
            continue;
        }
        if (debug) {
            vm_log();
        }
        ret = vm_run(debug);
        if (ret) {
            fprintf(stderr, "Error running\n%s\n", vm_status_lookup[ret]);
        }
    }
    return 0;
}

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
#define PARSER_HANDLER_CREATE 21
#define PARSER_HANDLER_VARIABLE 22
#define PARSER_HANDLER_ALLOT 23
#define PARSER_HANDLER_CELLS 24
#define PARSER_HANDLER_CELL_PLUS 25
#define PARSER_HANDLER_AT 26
#define PARSER_HANDLER_BANG 27
#define PARSER_CAT 28
#define PARSER_CBANG 29
#define PARSER_HANDLER_PLUS 30
#define PARSER_HANDLER_DASH 31
#define PARSER_HANDLER_ASTERISK 32
#define PARSER_HANDLER_SLASH 33
#define PARSER_HANDLER_MOD 34
#define PARSER_HANDLER_ONE_PLUS 35
#define PARSER_HANDLER_ONE_DASH 36
#define PARSER_HANDLER_TWO_ASTERISK 37
#define PARSER_HANDLER_TWO_SLASH 38
#define PARSER_HANDLER_LSFHIFT 39
#define PARSER_HANDLER_RSFHIFT 40
#define PARSER_HANDLER_AND 41
#define PARSER_HANDLER_OR 42
#define PARSER_HANDLER_XOR 43
#define PARSER_HANDLER_INVERT 44
#define PARSER_HANDLER_NEGATE 45
#define PARSER_HANDLER_GT 46
#define PARSER_HANDLER_LT 47
#define PARSER_HANDLER_EQ 48
#define PARSER_HANDLER_ULT 49
#define PARSER_HANDLER_ZERO_LT 50
#define PARSER_HANDLER_ZERO_EQ 51
#define PARSER_HANDLER_DUP 52
#define PARSER_HANDLER_DROP 53
#define PARSER_HANDLER_SWAP 54


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
int parser_handler_begin(void);
int parser_handler_while(void);
int parser_handler_repeat(void);
int parser_handler_until(void);
int parser_handler_again(void);
int parser_handler_create(void);
int parser_handler_variable(void);
int parser_handler_allocate(void);
int parser_handler_cells(void);
int parser_handler_cell_plus(void);
int parser_handler_at(void);
int parser_handler_bang(void);
int parser_handler_cat(void);
int parser_handler_cbang(void);
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

int vm_handler_ret(void);
int vm_handler_pushd(void);
int vm_handler_pushid(void);
int vm_handler_pint(void);
int vm_handler_emit(void);
int vm_handler_call(void);
int vm_handler_jzd(void);
int vm_handler_jnzd(void);
int vm_handler_jmp(void);
int vm_handler_2pushc(void);
int vm_handler_peekc(void);
int vm_handler_2rmc(void);
int vm_handler_jnec(void);
int vm_handler_incc(void);
int vm_handler_addc(void);
int vm_handler_3peekc(void);
int vm_handler_def(void);
int vm_handler_defa(void);
int vm_handler_alloc(void);
int vm_handler_multcsd(void);
int vm_handler_std(void);
int vm_handler_ldd(void);
int vm_handler_addd(void);
int vm_handler_subd(void);
int vm_handler_multd(void);
int vm_handler_divd(void);
int vm_handler_modd(void);
int vm_handler_incd(void);
int vm_handler_decd(void);
int vm_handler_dubd(void);
int vm_handler_halfd(void);
int vm_handler_lsld(void);
int vm_handler_lsrd(void);
int vm_handler_andd(void);
int vm_handler_ord(void);
int vm_handler_xord(void);
int vm_handler_notd(void);
int vm_handler_negd(void);
int vm_handler_gtd(void);
int vm_handler_ltd(void);
int vm_handler_eqd(void);
int vm_handler_ultd(void);
int vm_handler_ltzd(void);
int vm_handler_eqzd(void);
int vm_handler_dupd(void);
int vm_handler_dropd(void);
int vm_handler_swapd(void);


void vm_initialize() {
    static const char *builtin[BUILTIN_SIZE] =
        {
            ".",
            "emit",
            "cr",
            ":",
            ";",
            "if",
            "then",
            "else",
            "do",
            "loop",
            "+loop",
            "i",
            "leave",
            "unloop",
            "exit",
            "j",
            "begin",
            "while",
            "repeat",
            "until",
            "again",
            "create",
            "variable",
            "allot",
            "cells",
            "cell+",
            "@",
            "!",
            "c@",
            "c!",
            "+",
            "-",
            "*",
            "/",
            "mod",
            "1+",
            "1-",
            "2*",
            "2/",
            "lshift",
            "rshift",
            "and",
            "or",
            "xor",
            "invert",
            "negate",
            ">",
            "<",
            "=",
            "u<",
            "0<",
            "0=",
            "dup",
            "drop",
            "swap"
        };
    static const char *memory[MEMORY_SIZE] =
        {
            "base",
            "here"
        };
    static const uintptr_t memory_value[MEMORY_SIZE] =
        {
            10,
            0
        };
    const char **iter_builtin = builtin;
    const char *iter_str = 0;
    const char **iter_memory = memory;
    const uintptr_t *iter_memory_value = memory_value;
    size_t i = 0;
    vm_lookup = mem;
    vm_lookup_cur = vm_lookup;
    vm_lookup_end = mem + 4000;
    vm_literal = vm_lookup_end;
    vm_literal_cur = vm_literal;
    vm_literal_end = vm_literal + 1000;
    vm_interpreted = vm_literal_end;
    vm_interpreted_cur = vm_interpreted;
    vm_interpreted_end = vm_interpreted + 1000;
    vm_data_stack = vm_interpreted_end;
    vm_data_stack_cur = vm_data_stack;
    vm_data_stack_end = vm_data_stack + 1000;
    vm_control_stack = vm_data_stack_end;
    vm_control_stack_cur = vm_control_stack;
    vm_control_stack_end = vm_control_stack + 1000;
    vm_compiled = vm_control_stack_end;
    vm_compiled_cur = vm_compiled;
    vm_compiled_end = vm_compiled + 12000;
    vm_memory = vm_compiled_end;
    vm_memory_cur = vm_memory + sizeof(size_t);
    *(uint8_t**)vm_memory_cur = vm_memory;
    vm_memory_end = mem + 32768;
    for (; i < BUILTIN_SIZE; ++i, ++iter_builtin) {
        *(vm_lookup_cur++) = VM_LOOKUP_META_BUILTIN;
        *(size_t*)(vm_lookup_cur) = i;
        vm_lookup_cur += sizeof(size_t);
        for (iter_str = *iter_builtin; *iter_str; ++iter_str, ++vm_lookup_cur) {
            *vm_lookup_cur = *iter_str;
        }
        *(vm_lookup_cur++) = 0;
    }
    for (i = 0; i < MEMORY_SIZE; ++i, ++iter_memory, ++iter_memory_value) {
        *(vm_lookup_cur++) = VM_LOOKUP_META_MEMORY;
        *(uintptr_t*)(vm_lookup_cur) = *(uintptr_t*)vm_memory_cur;
        if (i != 1) {
            **(uintptr_t**)vm_memory_cur = *iter_memory_value;
        }
        vm_lookup_cur += sizeof(size_t);
        *vm_memory_cur += sizeof(size_t);
        for (iter_str = *iter_memory; *iter_str; ++iter_str, ++vm_lookup_cur) {
            *vm_lookup_cur = *iter_str;
        }
        *(vm_lookup_cur++) = 0;
    }
}

void vm_log(void) {
    uint8_t *iter = 0;
    puts("Lookup:");
    for (iter = vm_lookup; iter < vm_lookup_cur; ++iter) {
        printf("0x%04X ", (uintptr_t)iter);
        if (*iter & VM_LOOKUP_META_BUILTIN) {
            printf("%s ", "BUILTIN");
        }
        if (*iter & VM_LOOKUP_META_CALL) {
            printf("%s ", "CALL");
        }
        if (*iter & VM_LOOKUP_META_MEMORY) {
            printf("%s ", "MEMORY");
        }
        printf("0x%04X ", *(size_t*)(++iter));
        iter += sizeof(size_t);
        for (; *iter; ++iter) {
            putchar(*iter);
        }
        putchar(10);
    }
    puts("Memory:");
    for (
        iter = vm_memory;
        iter < *(uint8_t**)vm_memory_cur;
        iter += sizeof(uintptr_t)) {
        printf("0x%04X 0x%04X\n", (uintptr_t)iter, *(uintptr_t*)iter);
    }
    puts("Compiled:");
    for (iter = vm_compiled; iter < vm_compiled_cur; ++iter) {
        printf("0x%04X ", (uintptr_t)iter);
        switch (*iter) {
        case VM_INSTRUCTION_HALT:
            printf("%s", "HALT");
            break;
        case VM_INSTRUCTION_RET:
            printf("%s", "RET");
            break;
        case VM_INSTRUCTION_PUSHD:
            printf("%s 0x%04X", "PUSHD", *(uintptr_t*)(iter + 1));
            iter += sizeof(uintptr_t);
            break;
        case VM_INSTRUCTION_PUSHID:
            printf("%s 0x%04X", "PUSHID", *(uintptr_t*)(iter + 1));
            iter += sizeof(uintptr_t);
            break;
        case VM_INSTRUCTION_PINT:
            printf("%s", "PINT");
            break;
        case VM_INSTRUCTION_EMIT:
            printf("%s", "EMIT");
            break;
        case VM_INSTRUCTION_CALL:
            printf("%s 0x%04X", "CALL", *(uintptr_t*)(iter + 1));
            iter += sizeof(uintptr_t);
            break;
        case VM_INSTRUCTION_JZD:
            printf("%s 0x%04X", "JZD", *(uintptr_t*)(iter + 1));
            iter += sizeof(uintptr_t);
            break;
        case VM_INSTRUCTION_JNZD:
            printf("%s 0x%04X", "JNZD", *(uintptr_t*)(iter + 1));
            iter += sizeof(uintptr_t);
            break;
        case VM_INSTRUCTION_JMP:
            printf("%s 0x%04X", "JMP", *(uintptr_t*)(iter + 1));
            iter += sizeof(uintptr_t);
            break;
        case VM_INSTRUCTION_2PUSHC:
            printf("%s", "2PUSHC");
            break;
        case VM_INSTRUCTION_PEEKC:
            printf("%s", "PEEKC");
            break;
        case VM_INSTRUCTION_2RMC:
            printf("%s", "2RMC");
            break;
        case VM_INSTRUCTION_JNEC:
            printf("%s 0x%04X", "JNEC", *(uintptr_t*)(iter + 1));
            iter += sizeof(uintptr_t);
            break;
        case VM_INSTRUCTION_INCC:
            printf("%s", "INCC");
            break;
        case VM_INSTRUCTION_ADDC:
            printf("%s", "ADDC");
            break;
        case VM_INSTRUCTION_3PEEKC:
            printf("%s", "3PEEKC");
            break;
        case VM_INSTRUCTION_ALLOC:
            printf("%s", "ALLOC");
            break;
        case VM_INSTRUCTION_MULTCSD:
            printf("%s", "MULTCSD");
            break;
        case VM_INSTRUCTION_STD:
            printf("%s", "STD");
            break;
        case VM_INSTRUCTION_LDD:
            printf("%s", "LDD");
            break;
        case VM_INSTRUCTION_ADDD:
            printf("%s", "ADDD");
            break;
        case VM_INSTRUCTION_SUBD:
            printf("%s", "SUBD");
            break;
        case VM_INSTRUCTION_MULTD:
            printf("%s", "MULTD");
            break;
        case VM_INSTRUCTION_DIVD:
            printf("%s", "DIVD");
            break;
        case VM_INSTRUCTION_MODD:
            printf("%s", "MODD");
            break;
        case VM_INSTRUCTION_INCD:
            printf("%s", "INCD");
            break;
        case VM_INSTRUCTION_DECD:
            printf("%s", "DECD");
            break;
        case VM_INSTRUCTION_DUBD:
            printf("%s", "DUBD");
            break;
        case VM_INSTRUCTION_HALFD:
            printf("%s", "HALFD");
            break;
        case VM_INSTRUCTION_LSLD:
            printf("%s", "LSLD");
            break;
        case VM_INSTRUCTION_LSRD:
            printf("%s", "LSRD");
            break;
        case VM_INSTRUCTION_ANDD:
            printf("%s", "ANDD");
            break;
        case VM_INSTRUCTION_ORD:
            printf("%s", "ORD");
            break;
        case VM_INSTRUCTION_XORD:
            printf("%s", "XORD");
            break;
        case VM_INSTRUCTION_NOTD:
            printf("%s", "NOTD");
            break;
        case VM_INSTRUCTION_NEGD:
            printf("%s", "NEGD");
            break;
        case VM_INSTRUCTION_GTD:
            printf("%s", "GTD");
            break;
        case VM_INSTRUCTION_LTD:
            printf("%s", "LTD");
            break;
        case VM_INSTRUCTION_EQD:
            printf("%s", "EQD");
            break;
        case VM_INSTRUCTION_ULTD:
            printf("%s", "ULTD");
            break;
        case VM_INSTRUCTION_LTZD:
            printf("%s", "LTZD");
            break;
        case VM_INSTRUCTION_EQZD:
            printf("%s", "EQZD");
            break;
        case VM_INSTRUCTION_DUPD:
            printf("%s", "DUPD");
            break;
        case VM_INSTRUCTION_DROPD:
            printf("%s", "DROPD");
            break;
        case VM_INSTRUCTION_SWAPD:
            printf("%s", "SWAPD");
            break;
        }
        putchar(10);
    }
    puts("Interpreted:");
    for (iter = vm_interpreted; iter < vm_interpreted_cur; ++iter) {
        printf("0x%04X ", (uintptr_t)iter);
        switch (*iter) {
        case VM_INSTRUCTION_HALT:
            printf("%s", "HALT");
            break;
        case VM_INSTRUCTION_RET:
            printf("%s", "RET");
            break;
        case VM_INSTRUCTION_PUSHD:
            printf("%s 0x%04X", "PUSHD", *(uintptr_t*)(iter + 1));
            iter += sizeof(uintptr_t);
            break;
        case VM_INSTRUCTION_PUSHID:
            printf("%s 0x%04X", "PUSHID", *(uintptr_t*)(iter + 1));
            iter += sizeof(uintptr_t);
            break;
        case VM_INSTRUCTION_PINT:
            printf("%s", "PINT");
            break;
        case VM_INSTRUCTION_EMIT:
            printf("%s", "EMIT");
            break;
        case VM_INSTRUCTION_CALL:
            printf("%s 0x%04X", "CALL", *(uintptr_t*)(iter + 1));
            iter += sizeof(uintptr_t);
            break;
        case VM_INSTRUCTION_DEF:
            printf("%s 0x%04X", "DEF", *(uintptr_t*)(iter + 1));
            iter += sizeof(uintptr_t);
            break;
        case VM_INSTRUCTION_DEFA:
            printf("%s 0x%04X", "DEFA", *(uintptr_t*)(iter + 1));
            iter += sizeof(uintptr_t);
            break;
        case VM_INSTRUCTION_ALLOC:
            printf("%s", "ALLOC");
            break;
        case VM_INSTRUCTION_MULTCSD:
            printf("%s", "MULTCSD");
            break;
        case VM_INSTRUCTION_STD:
            printf("%s", "STD");
            break;
        case VM_INSTRUCTION_LDD:
            printf("%s", "LDD");
            break;
        case VM_INSTRUCTION_ADDD:
            printf("%s", "ADDD");
            break;
        case VM_INSTRUCTION_SUBD:
            printf("%s", "SUBD");
            break;
        case VM_INSTRUCTION_MULTD:
            printf("%s", "MULTD");
            break;
        case VM_INSTRUCTION_DIVD:
            printf("%s", "DIVD");
            break;
        case VM_INSTRUCTION_MODD:
            printf("%s", "MODD");
            break;
        case VM_INSTRUCTION_INCD:
            printf("%s", "INCD");
            break;
        case VM_INSTRUCTION_DECD:
            printf("%s", "DECD");
            break;
        case VM_INSTRUCTION_DUBD:
            printf("%s", "DUBD");
            break;
        case VM_INSTRUCTION_HALFD:
            printf("%s", "HALFD");
            break;
        case VM_INSTRUCTION_LSLD:
            printf("%s", "LSLD");
            break;
        case VM_INSTRUCTION_LSRD:
            printf("%s", "LSRD");
            break;
        case VM_INSTRUCTION_ANDD:
            printf("%s", "ANDD");
            break;
        case VM_INSTRUCTION_ORD:
            printf("%s", "ORD");
            break;
        case VM_INSTRUCTION_XORD:
            printf("%s", "XORD");
            break;
        case VM_INSTRUCTION_NOTD:
            printf("%s", "NOTD");
            break;
        case VM_INSTRUCTION_NEGD:
            printf("%s", "NEGD");
            break;
        case VM_INSTRUCTION_GTD:
            printf("%s", "GTD");
            break;
        case VM_INSTRUCTION_LTD:
            printf("%s", "LTD");
            break;
        case VM_INSTRUCTION_EQD:
            printf("%s", "EQD");
            break;
        case VM_INSTRUCTION_ULTD:
            printf("%s", "ULTD");
            break;
        case VM_INSTRUCTION_LTZD:
            printf("%s", "LTZD");
            break;
        case VM_INSTRUCTION_EQZD:
            printf("%s", "EQZD");
            break;
        case VM_INSTRUCTION_DUPD:
            printf("%s", "DUPD");
            break;
        case VM_INSTRUCTION_DROPD:
            printf("%s", "DROPD");
            break;
        case VM_INSTRUCTION_SWAPD:
            printf("%s", "SWAPD");
            break;
        }
        putchar(10);
    }
}

void vm_reset(void) {
    vm_interpreted_cur = vm_interpreted;
    vm_ip = vm_interpreted;
}

int vm_run(bool debug) {
    int ret = VM_STATUS_OK;
    vm_state = VM_STATE_RUNNING;
    for (; vm_state != VM_STATE_HALTED; ++vm_ip) {
        if (debug) {
            printf("\nExecuting 0x%04X ", (uintptr_t)vm_ip);
            switch (*vm_ip) {
            case VM_INSTRUCTION_HALT:
                printf("%s", "HALT");
                break;
            case VM_INSTRUCTION_RET:
                printf("%s", "RET");
                break;
            case VM_INSTRUCTION_PUSHD:
                printf("%s 0x%04X", "PUSHD", *(uintptr_t*)(vm_ip + 1));
                break;
            case VM_INSTRUCTION_PUSHID:
                printf("%s 0x%04X", "PUSHID", *(uintptr_t*)(vm_ip + 1));
                break;
            case VM_INSTRUCTION_PINT:
                printf("%s", "PINT");
                break;
            case VM_INSTRUCTION_EMIT:
                printf("%s", "EMIT");
                break;
            case VM_INSTRUCTION_CALL:
                printf("%s 0x%04X", "CALL", *(uintptr_t*)(vm_ip + 1));
                break;
            case VM_INSTRUCTION_JZD:
                printf("%s 0x%04X", "JZD", *(uintptr_t*)(vm_ip + 1));
                break;
            case VM_INSTRUCTION_JNZD:
                printf("%s 0x%04X", "JNZD", *(uintptr_t*)(vm_ip + 1));
                break;
            case VM_INSTRUCTION_JMP:
                printf("%s 0x%04X", "JMP", *(uintptr_t*)(vm_ip + 1));
                break;
            case VM_INSTRUCTION_2PUSHC:
                printf("%s", "2PUSHC");
                break;
            case VM_INSTRUCTION_PEEKC:
                printf("%s", "PEEKC");
                break;
            case VM_INSTRUCTION_2RMC:
                printf("%s", "2RMC");
                break;
            case VM_INSTRUCTION_JNEC:
                printf("%s 0x%04X", "JNEC", *(uintptr_t*)(vm_ip + 1));
                break;
            case VM_INSTRUCTION_INCC:
                printf("%s", "INCC");
                break;
            case VM_INSTRUCTION_ADDC:
                printf("%s", "ADDC");
                break;
            case VM_INSTRUCTION_3PEEKC:
                printf("%s", "3PEEKC");
                break;
            case VM_INSTRUCTION_DEF:
                printf("%s 0x%04X", "DEF", *(uintptr_t*)(vm_ip + 1));
                break;
            case VM_INSTRUCTION_DEFA:
                printf("%s 0x%04X", "DEFA", *(uintptr_t*)(vm_ip + 1));
                break;
            case VM_INSTRUCTION_ALLOC:
                printf("%s", "ALLOC");
                break;
            case VM_INSTRUCTION_MULTCSD:
                printf("%s", "MULTCSD");
                break;
            case VM_INSTRUCTION_STD:
                printf("%s", "STD");
                break;
            case VM_INSTRUCTION_LDD:
                printf("%s", "LDD");
                break;
            case VM_INSTRUCTION_ADDD:
                printf("%s", "ADDD");
                break;
            case VM_INSTRUCTION_SUBD:
                printf("%s", "SUBD");
                break;
            case VM_INSTRUCTION_MULTD:
                printf("%s", "MULTD");
                break;
            case VM_INSTRUCTION_DIVD:
                printf("%s", "DIVD");
                break;
            case VM_INSTRUCTION_MODD:
                printf("%s", "MODD");
                break;
            case VM_INSTRUCTION_INCD:
                printf("%s", "INCD");
                break;
            case VM_INSTRUCTION_DECD:
                printf("%s", "DECD");
                break;
            case VM_INSTRUCTION_DUBD:
                printf("%s", "DUBD");
                break;
            case VM_INSTRUCTION_HALFD:
                printf("%s", "HALFD");
                break;
            case VM_INSTRUCTION_LSLD:
                printf("%s", "LSLD");
                break;
            case VM_INSTRUCTION_LSRD:
                printf("%s", "LSRD");
                break;
            case VM_INSTRUCTION_ANDD:
                printf("%s", "ANDD");
                break;
            case VM_INSTRUCTION_ORD:
                printf("%s", "ORD");
                break;
            case VM_INSTRUCTION_XORD:
                printf("%s", "XORD");
                break;
            case VM_INSTRUCTION_NOTD:
                printf("%s", "NOTD");
                break;
            case VM_INSTRUCTION_NEGD:
                printf("%s", "NEGD");
                break;
            case VM_INSTRUCTION_GTD:
                printf("%s", "GTD");
                break;
            case VM_INSTRUCTION_LTD:
                printf("%s", "LTD");
                break;
            case VM_INSTRUCTION_EQD:
                printf("%s", "EQD");
                break;
            case VM_INSTRUCTION_ULTD:
                printf("%s", "ULTD");
                break;
            case VM_INSTRUCTION_LTZD:
                printf("%s", "LTZD");
                break;
            case VM_INSTRUCTION_EQZD:
                printf("%s", "EQZD");
                break;
            case VM_INSTRUCTION_DUPD:
                printf("%s", "DUPD");
                break;
            case VM_INSTRUCTION_DROPD:
                printf("%s", "DROPD");
                break;
            case VM_INSTRUCTION_SWAPD:
                printf("%s", "SWAPD");
                break;
            }
            putchar(10);
        }
        switch (*vm_ip) {
        case VM_INSTRUCTION_HALT:
            vm_state = VM_STATE_HALTED;
            break;
        case VM_INSTRUCTION_RET:
            ret = vm_handler_ret();
            break;
        case VM_INSTRUCTION_PUSHD:
            ret = vm_handler_pushd();
            break;
        case VM_INSTRUCTION_PUSHID:
            ret = vm_handler_pushid();
            break;
        case VM_INSTRUCTION_PINT:
            ret = vm_handler_pint();
            break;
        case VM_INSTRUCTION_EMIT:
            ret = vm_handler_emit();
            break;
        case VM_INSTRUCTION_CALL:
            ret = vm_handler_call();
            break;
        case VM_INSTRUCTION_JZD:
            ret = vm_handler_jzd();
            break;
        case VM_INSTRUCTION_JNZD:
            ret = vm_handler_jnzd();
            break;
        case VM_INSTRUCTION_JMP:
            ret = vm_handler_jmp();
            break;
        case VM_INSTRUCTION_2PUSHC:
            ret = vm_handler_2pushc();
            break;
        case VM_INSTRUCTION_PEEKC:
            ret = vm_handler_peekc();
            break;
        case VM_INSTRUCTION_2RMC:
            ret = vm_handler_2rmc();
            break;
        case VM_INSTRUCTION_JNEC:
            ret = vm_handler_jnec();
            break;
        case VM_INSTRUCTION_INCC:
            ret = vm_handler_incc();
            break;
        case VM_INSTRUCTION_ADDC:
            ret = vm_handler_addc();
            break;
        case VM_INSTRUCTION_3PEEKC:
            ret = vm_handler_3peekc();
            break;
        case VM_INSTRUCTION_DEF:
            ret = vm_handler_def();
            break;
        case VM_INSTRUCTION_DEFA:
            ret = vm_handler_defa();
            break;
        case VM_INSTRUCTION_ALLOC:
            ret = vm_handler_alloc();
            break;
        case VM_INSTRUCTION_MULTCSD:
            ret = vm_handler_multcsd();
            break;
        case VM_INSTRUCTION_STD:
            ret = vm_handler_std();
            break;
        case VM_INSTRUCTION_LDD:
            ret = vm_handler_ldd();
            break;
        case VM_INSTRUCTION_ADDD:
            ret = vm_handler_addd();
            break;
        case VM_INSTRUCTION_SUBD:
            ret = vm_handler_subd();
            break;
        case VM_INSTRUCTION_MULTD:
            ret = vm_handler_multd();
            break;
        case VM_INSTRUCTION_DIVD:
            ret = vm_handler_divd();
            break;
        case VM_INSTRUCTION_INCD:
            ret = vm_handler_incd();
            break;
        case VM_INSTRUCTION_MODD:
            ret = vm_handler_modd();
            break;
        case VM_INSTRUCTION_DECD:
            ret = vm_handler_decd();
            break;
        case VM_INSTRUCTION_DUBD:
            ret = vm_handler_dubd();
            break;
        case VM_INSTRUCTION_HALFD:
            ret = vm_handler_halfd();
            break;
        case VM_INSTRUCTION_LSLD:
            ret = vm_handler_lsld();
            break;
        case VM_INSTRUCTION_LSRD:
            ret = vm_handler_lsrd();
            break;
        case VM_INSTRUCTION_ANDD:
            ret = vm_handler_andd();
            break;
        case VM_INSTRUCTION_ORD:
            ret = vm_handler_ord();
            break;
        case VM_INSTRUCTION_XORD:
            ret = vm_handler_xord();
            break;
        case VM_INSTRUCTION_NOTD:
            ret = vm_handler_notd();
            break;
        case VM_INSTRUCTION_NEGD:
            ret = vm_handler_negd();
            break;
        case VM_INSTRUCTION_GTD:
            ret = vm_handler_gtd();
            break;
        case VM_INSTRUCTION_LTD:
            ret = vm_handler_ltd();
            break;
        case VM_INSTRUCTION_EQD:
            ret = vm_handler_eqd();
            break;
        case VM_INSTRUCTION_ULTD:
            ret = vm_handler_ultd();
            break;
        case VM_INSTRUCTION_LTZD:
            ret = vm_handler_ltzd();
            break;
        case VM_INSTRUCTION_EQZD:
            ret = vm_handler_eqzd();
            break;
        case VM_INSTRUCTION_DUPD:
            ret = vm_handler_dupd();
            break;
        case VM_INSTRUCTION_DROPD:
            ret = vm_handler_dropd();
            break;
        case VM_INSTRUCTION_SWAPD:
            ret = vm_handler_swapd();
            break;
        }
    }
    return ret;
}

bool parse_token(char *buf, char *buf_end, bool line, FILE *fin);
void next_token(bool line, FILE *fin);
bool lookup_token(char *buf, uint8_t **meta, uintptr_t **addr);
bool parser_int10(char *buf, uintptr_t *out);
bool parser_int16(char *buf, uintptr_t *out);

void parser_initialize(void) {
    buf_end = buf + 100;
    parser_state = PARSER_STATE_INTERPRET;
    parser_status = PARSER_STATUS_END;
    parser_pending = 0;
    parser_conditional_stack_cur = parser_conditional_stack;
    parser_conditional_stack_end =
        parser_conditional_stack + (sizeof(uintptr_t) + 1) * 20;
    parser_loop_stack_cur = parser_loop_stack;
    parser_loop_stack_end =
        parser_loop_stack + (sizeof(uintptr_t) + 1) * 20;
}

int parser_parse(bool debug, bool line, FILE *fin) {
    bool ret = false;
    int ret_int = 0;
    uint8_t *meta = 0;
    uintptr_t *addr = 0;
    uintptr_t num = 0;
    char *iter = 0;
    parser_status = PARSER_STATUS_RUNNING;
    parser_eos = false;
    for (next_token(line, fin); !parser_status; next_token(line, fin)) {
        ret = parse_token(buf, buf_end, line, fin);
        if (debug) {
            printf("token: %s\n", buf);
        }
        if (!ret) {
            return PARSER_STATUS_TOKEN_TOO_LONG;
        }
        ret = lookup_token(buf, &meta, &addr);
        if (ret) {
            if (*meta == VM_LOOKUP_META_BUILTIN) {
                if (parser_state & PARSER_STATE_NAME) {
                    *meta = VM_LOOKUP_META_CALL;
                    *addr = (uintptr_t)vm_compiled_cur;
                    parser_state ^= PARSER_STATE_NAME;
                } else if (parser_state & PARSER_STATE_CREATE) {
                    if (
                        vm_interpreted_cur + sizeof(uintptr_t) >
                        vm_interpreted_end) {
                        parser_status = PARSER_STATUS_END;
                        ret_int = PARSER_STATUS_INTERPRETED_OVERFLOW;
                        break;
                    }
                    *meta = VM_LOOKUP_META_MEMORY;
                    *(uintptr_t*)vm_interpreted_cur = (uintptr_t)addr;
                    vm_interpreted_cur += sizeof(uintptr_t);
                    parser_state ^= PARSER_STATE_CREATE;
                } else {
                    switch (*addr) {
                    case PARSER_HANDLER_DOT:
                        ret_int = parser_handler_dot();
                        break;
                    case PARSER_HANDLER_EMIT:
                        ret_int = parser_handler_emit();
                        break;
                    case PARSER_HANDLER_CR:
                        ret_int = parser_handler_cr();
                        break;
                    case PARSER_HANDLER_COLON:
                        ret_int = parser_handler_colon();
                        break;
                    case PARSER_HANDLER_SEMI_COLON:
                        ret_int = parser_handler_semi_colon();
                        break;
                    case PARSER_HANDLER_IF:
                        ret_int = parser_handler_if();
                        break;
                    case PARSER_HANDLER_THEN:
                        ret_int = parser_handler_then();
                        break;
                    case PARSER_HANDLER_ELSE:
                        ret_int = parser_handler_else();
                        break;
                    case PARSER_HANDLER_DO:
                        ret_int = parser_handler_do();
                        break;
                    case PARSER_HANDLER_LOOP:
                        ret_int = parser_handler_loop();
                        break;
                    case PARSER_HANDLER_PLUS_LOOP:
                        ret_int = parser_handler_plus_loop();
                        break;
                    case PARSER_HANDLER_I:
                        ret_int = parser_handler_i();
                        break;
                    case PARSER_HANDLER_LEAVE:
                        ret_int = parser_handler_leave();
                        break;
                    case PARSER_HANDLER_UNLOOP:
                        ret_int = parser_handler_unloop();
                        break;
                    case PARSER_HANDLER_EXIT:
                        ret_int = parser_handler_exit();
                        break;
                    case PARSER_HANDLER_J:
                        ret_int = parser_handler_j();
                        break;
                    case PARSER_HANDLER_BEGIN:
                        ret_int = parser_handler_begin();
                        break;
                    case PARSER_HANDLER_WHILE:
                        ret_int = parser_handler_while();
                        break;
                    case PARSER_HANDLER_REPEAT:
                        ret_int = parser_handler_repeat();
                        break;
                    case PARSER_HANDLER_UNTIL:
                        ret_int = parser_handler_until();
                        break;
                    case PARSER_HANDLER_AGAIN:
                        ret_int = parser_handler_again();
                        break;
                    case PARSER_HANDLER_CREATE:
                        ret_int = parser_handler_create();
                        break;
                    case PARSER_HANDLER_VARIABLE:
                        ret_int = parser_handler_variable();
                        break;

                    case PARSER_HANDLER_ALLOT:
                        ret_int = parser_handler_allot();
                        break;
                    case PARSER_HANDLER_CELLS:
                        ret_int = parser_handler_cells();
                        break;
                    case PARSER_HANDLER_BANG:
                        ret_int = parser_handler_bang();
                        break;
                    case PARSER_HANDLER_AT:
                        ret_int = parser_handler_at();
                        break;
                    case PARSER_HANDLER_PLUS:
                        ret_int = parser_handler_plus();
                        break;
                    case PARSER_HANDLER_DASH:
                        ret_int = parser_handler_dash();
                        break;
                    case PARSER_HANDLER_ASTERISK:
                        ret_int = parser_handler_asterisk();
                        break;
                    case PARSER_HANDLER_SLASH:
                        ret_int = parser_handler_slash();
                        break;
                    case PARSER_HANDLER_MOD:
                        ret_int = parser_handler_mod();
                        break;
                    case PARSER_HANDLER_ONE_PLUS:
                        ret_int = parser_handler_one_plus();
                        break;
                    case PARSER_HANDLER_ONE_DASH:
                        ret_int = parser_handler_one_dash();
                        break;
                    case PARSER_HANDLER_TWO_ASTERISK:
                        ret_int = parser_handler_two_asterisk();
                        break;
                    case PARSER_HANDLER_TWO_SLASH:
                        ret_int = parser_handler_two_slash();
                        break;
                    case PARSER_HANDLER_LSFHIFT:
                        ret_int = parser_handler_lshift();
                        break;
                    case PARSER_HANDLER_RSFHIFT:
                        ret_int = parser_handler_rshift();
                        break;
                    case PARSER_HANDLER_AND:
                        ret_int = parser_handler_and();
                        break;
                    case PARSER_HANDLER_OR:
                        ret_int = parser_handler_or();
                        break;
                    case PARSER_HANDLER_XOR:
                        ret_int = parser_handler_xor();
                        break;
                    case PARSER_HANDLER_INVERT:
                        ret_int = parser_handler_invert();
                        break;
                    case PARSER_HANDLER_NEGATE:
                        ret_int = parser_handler_negate();
                        break;
                    case PARSER_HANDLER_GT:
                        ret_int = parser_handler_gt();
                        break;
                    case PARSER_HANDLER_LT:
                        ret_int = parser_handler_lt();
                        break;
                    case PARSER_HANDLER_EQ:
                        ret_int = parser_handler_eq();
                        break;
                    case PARSER_HANDLER_ULT:
                        ret_int = parser_handler_ult();
                        break;
                    case PARSER_HANDLER_ZERO_LT:
                        ret_int = parser_handler_zero_lt();
                        break;
                    case PARSER_HANDLER_ZERO_EQ:
                        ret_int = parser_handler_zero_eq();
                        break;
                    case PARSER_HANDLER_DUP:
                        ret_int = parser_handler_dup();
                        break;
                    case PARSER_HANDLER_DROP:
                        ret_int = parser_handler_drop();
                        break;
                    case PARSER_HANDLER_SWAP:
                        ret_int = parser_handler_swap();
                        break;
                    }
                }
            } else {
                if (parser_state & PARSER_STATE_NAME) {
                    parser_pending = meta;
                    *addr = (uintptr_t)vm_compiled_cur;
                    parser_state ^= PARSER_STATE_NAME;
                } else if (parser_state & PARSER_STATE_CREATE) {
                    if (
                        vm_interpreted_cur + sizeof(uintptr_t) >
                        vm_interpreted_end) {
                        parser_status = PARSER_STATUS_END;
                        ret_int = PARSER_STATUS_INTERPRETED_OVERFLOW;
                        break;
                    }
                    *meta = VM_LOOKUP_META_MEMORY;
                    *(uintptr_t*)vm_interpreted_cur = (uintptr_t)addr;
                    vm_interpreted_cur += sizeof(uintptr_t);
                    parser_state ^= PARSER_STATE_CREATE;
                } else {
                    if (meta == parser_pending) {
                        parser_status = PARSER_STATUS_END;
                        parser_pending = 0;
                        parser_state = PARSER_STATE_INTERPRET;
                        ret_int = PARSER_STATUS_PENDING_DEFINITION;
                        break;
                    }
                    if (*meta & VM_LOOKUP_META_MEMORY) {
                        if (parser_state & PARSER_STATE_INTERPRET) {
                            if (vm_interpreted_cur == vm_interpreted_end) {
                                parser_status = PARSER_STATUS_END;
                                ret_int = PARSER_STATUS_INTERPRETED_OVERFLOW;
                                break;
                            }
                            *(vm_interpreted_cur++) = VM_INSTRUCTION_PUSHID;
                            if (
                                vm_interpreted_cur + sizeof(uintptr_t) >=
                                vm_interpreted_end) {
                                parser_status = PARSER_STATUS_END;
                                ret_int =  PARSER_STATUS_INTERPRETED_OVERFLOW;
                                break;
                            }
                            *(uint8_t**)vm_interpreted_cur = (uint8_t*)addr;
                            vm_interpreted_cur += sizeof(uintptr_t);
                            if (vm_interpreted_cur == vm_interpreted_end) {
                                parser_status = PARSER_STATUS_END;
                                ret_int =  PARSER_STATUS_INTERPRETED_OVERFLOW;
                                break;
                            }
                        }
                        if (parser_state & PARSER_STATE_COMPILE) {
                            if (vm_compiled_cur == vm_interpreted_end) {
                                parser_status = PARSER_STATUS_END;
                                ret_int =  PARSER_STATUS_COMPILED_OVERFLOW;
                                break;
                            }
                            *(vm_compiled_cur++) = VM_INSTRUCTION_PUSHID;
                            if (
                                vm_compiled_cur + sizeof(uintptr_t) >=
                                vm_compiled_end) {
                                parser_status = PARSER_STATUS_END;
                                ret_int =  PARSER_STATUS_COMPILED_OVERFLOW;
                                break;
                            }
                            *(uint8_t**)vm_compiled_cur = (uint8_t*)addr;
                            vm_compiled_cur += sizeof(uintptr_t);
                            if (vm_compiled_cur == vm_compiled_end) {
                                parser_status = PARSER_STATUS_END;
                                ret_int =  PARSER_STATUS_COMPILED_OVERFLOW;
                                break;
                            }
                        }
                    }
                    if (*meta & VM_LOOKUP_META_CALL) {
                        if (parser_state & PARSER_STATE_INTERPRET) {
                            if (vm_interpreted_cur == vm_interpreted_end) {
                                parser_status = PARSER_STATUS_END;
                                ret_int = PARSER_STATUS_INTERPRETED_OVERFLOW;
                                break;
                            }
                            *(vm_interpreted_cur++) = VM_INSTRUCTION_CALL;
                            if (
                                vm_interpreted_cur + sizeof(uintptr_t) >=
                                vm_interpreted_end) {
                                parser_status = PARSER_STATUS_END;
                                ret_int =  PARSER_STATUS_INTERPRETED_OVERFLOW;
                                break;
                            }
                            *(uint8_t**)vm_interpreted_cur =
                                (uint8_t*)(*addr - 1);
                            vm_interpreted_cur += sizeof(uintptr_t);
                            if (vm_interpreted_cur == vm_interpreted_end) {
                                parser_status = PARSER_STATUS_END;
                                ret_int =  PARSER_STATUS_INTERPRETED_OVERFLOW;
                                break;
                            }
                        }
                        if (parser_state & PARSER_STATE_COMPILE) {
                            if (vm_compiled_cur == vm_interpreted_end) {
                                parser_status = PARSER_STATUS_END;
                                ret_int =  PARSER_STATUS_COMPILED_OVERFLOW;
                                break;
                            }
                            *(vm_compiled_cur++) = VM_INSTRUCTION_CALL;
                            if (
                                vm_compiled_cur + sizeof(uintptr_t) >=
                                vm_compiled_end) {
                                parser_status = PARSER_STATUS_END;
                                ret_int =  PARSER_STATUS_COMPILED_OVERFLOW;
                                break;
                            }
                            *(uint8_t**)vm_compiled_cur = (uint8_t*)(*addr - 1);
                            vm_compiled_cur += sizeof(uintptr_t);
                            if (vm_compiled_cur == vm_compiled_end) {
                                parser_status = PARSER_STATUS_END;
                                ret_int =  PARSER_STATUS_COMPILED_OVERFLOW;
                                break;
                            }
                        }
                    }
                }

            }
        } else {
            if (parser_state & PARSER_STATE_NAME) {
                if (vm_lookup_cur == vm_lookup_end) {
                    parser_status = PARSER_STATUS_END;
                    ret_int = PARSER_STATUS_LOOKUP_OVERFLOW;
                    break;
                }
                parser_pending = vm_lookup_cur;
                *(vm_lookup_cur++) = VM_LOOKUP_META_CALL;
                if (vm_lookup_cur + sizeof(uintptr_t) >= vm_lookup_end) {
                    parser_status = PARSER_STATUS_END;
                    ret_int = PARSER_STATUS_LOOKUP_OVERFLOW;
                    break;
                }
                *(uintptr_t*)vm_lookup_cur = (uintptr_t)vm_compiled_cur;
                vm_lookup_cur += sizeof(uintptr_t);
                for (iter = buf; *iter; ++iter) {
                    if (vm_lookup_cur == vm_lookup_end) {
                        parser_status = PARSER_STATUS_END;
                        ret_int = PARSER_STATUS_LOOKUP_OVERFLOW;
                        break;
                    }
                    *(vm_lookup_cur++) = *iter;
                }
                if (vm_lookup_cur == vm_lookup_end) {
                    parser_status = PARSER_STATUS_END;
                    ret_int = PARSER_STATUS_LOOKUP_OVERFLOW;
                    break;
                }
                *(vm_lookup_cur++) = 0;
                parser_state ^= PARSER_STATE_NAME;
            } else if (parser_state & PARSER_STATE_CREATE) {
                if (vm_lookup_cur == vm_lookup_end) {
                    parser_status = PARSER_STATUS_END;
                    ret_int = PARSER_STATUS_LOOKUP_OVERFLOW;
                    break;
                }
                *(vm_lookup_cur++) = VM_LOOKUP_META_MEMORY;
                if (vm_lookup_cur + sizeof(uintptr_t) >= vm_lookup_end) {
                    parser_status = PARSER_STATUS_END;
                    ret_int = PARSER_STATUS_LOOKUP_OVERFLOW;
                    break;
                }
                if (
                    vm_interpreted_cur + sizeof(uintptr_t) >
                    vm_interpreted_end) {
                    parser_status = PARSER_STATUS_END;
                    ret_int = PARSER_STATUS_INTERPRETED_OVERFLOW;
                    break;
                }
                *(uint8_t**)vm_interpreted_cur = vm_lookup_cur;
                vm_interpreted_cur += sizeof(uintptr_t);
                vm_lookup_cur += sizeof(uintptr_t);
                for (iter = buf; *iter; ++iter) {
                    if (vm_lookup_cur == vm_lookup_end) {
                        parser_status = PARSER_STATUS_END;
                        ret_int = PARSER_STATUS_LOOKUP_OVERFLOW;
                        break;
                    }
                    *(vm_lookup_cur++) = *iter;
                }
                if (vm_lookup_cur == vm_lookup_end) {
                    parser_status = PARSER_STATUS_END;
                    ret_int = PARSER_STATUS_LOOKUP_OVERFLOW;
                    break;
                }
                *(vm_lookup_cur++) = 0;
                parser_state ^= PARSER_STATE_CREATE;
            } else {
                switch (*vm_memory) {
                case 10:
                    ret = parser_int10(buf, &num);
                    if (!ret) {
                        ret_int = PARSER_STATUS_INVALID_BASE10;
                        break;
                    }
                    break;
                case 16:
                    ret = parser_int16(buf, &num);
                    if (!ret) {
                        ret_int = PARSER_STATUS_INVALID_BASE16;
                        break;
                    }
                    break;
                default:
                    ret_int = PARSER_STATUS_INVALID_BASE;
                    break;
                }
                if (ret_int) {
                    break;
                }
                if (parser_state & PARSER_STATE_INTERPRET) {
                    if (vm_interpreted_cur == vm_interpreted_end) {
                        parser_status = PARSER_STATUS_END;
                        return PARSER_STATUS_INTERPRETED_OVERFLOW;
                    }
                    *(vm_interpreted_cur++) = VM_INSTRUCTION_PUSHD;
                    if (
                        vm_interpreted_cur + sizeof(uintptr_t) >
                        vm_interpreted_end) {
                        parser_status = PARSER_STATUS_END;
                        return PARSER_STATUS_INTERPRETED_OVERFLOW;
                    }
                    *(uintptr_t*)vm_interpreted_cur = num;
                    vm_interpreted_cur += sizeof(uintptr_t);
                }
                if (parser_state & PARSER_STATE_COMPILE) {
                    if (vm_compiled_cur == vm_compiled_end) {
                        parser_status = PARSER_STATUS_END;
                        return PARSER_STATUS_COMPILED_OVERFLOW;
                    }
                    *(vm_compiled_cur++) = VM_INSTRUCTION_PUSHD;
                    if (
                        vm_compiled_cur + sizeof(uintptr_t) >
                        vm_compiled_end) {
                        parser_status = PARSER_STATUS_END;
                        return PARSER_STATUS_COMPILED_OVERFLOW;
                    }
                    *(uintptr_t*)vm_compiled_cur = num;
                    vm_compiled_cur += sizeof(uintptr_t);
                }
            }
        }
        if (parser_status) {
            break;
        }
    }
    if (ret_int) {
        return ret_int;
    }
    if (parser_status == PARSER_STATUS_REPL_END) {
        return PARSER_STATUS_END_REPL;
    }
    if (vm_interpreted_cur == vm_interpreted_end) {
        return PARSER_STATUS_INTERPRETED_OVERFLOW;
    }
    *(vm_interpreted_cur++) = VM_INSTRUCTION_HALT;
    return PARSER_STATUS_OK;
}


int vm_handler_2pushc(void) {
    if (vm_data_stack_cur - (sizeof(uintptr_t) << 1) < vm_data_stack) {
        vm_state = VM_STATE_HALTED;
        return VM_STATUS_DATA_STACK_UNDERFLOW;
    }
    vm_data_stack_cur -= (sizeof(uintptr_t) << 1);
    if (
        vm_control_stack_cur + (sizeof(uintptr_t) << 1) >
        vm_control_stack_end) {
        vm_state = VM_STATE_HALTED;
        return VM_STATUS_CONTROL_STACK_OVERFLOW;
    }
    *(uintptr_t*)vm_control_stack_cur = *(uintptr_t*)(vm_data_stack_cur);
    vm_control_stack_cur += sizeof(uintptr_t);
    *(uintptr_t*)vm_control_stack_cur =
        *(uintptr_t*)(vm_data_stack_cur + sizeof(uintptr_t));
    vm_control_stack_cur += sizeof(uintptr_t);
    return VM_STATUS_OK;
}

int vm_handler_2rmc(void) {
    if (vm_control_stack_cur - (sizeof(uintptr_t) << 1) < vm_control_stack) {
        vm_state = VM_STATE_HALTED;
        return VM_STATUS_CONTROL_STACK_UNDERFLOW;
    }
    vm_control_stack_cur -= (sizeof(uintptr_t) << 1);
    return VM_STATUS_OK;
}

int vm_handler_3peekc(void) {
    if (vm_control_stack_cur - 3 * sizeof(uintptr_t) < vm_control_stack) {
        vm_state = VM_STATE_HALTED;
        return VM_STATUS_CONTROL_STACK_UNDERFLOW;
    }
    if (vm_data_stack_cur == vm_data_stack_end) {
        vm_state = VM_STATE_HALTED;
        return VM_STATUS_DATA_STACK_OVERFLOW;
    }
    *(uintptr_t*)vm_data_stack_cur =
        *(uintptr_t*)(vm_control_stack_cur - 3 * sizeof(uintptr_t));
    vm_data_stack_cur += sizeof(uintptr_t);
    return VM_STATUS_OK;
}

int vm_handler_addc(void) {
    if (vm_control_stack_cur == vm_control_stack) {
        vm_state = VM_STATE_HALTED;
        return VM_STATUS_CONTROL_STACK_UNDERFLOW;
    }
    if (vm_data_stack_cur == vm_data_stack) {
        vm_state = VM_STATE_HALTED;
        return VM_STATUS_DATA_STACK_UNDERFLOW;
    }
    vm_data_stack_cur -= sizeof(uintptr_t);
    *(intptr_t*)(vm_control_stack_cur - sizeof(uintptr_t)) +=
        *(intptr_t*)vm_data_stack_cur;
    return VM_STATUS_OK;
}

int vm_handler_addd(void) {
    if (vm_data_stack_cur - (sizeof(uintptr_t) << 1) < vm_data_stack) {
        vm_state = VM_STATE_HALTED;
        return VM_STATUS_DATA_STACK_UNDERFLOW;
    }
    vm_data_stack_cur -= sizeof(uintptr_t);
    *(intptr_t*)(vm_data_stack_cur - sizeof(uintptr_t)) +=
        *(intptr_t*)vm_data_stack_cur;
    return VM_STATUS_OK;
}

int vm_handler_alloc(void) {
    if (vm_data_stack_cur == vm_data_stack) {
        vm_state = VM_STATE_HALTED;
        return VM_STATUS_DATA_STACK_UNDERFLOW;
    }
    vm_data_stack_cur -= sizeof(uintptr_t);
    if (
        *(uint8_t**)vm_memory_cur + *(intptr_t*)(vm_data_stack_cur) >
        vm_memory_end ||
        *(uint8_t**)vm_memory_cur + *(intptr_t*)(vm_data_stack_cur) <
        vm_memory) {
        vm_state = VM_STATE_HALTED;
        return VM_STATUS_INVALID_MEMORY_ALLOCATION;
    }
    *(uint8_t**)vm_memory_cur += *(intptr_t*)(vm_data_stack_cur);
    return VM_STATUS_OK;
}

int vm_handler_andd(void) {
    if (vm_data_stack_cur - (sizeof(uintptr_t) << 1) < vm_data_stack) {
        vm_state = VM_STATE_HALTED;
        return VM_STATUS_DATA_STACK_UNDERFLOW;
    }
    vm_data_stack_cur -= sizeof(uintptr_t);
    *(intptr_t*)(vm_data_stack_cur - sizeof(uintptr_t)) &=
        *(intptr_t*)vm_data_stack_cur;
    return VM_STATUS_OK;
}

int vm_handler_call(void) {
    if (vm_control_stack_cur + sizeof(uintptr_t) >= vm_control_stack_end) {
        vm_state = VM_STATE_HALTED;
        return VM_STATUS_CONTROL_STACK_OVERFLOW;
    }
    *(uintptr_t*)vm_control_stack_cur = (uintptr_t)(vm_ip + sizeof(uintptr_t));
    vm_control_stack_cur += sizeof(uintptr_t);
    vm_ip = *(uint8_t**)(vm_ip + 1);
    return VM_STATUS_OK;
}

int vm_handler_decd(void) {
    if (vm_data_stack_cur == vm_data_stack) {
        vm_state = VM_STATE_HALTED;
        return VM_STATUS_DATA_STACK_UNDERFLOW;
    }
    --(*(intptr_t*)(vm_data_stack_cur - sizeof(uintptr_t)));
    return VM_STATUS_OK;
}

int vm_handler_defa(void) {
    **(uint8_t***)(vm_ip + 1) = *(uint8_t**)vm_memory_cur;
    vm_ip += sizeof(uintptr_t);
    if (*(uint8_t**)vm_memory_cur + sizeof(uintptr_t) > vm_memory_end) {
        vm_state = VM_STATE_HALTED;
        return VM_STATUS_INVALID_MEMORY_ALLOCATION;
    }
    *(uint8_t**)vm_memory_cur += sizeof(uintptr_t);
    return VM_STATUS_OK;
}

int vm_handler_def(void) {
    **(uint8_t***)(vm_ip + 1) = *(uint8_t**)vm_memory_cur;
    vm_ip += sizeof(uintptr_t);
    return VM_STATUS_OK;
}

int vm_handler_divd(void) {
    if (vm_data_stack_cur - (sizeof(uintptr_t) << 1) < vm_data_stack) {
        vm_state = VM_STATE_HALTED;
        return VM_STATUS_DATA_STACK_UNDERFLOW;
    }
    vm_data_stack_cur -= sizeof(uintptr_t);
    *(intptr_t*)(vm_data_stack_cur - sizeof(uintptr_t)) /=
        *(intptr_t*)vm_data_stack_cur;
    return VM_STATUS_OK;
}

int vm_handler_dropd(void) {
    if (vm_data_stack_cur == vm_data_stack) {
        vm_state = VM_STATE_HALTED;
        return VM_STATUS_DATA_STACK_UNDERFLOW;
    }
    vm_data_stack_cur -= sizeof(uintptr_t);
    return VM_STATUS_OK;
}

int vm_handler_dubd(void) {
    if (vm_data_stack_cur == vm_data_stack) {
        vm_state = VM_STATE_HALTED;
        return VM_STATUS_DATA_STACK_UNDERFLOW;
    }
    *(intptr_t*)(vm_data_stack_cur - sizeof(uintptr_t)) <<= 1;
    return VM_STATUS_OK;
}

int vm_handler_dupd(void) {
    if (vm_data_stack_cur == vm_data_stack) {
        vm_state = VM_STATE_HALTED;
        return VM_STATUS_DATA_STACK_UNDERFLOW;
    }
    if (vm_data_stack_cur + sizeof(uintptr_t) > vm_data_stack_end) {
        vm_state = VM_STATE_HALTED;
        return VM_STATUS_DATA_STACK_OVERFLOW;
    }
    *(uintptr_t*)vm_data_stack_cur =
        *(uintptr_t*)(vm_data_stack_cur - sizeof(uintptr_t));
    vm_data_stack_cur += sizeof(uintptr_t);
    return VM_STATUS_OK;
}


int vm_handler_emit(void) {
    if (vm_data_stack_cur == vm_data_stack) {
        vm_state = VM_STATE_HALTED;
        return VM_STATUS_DATA_STACK_UNDERFLOW;
    }
    vm_data_stack_cur -= sizeof(uintptr_t);
    putchar(*(uintptr_t*)vm_data_stack_cur);
    return VM_STATUS_OK;
}

int vm_handler_eqd(void) {
    if (vm_data_stack_cur - (sizeof(uintptr_t) << 1) < vm_data_stack) {
        vm_state = VM_STATE_HALTED;
        return VM_STATUS_DATA_STACK_UNDERFLOW;
    }
    vm_data_stack_cur -= sizeof(uintptr_t);
    *(intptr_t*)(vm_data_stack_cur - sizeof(uintptr_t)) =
        *(intptr_t*)(vm_data_stack_cur - sizeof(uintptr_t)) ==
        *(intptr_t*)vm_data_stack_cur ? -1 : 0;
    return VM_STATUS_OK;
}

int vm_handler_eqzd(void) {
    if (vm_data_stack_cur == vm_data_stack) {
        vm_state = VM_STATE_HALTED;
        return VM_STATUS_DATA_STACK_UNDERFLOW;
    }
    *(intptr_t*)(vm_data_stack_cur - sizeof(uintptr_t)) =
        *(intptr_t*)(vm_data_stack_cur - sizeof(uintptr_t)) ? 0 : -1;
    return VM_STATUS_OK;
}

int vm_handler_gtd(void) {
    if (vm_data_stack_cur - (sizeof(uintptr_t) << 1) < vm_data_stack) {
        vm_state = VM_STATE_HALTED;
        return VM_STATUS_DATA_STACK_UNDERFLOW;
    }
    vm_data_stack_cur -= sizeof(uintptr_t);
    *(intptr_t*)(vm_data_stack_cur - sizeof(uintptr_t)) =
        *(intptr_t*)(vm_data_stack_cur - sizeof(uintptr_t)) >
        *(intptr_t*)vm_data_stack_cur ? -1 : 0;
    return VM_STATUS_OK;
}

int vm_handler_halfd(void) {
    if (vm_data_stack_cur == vm_data_stack) {
        vm_state = VM_STATE_HALTED;
        return VM_STATUS_DATA_STACK_UNDERFLOW;
    }
    *(intptr_t*)(vm_data_stack_cur - sizeof(uintptr_t)) >>= 1;
    return VM_STATUS_OK;
}

int vm_handler_incc(void) {
    if (vm_control_stack_cur == vm_control_stack) {
        vm_state = VM_STATE_HALTED;
        return VM_STATUS_CONTROL_STACK_UNDERFLOW;
    }
    ++(*(uintptr_t*)(vm_control_stack_cur - sizeof(uintptr_t)));
    return VM_STATUS_OK;
}

int vm_handler_incd(void) {
    if (vm_data_stack_cur == vm_data_stack) {
        vm_state = VM_STATE_HALTED;
        return VM_STATUS_DATA_STACK_UNDERFLOW;
    }
    ++(*(intptr_t*)(vm_data_stack_cur - sizeof(uintptr_t)));
    return VM_STATUS_OK;
}

int vm_handler_jmp(void) {
    vm_ip = *(uint8_t**)(vm_ip + 1);
    return VM_STATUS_OK;
}

int vm_handler_jnec(void) {
    if (vm_control_stack_cur - (sizeof(uintptr_t) << 1) < vm_control_stack) {
        vm_state = VM_STATE_HALTED;
        return VM_STATUS_CONTROL_STACK_UNDERFLOW;
    }
    if (
        *(intptr_t*)(vm_control_stack_cur - sizeof(uintptr_t)) !=
        *(intptr_t*)(vm_control_stack_cur - (sizeof(uintptr_t) << 1))) {
        vm_ip = *(uint8_t**)(vm_ip + 1);
    } else {
        vm_ip += sizeof(uintptr_t);
    }
    return VM_STATUS_OK;
}

int vm_handler_jnzd(void) {
    if (vm_data_stack_cur == vm_data_stack) {
        vm_state = VM_STATE_HALTED;
        return VM_STATUS_DATA_STACK_UNDERFLOW;
    }
    vm_data_stack_cur -= sizeof(uintptr_t);
    if (*(intptr_t*)vm_data_stack_cur) {
        vm_ip = *(uint8_t**)(vm_ip + 1);
    } else {
        vm_ip += sizeof(uintptr_t);
    }
    return VM_STATUS_OK;
}

int vm_handler_jzd(void) {
    if (vm_data_stack_cur == vm_data_stack) {
        vm_state = VM_STATE_HALTED;
        return VM_STATUS_DATA_STACK_UNDERFLOW;
    }
    vm_data_stack_cur -= sizeof(uintptr_t);
    if (!*(intptr_t*)vm_data_stack_cur) {
        vm_ip = *(uint8_t**)(vm_ip + 1);
    } else {
        vm_ip += sizeof(uintptr_t);
    }
    return VM_STATUS_OK;
}

int vm_handler_ldd(void) {
    if (vm_data_stack_cur == vm_data_stack) {
        vm_state = VM_STATE_HALTED;
        return VM_STATUS_DATA_STACK_UNDERFLOW;
    }
    if (
        *(uint8_t**)(vm_data_stack_cur - sizeof(uintptr_t)) <
        vm_memory ||
        *(uint8_t**)(vm_data_stack_cur - sizeof(uintptr_t)) >
        *(uint8_t**)vm_memory_cur) {
        vm_state = VM_STATE_HALTED;
        return VM_STATUS_INVALID_MEMORY_ADDRESS;
    }
    *(uintptr_t*)(vm_data_stack_cur - sizeof(uintptr_t)) =
        **(uintptr_t**)(vm_data_stack_cur - sizeof(uintptr_t));
    return VM_STATUS_OK;
}

int vm_handler_lsld(void) {
    if (vm_data_stack_cur - (sizeof(uintptr_t) << 1) < vm_data_stack) {
        vm_state = VM_STATE_HALTED;
        return VM_STATUS_DATA_STACK_UNDERFLOW;
    }
    vm_data_stack_cur -= sizeof(uintptr_t);
    *(intptr_t*)(vm_data_stack_cur - sizeof(uintptr_t)) <<=
        *(intptr_t*)vm_data_stack_cur;
    return VM_STATUS_OK;
}

int vm_handler_lsrd(void) {
    if (vm_data_stack_cur - (sizeof(uintptr_t) << 1) < vm_data_stack) {
        vm_state = VM_STATE_HALTED;
        return VM_STATUS_DATA_STACK_UNDERFLOW;
    }
    vm_data_stack_cur -= sizeof(uintptr_t);
    *(uintptr_t*)(vm_data_stack_cur - sizeof(uintptr_t)) >>=
        *(uintptr_t*)vm_data_stack_cur;
    return VM_STATUS_OK;
}

int vm_handler_ltd(void) {
    if (vm_data_stack_cur - (sizeof(uintptr_t) << 1) < vm_data_stack) {
        vm_state = VM_STATE_HALTED;
        return VM_STATUS_DATA_STACK_UNDERFLOW;
    }
    vm_data_stack_cur -= sizeof(uintptr_t);
    *(intptr_t*)(vm_data_stack_cur - sizeof(uintptr_t)) =
        *(intptr_t*)(vm_data_stack_cur - sizeof(uintptr_t)) <
        *(intptr_t*)vm_data_stack_cur ? -1 : 0;
    return VM_STATUS_OK;
}

int vm_handler_ltzd(void) {
    if (vm_data_stack_cur == vm_data_stack) {
        vm_state = VM_STATE_HALTED;
        return VM_STATUS_DATA_STACK_UNDERFLOW;
    }
    *(intptr_t*)(vm_data_stack_cur - sizeof(uintptr_t)) =
        *(intptr_t*)(vm_data_stack_cur - sizeof(uintptr_t)) < 0 ? -1 : 0;
    return VM_STATUS_OK;
}

int vm_handler_modd(void) {
    if (vm_data_stack_cur - (sizeof(uintptr_t) << 1) < vm_data_stack) {
        vm_state = VM_STATE_HALTED;
        return VM_STATUS_DATA_STACK_UNDERFLOW;
    }
    vm_data_stack_cur -= sizeof(uintptr_t);
    *(intptr_t*)(vm_data_stack_cur - sizeof(uintptr_t)) %=
        *(intptr_t*)vm_data_stack_cur;
    return VM_STATUS_OK;
}

int vm_handler_multcsd(void) {
    if (vm_data_stack_cur == vm_data_stack) {
        vm_state = VM_STATE_HALTED;
        return VM_STATUS_DATA_STACK_UNDERFLOW;
    }
    *(intptr_t*)(vm_data_stack_cur - sizeof(uintptr_t)) *= sizeof(uintptr_t);
    return VM_STATUS_OK;
}

int vm_handler_multd(void) {
    if (vm_data_stack_cur - (sizeof(uintptr_t) << 1) < vm_data_stack) {
        vm_state = VM_STATE_HALTED;
        return VM_STATUS_DATA_STACK_UNDERFLOW;
    }
    vm_data_stack_cur -= sizeof(uintptr_t);
    *(intptr_t*)(vm_data_stack_cur - sizeof(uintptr_t)) *=
        *(intptr_t*)vm_data_stack_cur;
    return VM_STATUS_OK;
}

int vm_handler_negd(void) {
    if (vm_data_stack_cur == vm_data_stack) {
        vm_state = VM_STATE_HALTED;
        return VM_STATUS_DATA_STACK_UNDERFLOW;
    }
    *(intptr_t*)(vm_data_stack_cur - sizeof(uintptr_t)) =
        -*(intptr_t*)(vm_data_stack_cur - sizeof(uintptr_t));
    return VM_STATUS_OK;
}

int vm_handler_notd(void) {
    if (vm_data_stack_cur == vm_data_stack) {
        vm_state = VM_STATE_HALTED;
        return VM_STATUS_DATA_STACK_UNDERFLOW;
    }
    *(intptr_t*)(vm_data_stack_cur - sizeof(uintptr_t)) =
        ~*(intptr_t*)(vm_data_stack_cur - sizeof(uintptr_t));
    return VM_STATUS_OK;
}

int vm_handler_ord(void) {
    if (vm_data_stack_cur - (sizeof(uintptr_t) << 1) < vm_data_stack) {
        vm_state = VM_STATE_HALTED;
        return VM_STATUS_DATA_STACK_UNDERFLOW;
    }
    vm_data_stack_cur -= sizeof(uintptr_t);
    *(intptr_t*)(vm_data_stack_cur - sizeof(uintptr_t)) |=
        *(intptr_t*)vm_data_stack_cur;
    return VM_STATUS_OK;
}

int vm_handler_peekc(void) {
    if (vm_control_stack_cur == vm_control_stack) {
        vm_state = VM_STATE_HALTED;
        return VM_STATUS_CONTROL_STACK_UNDERFLOW;
    }
    if (vm_data_stack_cur == vm_data_stack_end) {
        vm_state = VM_STATE_HALTED;
        return VM_STATUS_DATA_STACK_OVERFLOW;
    }
    *(uintptr_t*)vm_data_stack_cur =
        *(uintptr_t*)(vm_control_stack_cur - sizeof(uintptr_t));
    vm_data_stack_cur += sizeof(uintptr_t);
    return VM_STATUS_OK;
}


int vm_handler_pint(void) {
    if (vm_data_stack_cur == vm_data_stack) {
        vm_state = VM_STATE_HALTED;
        return VM_STATUS_DATA_STACK_UNDERFLOW;
    }
    vm_data_stack_cur -= sizeof(uintptr_t);
    switch (*vm_memory) {
    case 10:
        printf("%d", *(intptr_t*)vm_data_stack_cur);
        break;
    case 16:
        printf("0x%04X", *(intptr_t*)vm_data_stack_cur);
        break;
    default:
        vm_state = VM_STATE_HALTED;
        return VM_STATUS_INVALID_BASE;
    }
    return VM_STATUS_OK;
}

int vm_handler_pushd(void) {
    if (vm_data_stack_cur + sizeof(uintptr_t) > vm_data_stack_end) {
        vm_state = VM_STATE_HALTED;
        return VM_STATUS_DATA_STACK_OVERFLOW;
    }
    *(uintptr_t*)vm_data_stack_cur = *(uintptr_t*)(vm_ip + 1);
    vm_data_stack_cur += sizeof(uintptr_t);
    vm_ip += sizeof(uintptr_t);
    return VM_STATUS_OK;
}

int vm_handler_pushid(void) {
    if (vm_data_stack_cur + sizeof(uintptr_t) > vm_data_stack_end) {
        vm_state = VM_STATE_HALTED;
        return VM_STATUS_DATA_STACK_OVERFLOW;
    }
    *(uintptr_t*)vm_data_stack_cur = **(uintptr_t**)(vm_ip + 1);
    vm_data_stack_cur += sizeof(uintptr_t);
    vm_ip += sizeof(uintptr_t);
    return VM_STATUS_OK;
}

int vm_handler_ret(void) {
    if (vm_control_stack_cur == vm_control_stack) {
        vm_state = VM_STATE_HALTED;
        return VM_STATUS_CONTROL_STACK_UNDERFLOW;
    }
    vm_control_stack_cur -= sizeof(uintptr_t);
    if (
        !(
            (
                *(uint8_t**)vm_control_stack_cur >= vm_compiled &&
                *(uint8_t**)vm_control_stack_cur < vm_compiled_end
            ) ||
            (
                *(uint8_t**)vm_control_stack_cur >= vm_interpreted &&
                *(uint8_t**)vm_control_stack_cur < vm_interpreted_end
            )
        )) {
        vm_state = VM_STATE_HALTED;
        return VM_STATUS_INVALID_RETURN_ADDRESS;
    }
    vm_ip = *(uint8_t**)vm_control_stack_cur;
    return VM_STATUS_OK;
}

int vm_handler_std(void) {
    if (vm_data_stack_cur - (sizeof(uintptr_t) << 1) < vm_data_stack) {
        vm_state = VM_STATE_HALTED;
        return VM_STATUS_DATA_STACK_UNDERFLOW;
    }
    vm_data_stack_cur -= sizeof(uintptr_t) << 1;
    if (
        *(uint8_t**)(vm_data_stack_cur + sizeof(uintptr_t)) <
        vm_memory ||
        *(uint8_t**)(vm_data_stack_cur + sizeof(uintptr_t)) >
        *(uint8_t**)vm_memory_cur) {
        vm_state = VM_STATE_HALTED;
        return VM_STATUS_INVALID_MEMORY_ADDRESS;
    }
    **(uintptr_t**)(vm_data_stack_cur + sizeof(uintptr_t)) =
        *(uintptr_t*)vm_data_stack_cur;
    return VM_STATUS_OK;
}

int vm_handler_subd(void) {
    if (vm_data_stack_cur - (sizeof(uintptr_t) << 1) < vm_data_stack) {
        vm_state = VM_STATE_HALTED;
        return VM_STATUS_DATA_STACK_UNDERFLOW;
    }
    vm_data_stack_cur -= sizeof(uintptr_t);
    *(intptr_t*)(vm_data_stack_cur - sizeof(uintptr_t)) -=
        *(intptr_t*)vm_data_stack_cur;
    return VM_STATUS_OK;
}

int vm_handler_swapd(void) {
    if (vm_data_stack_cur - (sizeof(uintptr_t) << 1) < vm_data_stack) {
        vm_state = VM_STATE_HALTED;
        return VM_STATUS_DATA_STACK_UNDERFLOW;
    }
    *(uintptr_t*)(vm_data_stack_cur - sizeof(uintptr_t)) ^=
        *(uintptr_t*)(vm_data_stack_cur - (sizeof(uintptr_t) << 1));
    *(uintptr_t*)(vm_data_stack_cur - (sizeof(uintptr_t) << 1)) ^=
        *(uintptr_t*)(vm_data_stack_cur - sizeof(uintptr_t));
    *(uintptr_t*)(vm_data_stack_cur - sizeof(uintptr_t)) ^=
        *(uintptr_t*)(vm_data_stack_cur - (sizeof(uintptr_t) << 1));
    return VM_STATUS_OK;
}

int vm_handler_ultd(void) {
    if (vm_data_stack_cur - (sizeof(uintptr_t) << 1) < vm_data_stack) {
        vm_state = VM_STATE_HALTED;
        return VM_STATUS_DATA_STACK_UNDERFLOW;
    }
    vm_data_stack_cur -= sizeof(uintptr_t);
    *(uintptr_t*)(vm_data_stack_cur - sizeof(uintptr_t)) =
        *(uintptr_t*)(vm_data_stack_cur - sizeof(uintptr_t)) <
        *(uintptr_t*)vm_data_stack_cur ? -1 : 0;
    return VM_STATUS_OK;
}

int vm_handler_xord(void) {
    if (vm_data_stack_cur - (sizeof(uintptr_t) << 1) < vm_data_stack) {
        vm_state = VM_STATE_HALTED;
        return VM_STATUS_DATA_STACK_UNDERFLOW;
    }
    vm_data_stack_cur -= sizeof(uintptr_t);
    *(intptr_t*)(vm_data_stack_cur - sizeof(uintptr_t)) ^=
        *(intptr_t*)vm_data_stack_cur;
    return VM_STATUS_OK;
}


int parser_handler_again(void) {
    if (parser_state & PARSER_STATE_INTERPRET) {
        parser_status = PARSER_STATUS_END;
        return PARSER_STATUS_NOT_IN_COMPILATION_MODE;
    }
    if (
        parser_loop_stack_cur - (sizeof(uintptr_t) + 1) <
        parser_loop_stack) {
        parser_status = PARSER_STATUS_END;
        return PARSER_STATUS_CONTROL_STACK_UNDERFLOW;
    }
    parser_loop_stack_cur -= (sizeof(uintptr_t) + 1);
    if (*parser_loop_stack_cur != PARSER_CONTROL_BEGIN) {
        parser_status = PARSER_STATUS_END;
        return PARSER_STATUS_CONTROL_STACK_MISMATCH;
    }
    if (vm_compiled_cur == vm_compiled_end) {
        parser_status = PARSER_STATUS_END;
        return PARSER_STATUS_COMPILED_OVERFLOW;
    }
    *(vm_compiled_cur++) = VM_INSTRUCTION_JMP;
    *(uint8_t**)(vm_compiled_cur) =
        *(uint8_t**)(parser_loop_stack_cur + 1) - 1;
    if (vm_compiled_cur + sizeof(uintptr_t) > vm_compiled_end) {
        parser_status = PARSER_STATUS_END;
        return PARSER_STATUS_COMPILED_OVERFLOW;
    }
    vm_compiled_cur += sizeof(uintptr_t);
    return PARSER_STATUS_OK;
}

int parser_handler_allot(void) {
    if (parser_state & PARSER_STATE_INTERPRET) {
        if (vm_interpreted_cur == vm_interpreted_end) {
            parser_status = PARSER_STATUS_END;
            return PARSER_STATUS_INTERPRETED_OVERFLOW;
        }
        *(vm_interpreted_cur++) = VM_INSTRUCTION_ALLOC;
    }
    if (parser_state & PARSER_STATE_COMPILE) {
        if (vm_compiled_cur == vm_compiled_end) {
            parser_status = PARSER_STATUS_END;
            return PARSER_STATUS_COMPILED_OVERFLOW;
        }
        *(vm_compiled_cur++) = VM_INSTRUCTION_ALLOC;
    }
    return PARSER_STATUS_OK;
}

int parser_handler_and(void) {
    if (parser_state & PARSER_STATE_INTERPRET) {
        if (vm_interpreted_cur == vm_interpreted_end) {
            parser_status = PARSER_STATUS_END;
            return PARSER_STATUS_INTERPRETED_OVERFLOW;
        }
        *(vm_interpreted_cur++) = VM_INSTRUCTION_ANDD;
    }
    if (parser_state & PARSER_STATE_COMPILE) {
        if (vm_compiled_cur == vm_compiled_end) {
            parser_status = PARSER_STATUS_END;
            return PARSER_STATUS_COMPILED_OVERFLOW;
        }
        *(vm_compiled_cur++) = VM_INSTRUCTION_ANDD;
    }
    return PARSER_STATUS_OK;
}

int parser_handler_asterisk(void) {
    if (parser_state & PARSER_STATE_INTERPRET) {
        if (vm_interpreted_cur == vm_interpreted_end) {
            parser_status = PARSER_STATUS_END;
            return PARSER_STATUS_INTERPRETED_OVERFLOW;
        }
        *(vm_interpreted_cur++) = VM_INSTRUCTION_MULTD;
    }
    if (parser_state & PARSER_STATE_COMPILE) {
        if (vm_compiled_cur == vm_compiled_end) {
            parser_status = PARSER_STATUS_END;
            return PARSER_STATUS_COMPILED_OVERFLOW;
        }
        *(vm_compiled_cur++) = VM_INSTRUCTION_MULTD;
    }
    return PARSER_STATUS_OK;
}

int parser_handler_at(void) {
    if (parser_state & PARSER_STATE_INTERPRET) {
        if (vm_interpreted_cur == vm_interpreted_end) {
            parser_status = PARSER_STATUS_END;
            return PARSER_STATUS_INTERPRETED_OVERFLOW;
        }
        *(vm_interpreted_cur++) = VM_INSTRUCTION_LDD;
    }
    if (parser_state & PARSER_STATE_COMPILE) {
        if (vm_compiled_cur == vm_compiled_end) {
            parser_status = PARSER_STATUS_END;
            return PARSER_STATUS_COMPILED_OVERFLOW;
        }
        *(vm_compiled_cur++) = VM_INSTRUCTION_LDD;
    }
    return PARSER_STATUS_OK;
}

int parser_handler_bang(void) {
    if (parser_state & PARSER_STATE_INTERPRET) {
        if (vm_interpreted_cur == vm_interpreted_end) {
            parser_status = PARSER_STATUS_END;
            return PARSER_STATUS_INTERPRETED_OVERFLOW;
        }
        *(vm_interpreted_cur++) = VM_INSTRUCTION_STD;
    }
    if (parser_state & PARSER_STATE_COMPILE) {
        if (vm_compiled_cur == vm_compiled_end) {
            parser_status = PARSER_STATUS_END;
            return PARSER_STATUS_COMPILED_OVERFLOW;
        }
        *(vm_compiled_cur++) = VM_INSTRUCTION_STD;
    }
    return PARSER_STATUS_OK;
}

int parser_handler_begin(void) {
    if (parser_state & PARSER_STATE_INTERPRET) {
        parser_status = PARSER_STATUS_END;
        return PARSER_STATUS_NOT_IN_COMPILATION_MODE;
    }
    if (parser_loop_stack_cur == parser_loop_stack_end) {
        parser_status = PARSER_STATUS_END;
        return PARSER_STATUS_CONTROL_STACK_OVERFLOW;
    }
    *(parser_loop_stack_cur++) = PARSER_CONTROL_BEGIN;
    *(uint8_t**)parser_loop_stack_cur = vm_compiled_cur;
    parser_loop_stack_cur += sizeof(uintptr_t);
    return PARSER_STATUS_OK;
}

int parser_handler_cells(void) {
    if (parser_state & PARSER_STATE_INTERPRET) {
        if (vm_interpreted_cur == vm_interpreted_end) {
            parser_status = PARSER_STATUS_END;
            return PARSER_STATUS_INTERPRETED_OVERFLOW;
        }
        *(vm_interpreted_cur++) = VM_INSTRUCTION_MULTCSD;
    }
    if (parser_state & PARSER_STATE_COMPILE) {
        if (vm_compiled_cur == vm_compiled_end) {
            parser_status = PARSER_STATUS_END;
            return PARSER_STATUS_COMPILED_OVERFLOW;
        }
        *(vm_compiled_cur++) = VM_INSTRUCTION_MULTCSD;
    }
    return PARSER_STATUS_OK;
}

int parser_handler_colon(void) {
    if (parser_state & PARSER_STATE_COMPILE) {
        parser_status = PARSER_STATUS_END;
        return PARSER_STATUS_NESTED_COMPILATION_MODE;
    }
    parser_state ^= PARSER_STATE_INTERPRET;
    parser_state |= PARSER_STATE_COMPILE;
    parser_state |= PARSER_STATE_NAME;
    return PARSER_STATUS_OK;
}

int parser_handler_cr(void) {
    if (parser_state & PARSER_STATE_INTERPRET) {
        if (vm_interpreted_cur == vm_interpreted_end) {
            parser_status = PARSER_STATUS_END;
            return PARSER_STATUS_INTERPRETED_OVERFLOW;
        }
        *(vm_interpreted_cur++) = VM_INSTRUCTION_PUSHD;
        if (vm_interpreted_cur + sizeof(uintptr_t) > vm_interpreted_end) {
            parser_status = PARSER_STATUS_END;
            return PARSER_STATUS_INTERPRETED_OVERFLOW;
        }
        *(uintptr_t*)vm_interpreted_cur = 10;
        vm_interpreted_cur += sizeof(uintptr_t);
        if (vm_interpreted_cur == vm_interpreted_end) {
            parser_status = PARSER_STATUS_END;
            return PARSER_STATUS_INTERPRETED_OVERFLOW;
        }
        *(vm_interpreted_cur++) = VM_INSTRUCTION_EMIT;
    }
    if (parser_state & PARSER_STATE_COMPILE) {
        if (vm_compiled_cur == vm_interpreted_end) {
            parser_status = PARSER_STATUS_END;
            return PARSER_STATUS_COMPILED_OVERFLOW;
        }
        *(vm_compiled_cur++) = VM_INSTRUCTION_PUSHD;
        if (vm_compiled_cur + sizeof(uintptr_t) >= vm_compiled_end) {
            parser_status = PARSER_STATUS_END;
            return PARSER_STATUS_COMPILED_OVERFLOW;
        }
        *(uintptr_t*)vm_compiled_cur = 10;
        vm_compiled_cur += sizeof(uintptr_t);
        if (vm_compiled_cur == vm_compiled_end) {
            parser_status = PARSER_STATUS_END;
            return PARSER_STATUS_COMPILED_OVERFLOW;
        }
        *(vm_compiled_cur++) = VM_INSTRUCTION_EMIT;
    }
    return PARSER_STATUS_OK;
}

int parser_handler_create(void) {
    if (parser_state & PARSER_STATE_INTERPRET) {
        parser_state |= PARSER_STATE_CREATE;
        if (vm_interpreted_cur == vm_interpreted_end) {
            parser_status = PARSER_STATUS_END;
            return PARSER_STATUS_INTERPRETED_OVERFLOW;
        }
        *(vm_interpreted_cur++) = VM_INSTRUCTION_DEF;
    } else {
        *parser_pending |= VM_LOOKUP_META_MEMORY;
    }
    return PARSER_STATUS_OK;
}

int parser_handler_dash(void) {
    if (parser_state & PARSER_STATE_INTERPRET) {
        if (vm_interpreted_cur == vm_interpreted_end) {
            parser_status = PARSER_STATUS_END;
            return PARSER_STATUS_INTERPRETED_OVERFLOW;
        }
        *(vm_interpreted_cur++) = VM_INSTRUCTION_SUBD;
    }
    if (parser_state & PARSER_STATE_COMPILE) {
        if (vm_compiled_cur == vm_compiled_end) {
            parser_status = PARSER_STATUS_END;
            return PARSER_STATUS_COMPILED_OVERFLOW;
        }
        *(vm_compiled_cur++) = VM_INSTRUCTION_SUBD;
    }
    return PARSER_STATUS_OK;
}

int parser_handler_do(void) {
    if (parser_state & PARSER_STATE_INTERPRET) {
        parser_status = PARSER_STATUS_END;
        return PARSER_STATUS_NOT_IN_COMPILATION_MODE;
    }
    if (vm_compiled_cur == vm_compiled_end) {
        parser_status = PARSER_STATUS_END;
        return PARSER_STATUS_COMPILED_OVERFLOW;
    }
    *(vm_compiled_cur++) = VM_INSTRUCTION_2PUSHC;
    if (parser_loop_stack_cur == parser_loop_stack_end) {
        parser_status = PARSER_STATUS_END;
        return PARSER_STATUS_CONTROL_STACK_OVERFLOW;
    }
    *(parser_loop_stack_cur++) = PARSER_CONTROL_DO;
    *(uint8_t**)parser_loop_stack_cur = vm_compiled_cur;
    parser_loop_stack_cur += sizeof(uintptr_t);
    return PARSER_STATUS_OK;
}

int parser_handler_dot(void) {
    if (parser_state & PARSER_STATE_INTERPRET) {
        if (vm_interpreted_cur == vm_interpreted_end) {
            parser_status = PARSER_STATUS_END;
            return PARSER_STATUS_INTERPRETED_OVERFLOW;
        }
        *(vm_interpreted_cur++) = VM_INSTRUCTION_PINT;
    }
    if (parser_state & PARSER_STATE_COMPILE) {
        if (vm_compiled_cur == vm_compiled_end) {
            parser_status = PARSER_STATUS_END;
            return PARSER_STATUS_COMPILED_OVERFLOW;
        }
        *(vm_compiled_cur++) = VM_INSTRUCTION_PINT;
    }
    return PARSER_STATUS_OK;
}

int parser_handler_drop(void) {
    if (parser_state & PARSER_STATE_INTERPRET) {
        if (vm_interpreted_cur == vm_interpreted_end) {
            parser_status = PARSER_STATUS_END;
            return PARSER_STATUS_INTERPRETED_OVERFLOW;
        }
        *(vm_interpreted_cur++) = VM_INSTRUCTION_DROPD;
    }
    if (parser_state & PARSER_STATE_COMPILE) {
        if (vm_compiled_cur == vm_compiled_end) {
            parser_status = PARSER_STATUS_END;
            return PARSER_STATUS_COMPILED_OVERFLOW;
        }
        *(vm_compiled_cur++) = VM_INSTRUCTION_DROPD;
    }
    return PARSER_STATUS_OK;
}

int parser_handler_dup(void) {
    if (parser_state & PARSER_STATE_INTERPRET) {
        if (vm_interpreted_cur == vm_interpreted_end) {
            parser_status = PARSER_STATUS_END;
            return PARSER_STATUS_INTERPRETED_OVERFLOW;
        }
        *(vm_interpreted_cur++) = VM_INSTRUCTION_DUPD;
    }
    if (parser_state & PARSER_STATE_COMPILE) {
        if (vm_compiled_cur == vm_compiled_end) {
            parser_status = PARSER_STATUS_END;
            return PARSER_STATUS_COMPILED_OVERFLOW;
        }
        *(vm_compiled_cur++) = VM_INSTRUCTION_DUPD;
    }
    return PARSER_STATUS_OK;
}

int parser_handler_else(void) {
    if (parser_state & PARSER_STATE_INTERPRET) {
        parser_status = PARSER_STATUS_END;
        return PARSER_STATUS_NOT_IN_COMPILATION_MODE;
    }
    if (parser_conditional_stack_cur == parser_conditional_stack) {
        parser_status = PARSER_STATUS_END;
        return PARSER_STATUS_CONTROL_STACK_UNDERFLOW;
    }
    parser_conditional_stack_cur -= (sizeof(uintptr_t) + 1);
    if (
        *parser_conditional_stack_cur != PARSER_CONTROL_IF) {
        parser_status = PARSER_STATUS_END;
        return PARSER_STATUS_CONTROL_STACK_MISMATCH;
    }
    *parser_conditional_stack_cur = PARSER_CONTROL_ELSE;
    if (vm_compiled_cur == vm_compiled_end) {
        parser_status = PARSER_STATUS_END;
        return PARSER_STATUS_COMPILED_OVERFLOW;
    }
    *(vm_compiled_cur++) = VM_INSTRUCTION_JMP;
    if (parser_conditional_stack_cur == parser_conditional_stack_end) {
        parser_status = PARSER_STATUS_END;
        return PARSER_STATUS_CONTROL_STACK_OVERFLOW;
    }
    **(uint8_t***)(parser_conditional_stack_cur + 1) =
        vm_compiled_cur + sizeof(uintptr_t) - 1;
    *(uint8_t**)(parser_conditional_stack_cur + 1) = vm_compiled_cur;
    vm_compiled_cur += sizeof(uintptr_t);
    parser_conditional_stack_cur += (sizeof(uintptr_t) + 1);
    return PARSER_STATUS_OK;
}

int parser_handler_emit(void) {
    if (parser_state & PARSER_STATE_INTERPRET) {
        if (vm_interpreted_cur == vm_interpreted_end) {
            parser_status = PARSER_STATUS_END;
            return PARSER_STATUS_INTERPRETED_OVERFLOW;
        }
        *(vm_interpreted_cur++) = VM_INSTRUCTION_EMIT;
    }
    if (parser_state & PARSER_STATE_COMPILE) {
        if (vm_compiled_cur == vm_compiled_end) {
            parser_status = PARSER_STATUS_END;
            return PARSER_STATUS_COMPILED_OVERFLOW;
        }
        *(vm_compiled_cur++) = VM_INSTRUCTION_EMIT;
    }
    return PARSER_STATUS_OK;
}

int parser_handler_eq(void) {
    if (parser_state & PARSER_STATE_INTERPRET) {
        if (vm_interpreted_cur == vm_interpreted_end) {
            parser_status = PARSER_STATUS_END;
            return PARSER_STATUS_INTERPRETED_OVERFLOW;
        }
        *(vm_interpreted_cur++) = VM_INSTRUCTION_EQD;
    }
    if (parser_state & PARSER_STATE_COMPILE) {
        if (vm_compiled_cur == vm_compiled_end) {
            parser_status = PARSER_STATUS_END;
            return PARSER_STATUS_COMPILED_OVERFLOW;
        }
        *(vm_compiled_cur++) = VM_INSTRUCTION_EQD;
    }
    return PARSER_STATUS_OK;
}

int parser_handler_exit(void) {
    if (parser_state & PARSER_STATE_INTERPRET) {
        parser_status = PARSER_STATUS_END;
        return PARSER_STATUS_NOT_IN_COMPILATION_MODE;
    }
    if (vm_compiled_cur == vm_compiled_end) {
        parser_status = PARSER_STATUS_END;
        return PARSER_STATUS_COMPILED_OVERFLOW;
    }
    *(vm_compiled_cur++) = VM_INSTRUCTION_RET;
    return PARSER_STATUS_OK;
}

int parser_handler_gt(void) {
    if (parser_state & PARSER_STATE_INTERPRET) {
        if (vm_interpreted_cur == vm_interpreted_end) {
            parser_status = PARSER_STATUS_END;
            return PARSER_STATUS_INTERPRETED_OVERFLOW;
        }
        *(vm_interpreted_cur++) = VM_INSTRUCTION_GTD;
    }
    if (parser_state & PARSER_STATE_COMPILE) {
        if (vm_compiled_cur == vm_compiled_end) {
            parser_status = PARSER_STATUS_END;
            return PARSER_STATUS_COMPILED_OVERFLOW;
        }
        *(vm_compiled_cur++) = VM_INSTRUCTION_GTD;
    }
    return PARSER_STATUS_OK;
}

int parser_handler_i(void) {
    if (parser_state & PARSER_STATE_INTERPRET) {
        parser_status = PARSER_STATUS_END;
        return PARSER_STATUS_NOT_IN_COMPILATION_MODE;
    }
    if (vm_compiled_cur == vm_compiled_end) {
        parser_status = PARSER_STATUS_END;
        return PARSER_STATUS_COMPILED_OVERFLOW;
    }
    *(vm_compiled_cur++) = VM_INSTRUCTION_PEEKC;
    return PARSER_STATUS_OK;
}

int parser_handler_if(void) {
    if (parser_state & PARSER_STATE_INTERPRET) {
        parser_status = PARSER_STATUS_END;
        return PARSER_STATUS_NOT_IN_COMPILATION_MODE;
    }
    if (vm_compiled_cur == vm_compiled_end) {
        parser_status = PARSER_STATUS_END;
        return PARSER_STATUS_COMPILED_OVERFLOW;
    }
    *(vm_compiled_cur++) = VM_INSTRUCTION_JZD;
    if (parser_conditional_stack_cur == parser_conditional_stack_end) {
        parser_status = PARSER_STATUS_END;
        return PARSER_STATUS_CONTROL_STACK_OVERFLOW;
    }
    *(parser_conditional_stack_cur++) = PARSER_CONTROL_IF;
    *(uint8_t**)parser_conditional_stack_cur = vm_compiled_cur;
    parser_conditional_stack_cur += sizeof(uintptr_t);
    if (vm_compiled_cur + sizeof(uintptr_t) > vm_compiled_end) {
        parser_status = PARSER_STATUS_END;
        return PARSER_STATUS_COMPILED_OVERFLOW;
    }
    vm_compiled_cur += sizeof(uintptr_t);
    return PARSER_STATUS_OK;
}

int parser_handler_invert(void) {
    if (parser_state & PARSER_STATE_INTERPRET) {
        if (vm_interpreted_cur == vm_interpreted_end) {
            parser_status = PARSER_STATUS_END;
            return PARSER_STATUS_INTERPRETED_OVERFLOW;
        }
        *(vm_interpreted_cur++) = VM_INSTRUCTION_NOTD;
    }
    if (parser_state & PARSER_STATE_COMPILE) {
        if (vm_compiled_cur == vm_compiled_end) {
            parser_status = PARSER_STATUS_END;
            return PARSER_STATUS_COMPILED_OVERFLOW;
        }
        *(vm_compiled_cur++) = VM_INSTRUCTION_NOTD;
    }
    return PARSER_STATUS_OK;
}

int parser_handler_j(void) {
    if (parser_state & PARSER_STATE_INTERPRET) {
        parser_status = PARSER_STATUS_END;
        return PARSER_STATUS_NOT_IN_COMPILATION_MODE;
    }
    if (vm_compiled_cur == vm_compiled_end) {
        parser_status = PARSER_STATUS_END;
        return PARSER_STATUS_COMPILED_OVERFLOW;
    }
    *(vm_compiled_cur++) = VM_INSTRUCTION_3PEEKC;
    return PARSER_STATUS_OK;
}

int parser_handler_leave(void) {
    if (parser_state & PARSER_STATE_INTERPRET) {
        parser_status = PARSER_STATUS_END;
        return PARSER_STATUS_NOT_IN_COMPILATION_MODE;
    }
    if (parser_loop_stack_cur == parser_loop_stack) {
        parser_status = PARSER_STATUS_END;
        return PARSER_STATUS_CONTROL_STACK_UNDERFLOW;
    }
    parser_loop_stack_cur -= (sizeof(uintptr_t) + 1);
    if (
        *parser_loop_stack_cur != PARSER_CONTROL_DO &&
        *parser_loop_stack_cur != PARSER_CONTROL_DO_LEAVE) {
            parser_status = PARSER_STATUS_END;
        return PARSER_STATUS_CONTROL_STACK_MISMATCH;
    }
    parser_loop_stack_cur += (sizeof(uintptr_t) + 1);
    if (parser_loop_stack_cur == parser_loop_stack_end) {
        parser_status = PARSER_STATUS_END;
        return PARSER_STATUS_CONTROL_STACK_OVERFLOW;
    }
    *(parser_loop_stack_cur++) = PARSER_CONTROL_DO_LEAVE;
    if (vm_compiled_cur == vm_compiled_end) {
        parser_status = PARSER_STATUS_END;
        return PARSER_STATUS_COMPILED_OVERFLOW;
    }
    *(vm_compiled_cur++) = VM_INSTRUCTION_JMP;
    if (
        parser_loop_stack_cur + sizeof(uintptr_t) >
        parser_loop_stack_end) {
        return PARSER_STATUS_CONTROL_STACK_OVERFLOW;
    }
    *(uint8_t**)parser_loop_stack_cur = vm_compiled_cur;
    parser_loop_stack_cur += sizeof(uintptr_t);
    if (
        vm_compiled_cur + sizeof(uintptr_t) > vm_compiled_end) {
        parser_status = PARSER_STATUS_END;
        return PARSER_STATUS_COMPILED_OVERFLOW;
    }
    vm_compiled_cur += sizeof(uintptr_t);
    return PARSER_STATUS_OK;
}

int parser_handler_loop(void) {
    if (parser_state & PARSER_STATE_INTERPRET) {
        parser_status = PARSER_STATUS_END;
        return PARSER_STATUS_NOT_IN_COMPILATION_MODE;
    }
    if (parser_loop_stack_cur == parser_loop_stack) {
        parser_status = PARSER_STATUS_END;
        return PARSER_STATUS_CONTROL_STACK_UNDERFLOW;
    }
    parser_loop_stack_cur -= (sizeof(uintptr_t) + 1);
    if (
        *parser_loop_stack_cur != PARSER_CONTROL_DO &&
        *parser_loop_stack_cur != PARSER_CONTROL_DO_LEAVE) {
        parser_status = PARSER_STATUS_END;
        return PARSER_STATUS_CONTROL_STACK_MISMATCH;
    }
    for (
        ;
        parser_loop_stack_cur >= parser_loop_stack &&
        *parser_loop_stack_cur == PARSER_CONTROL_DO_LEAVE;
        parser_loop_stack_cur -= (sizeof(uintptr_t) + 1)) {
        **(uint8_t***)(parser_loop_stack_cur + 1) =
            vm_compiled_cur + sizeof(uintptr_t) + 1;
    }
    if (*parser_loop_stack_cur != PARSER_CONTROL_DO) {
        parser_status = PARSER_STATUS_END;
        return PARSER_STATUS_CONTROL_STACK_MISMATCH;
    }
    if (vm_compiled_cur == vm_compiled_end) {
        parser_status = PARSER_STATUS_END;
        return PARSER_STATUS_COMPILED_OVERFLOW;
    }
    *(vm_compiled_cur++) = VM_INSTRUCTION_INCC;
    if (vm_compiled_cur == vm_compiled_end) {
        parser_status = PARSER_STATUS_END;
        return PARSER_STATUS_COMPILED_OVERFLOW;
    }
    *(vm_compiled_cur++) = VM_INSTRUCTION_JNEC;
    *(uint8_t**)(vm_compiled_cur) =
        *(uint8_t**)(parser_loop_stack_cur + 1) - 1;
    if (vm_compiled_cur + sizeof(uintptr_t) > vm_compiled_end) {
        parser_status = PARSER_STATUS_END;
        return PARSER_STATUS_COMPILED_OVERFLOW;
    }
    vm_compiled_cur += sizeof(uintptr_t);
    if (vm_compiled_cur == vm_compiled_end) {
        parser_status = PARSER_STATUS_END;
        return PARSER_STATUS_COMPILED_OVERFLOW;
    }
    *(vm_compiled_cur++) = VM_INSTRUCTION_2RMC;
    return PARSER_STATUS_OK;
}

int parser_handler_lshift(void) {
    if (parser_state & PARSER_STATE_INTERPRET) {
        if (vm_interpreted_cur == vm_interpreted_end) {
            parser_status = PARSER_STATUS_END;
            return PARSER_STATUS_INTERPRETED_OVERFLOW;
        }
        *(vm_interpreted_cur++) = VM_INSTRUCTION_LSLD;
    }
    if (parser_state & PARSER_STATE_COMPILE) {
        if (vm_compiled_cur == vm_compiled_end) {
            parser_status = PARSER_STATUS_END;
            return PARSER_STATUS_COMPILED_OVERFLOW;
        }
        *(vm_compiled_cur++) = VM_INSTRUCTION_LSLD;
    }
    return PARSER_STATUS_OK;
}

int parser_handler_lt(void) {
    if (parser_state & PARSER_STATE_INTERPRET) {
        if (vm_interpreted_cur == vm_interpreted_end) {
            parser_status = PARSER_STATUS_END;
            return PARSER_STATUS_INTERPRETED_OVERFLOW;
        }
        *(vm_interpreted_cur++) = VM_INSTRUCTION_LTD;
    }
    if (parser_state & PARSER_STATE_COMPILE) {
        if (vm_compiled_cur == vm_compiled_end) {
            parser_status = PARSER_STATUS_END;
            return PARSER_STATUS_COMPILED_OVERFLOW;
        }
        *(vm_compiled_cur++) = VM_INSTRUCTION_LTD;
    }
    return PARSER_STATUS_OK;
}

int parser_handler_mod(void) {
    if (parser_state & PARSER_STATE_INTERPRET) {
        if (vm_interpreted_cur == vm_interpreted_end) {
            parser_status = PARSER_STATUS_END;
            return PARSER_STATUS_INTERPRETED_OVERFLOW;
        }
        *(vm_interpreted_cur++) = VM_INSTRUCTION_MODD;
    }
    if (parser_state & PARSER_STATE_COMPILE) {
        if (vm_compiled_cur == vm_compiled_end) {
            parser_status = PARSER_STATUS_END;
            return PARSER_STATUS_COMPILED_OVERFLOW;
        }
        *(vm_compiled_cur++) = VM_INSTRUCTION_MODD;
    }
    return PARSER_STATUS_OK;
}

int parser_handler_negate(void) {
    if (parser_state & PARSER_STATE_INTERPRET) {
        if (vm_interpreted_cur == vm_interpreted_end) {
            parser_status = PARSER_STATUS_END;
            return PARSER_STATUS_INTERPRETED_OVERFLOW;
        }
        *(vm_interpreted_cur++) = VM_INSTRUCTION_NEGD;
    }
    if (parser_state & PARSER_STATE_COMPILE) {
        if (vm_compiled_cur == vm_compiled_end) {
            parser_status = PARSER_STATUS_END;
            return PARSER_STATUS_COMPILED_OVERFLOW;
        }
        *(vm_compiled_cur++) = VM_INSTRUCTION_NEGD;
    }
    return PARSER_STATUS_OK;
}

int parser_handler_one_dash(void) {
    if (parser_state & PARSER_STATE_INTERPRET) {
        if (vm_interpreted_cur == vm_interpreted_end) {
            parser_status = PARSER_STATUS_END;
            return PARSER_STATUS_INTERPRETED_OVERFLOW;
        }
        *(vm_interpreted_cur++) = VM_INSTRUCTION_DECD;
    }
    if (parser_state & PARSER_STATE_COMPILE) {
        if (vm_compiled_cur == vm_compiled_end) {
            parser_status = PARSER_STATUS_END;
            return PARSER_STATUS_COMPILED_OVERFLOW;
        }
        *(vm_compiled_cur++) = VM_INSTRUCTION_DECD;
    }
    return PARSER_STATUS_OK;
}

int parser_handler_one_plus(void) {
    if (parser_state & PARSER_STATE_INTERPRET) {
        if (vm_interpreted_cur == vm_interpreted_end) {
            parser_status = PARSER_STATUS_END;
            return PARSER_STATUS_INTERPRETED_OVERFLOW;
        }
        *(vm_interpreted_cur++) = VM_INSTRUCTION_INCD;
    }
    if (parser_state & PARSER_STATE_COMPILE) {
        if (vm_compiled_cur == vm_compiled_end) {
            parser_status = PARSER_STATUS_END;
            return PARSER_STATUS_COMPILED_OVERFLOW;
        }
        *(vm_compiled_cur++) = VM_INSTRUCTION_INCD;
    }
    return PARSER_STATUS_OK;
}

int parser_handler_or(void) {
    if (parser_state & PARSER_STATE_INTERPRET) {
        if (vm_interpreted_cur == vm_interpreted_end) {
            parser_status = PARSER_STATUS_END;
            return PARSER_STATUS_INTERPRETED_OVERFLOW;
        }
        *(vm_interpreted_cur++) = VM_INSTRUCTION_ORD;
    }
    if (parser_state & PARSER_STATE_COMPILE) {
        if (vm_compiled_cur == vm_compiled_end) {
            parser_status = PARSER_STATUS_END;
            return PARSER_STATUS_COMPILED_OVERFLOW;
        }
        *(vm_compiled_cur++) = VM_INSTRUCTION_ORD;
    }
    return PARSER_STATUS_OK;
}

int parser_handler_plus(void) {
    if (parser_state & PARSER_STATE_INTERPRET) {
        if (vm_interpreted_cur == vm_interpreted_end) {
            parser_status = PARSER_STATUS_END;
            return PARSER_STATUS_INTERPRETED_OVERFLOW;
        }
        *(vm_interpreted_cur++) = VM_INSTRUCTION_ADDD;
    }
    if (parser_state & PARSER_STATE_COMPILE) {
        if (vm_compiled_cur == vm_compiled_end) {
            parser_status = PARSER_STATUS_END;
            return PARSER_STATUS_COMPILED_OVERFLOW;
        }
        *(vm_compiled_cur++) = VM_INSTRUCTION_ADDD;
    }
    return PARSER_STATUS_OK;
}

int parser_handler_plus_loop(void) {
    if (parser_state & PARSER_STATE_INTERPRET) {
        parser_status = PARSER_STATUS_END;
        return PARSER_STATUS_NOT_IN_COMPILATION_MODE;
    }
    if (parser_loop_stack_cur == parser_loop_stack) {
        parser_status = PARSER_STATUS_END;
        return PARSER_STATUS_CONTROL_STACK_UNDERFLOW;
    }
    parser_loop_stack_cur -= (sizeof(uintptr_t) + 1);
    if (
        *parser_loop_stack_cur != PARSER_CONTROL_DO &&
        *parser_loop_stack_cur != PARSER_CONTROL_DO_LEAVE) {
        parser_status = PARSER_STATUS_END;
        return PARSER_STATUS_CONTROL_STACK_MISMATCH;
    }
    for (
        ;
        parser_loop_stack_cur >= parser_loop_stack &&
        *parser_loop_stack_cur == PARSER_CONTROL_DO_LEAVE;
        parser_loop_stack_cur -= (sizeof(uintptr_t) + 1)) {
        **(uint8_t***)(parser_loop_stack_cur + 1) =
            vm_compiled_cur + sizeof(uintptr_t) + 1;
    }
    if (*parser_loop_stack_cur != PARSER_CONTROL_DO) {
        parser_status = PARSER_STATUS_END;
        return PARSER_STATUS_CONTROL_STACK_MISMATCH;
    }
    if (vm_compiled_cur == vm_compiled_end) {
        parser_status = PARSER_STATUS_END;
        return PARSER_STATUS_COMPILED_OVERFLOW;
    }
    *(vm_compiled_cur++) = VM_INSTRUCTION_ADDC;
    if (vm_compiled_cur == vm_compiled_end) {
        parser_status = PARSER_STATUS_END;
        return PARSER_STATUS_COMPILED_OVERFLOW;
    }
    *(vm_compiled_cur++) = VM_INSTRUCTION_JNEC;
    *(uint8_t**)(vm_compiled_cur) =
        *(uint8_t**)(parser_loop_stack_cur + 1) - 1;
    vm_compiled_cur += sizeof(uintptr_t);
    if (vm_compiled_cur == vm_compiled_end) {
        parser_status = PARSER_STATUS_END;
        return PARSER_STATUS_COMPILED_OVERFLOW;
    }
    *(vm_compiled_cur++) = VM_INSTRUCTION_2RMC;
    return PARSER_STATUS_OK;
}

int parser_handler_repeat(void) {
    if (parser_state & PARSER_STATE_INTERPRET) {
        parser_status = PARSER_STATUS_END;
        return PARSER_STATUS_NOT_IN_COMPILATION_MODE;
    }
    if (
        parser_loop_stack_cur - ((sizeof(uintptr_t) + 1) << 1) <
        parser_loop_stack) {
        parser_status = PARSER_STATUS_END;
        return PARSER_STATUS_CONTROL_STACK_UNDERFLOW;
    }
    parser_loop_stack_cur -= (sizeof(uintptr_t) + 1) << 1;
    if (
        *parser_loop_stack_cur != PARSER_CONTROL_BEGIN ||
        *(parser_loop_stack_cur + (sizeof(uintptr_t) + 1)) !=
        PARSER_CONTROL_WHILE) {
        parser_status = PARSER_STATUS_END;
        return PARSER_STATUS_CONTROL_STACK_MISMATCH;
    }
    if (vm_compiled_cur == vm_compiled_end) {
        parser_status = PARSER_STATUS_END;
        return PARSER_STATUS_COMPILED_OVERFLOW;
    }
    *(vm_compiled_cur++) = VM_INSTRUCTION_JMP;
    *(uint8_t**)(vm_compiled_cur) =
        *(uint8_t**)(parser_loop_stack_cur + 1) - 1;
    if (vm_compiled_cur + sizeof(uintptr_t) > vm_compiled_end) {
        parser_status = PARSER_STATUS_END;
        return PARSER_STATUS_COMPILED_OVERFLOW;
    }
    vm_compiled_cur += sizeof(uintptr_t);
    **(uint8_t***)(parser_loop_stack_cur + (sizeof(uintptr_t) + 2)) =
        vm_compiled_cur - 1;
    return PARSER_STATUS_OK;
}

int parser_handler_rshift(void) {
    if (parser_state & PARSER_STATE_INTERPRET) {
        if (vm_interpreted_cur == vm_interpreted_end) {
            parser_status = PARSER_STATUS_END;
            return PARSER_STATUS_INTERPRETED_OVERFLOW;
        }
        *(vm_interpreted_cur++) = VM_INSTRUCTION_LSRD;
    }
    if (parser_state & PARSER_STATE_COMPILE) {
        if (vm_compiled_cur == vm_compiled_end) {
            parser_status = PARSER_STATUS_END;
            return PARSER_STATUS_COMPILED_OVERFLOW;
        }
        *(vm_compiled_cur++) = VM_INSTRUCTION_LSRD;
    }
    return PARSER_STATUS_OK;
}

int parser_handler_semi_colon(void) {
    if (parser_state & PARSER_STATE_INTERPRET) {
        parser_status = PARSER_STATUS_END;
        return PARSER_STATUS_NOT_IN_COMPILATION_MODE;
    }
    if (
        parser_conditional_stack_cur != parser_conditional_stack ||
        parser_loop_stack_cur != parser_loop_stack) {
        parser_status = PARSER_STATUS_END;
        return PARSER_STATUS_CONTROL_STACK_MISMATCH;
    }
    if (vm_compiled_cur == vm_compiled_end) {
        parser_status = PARSER_STATUS_END;
        return PARSER_STATUS_COMPILED_OVERFLOW;
    }
    *(vm_compiled_cur++) = VM_INSTRUCTION_RET;
    parser_state ^= PARSER_STATE_COMPILE;
    parser_state |= PARSER_STATE_INTERPRET;
    parser_pending = 0;
    return PARSER_STATUS_OK;
}

int parser_handler_slash(void) {
    if (parser_state & PARSER_STATE_INTERPRET) {
        if (vm_interpreted_cur == vm_interpreted_end) {
            parser_status = PARSER_STATUS_END;
            return PARSER_STATUS_INTERPRETED_OVERFLOW;
        }
        *(vm_interpreted_cur++) = VM_INSTRUCTION_DIVD;
    }
    if (parser_state & PARSER_STATE_COMPILE) {
        if (vm_compiled_cur == vm_compiled_end) {
            parser_status = PARSER_STATUS_END;
            return PARSER_STATUS_COMPILED_OVERFLOW;
        }
        *(vm_compiled_cur++) = VM_INSTRUCTION_DIVD;
    }
    return PARSER_STATUS_OK;
}

int parser_handler_swap(void) {
    if (parser_state & PARSER_STATE_INTERPRET) {
        if (vm_interpreted_cur == vm_interpreted_end) {
            parser_status = PARSER_STATUS_END;
            return PARSER_STATUS_INTERPRETED_OVERFLOW;
        }
        *(vm_interpreted_cur++) = VM_INSTRUCTION_SWAPD;
    }
    if (parser_state & PARSER_STATE_COMPILE) {
        if (vm_compiled_cur == vm_compiled_end) {
            parser_status = PARSER_STATUS_END;
            return PARSER_STATUS_COMPILED_OVERFLOW;
        }
        *(vm_compiled_cur++) = VM_INSTRUCTION_SWAPD;
    }
    return PARSER_STATUS_OK;
}

int parser_handler_then(void) {
    if (parser_state & PARSER_STATE_INTERPRET) {
        return PARSER_STATUS_NOT_IN_COMPILATION_MODE;
    }
    if (parser_conditional_stack_cur == parser_conditional_stack) {
        return PARSER_STATUS_CONTROL_STACK_UNDERFLOW;
    }
    parser_conditional_stack_cur -= (sizeof(uintptr_t) + 1);
    if (
        *parser_conditional_stack_cur != PARSER_CONTROL_IF &&
        *parser_conditional_stack_cur != PARSER_CONTROL_ELSE) {
        return PARSER_STATUS_CONTROL_STACK_MISMATCH;
    }
    **(uint8_t***)(parser_conditional_stack_cur + 1) = vm_compiled_cur - 1;
    return PARSER_STATUS_OK;
}

int parser_handler_two_asterisk(void) {
    if (parser_state & PARSER_STATE_INTERPRET) {
        if (vm_interpreted_cur == vm_interpreted_end) {
            parser_status = PARSER_STATUS_END;
            return PARSER_STATUS_INTERPRETED_OVERFLOW;
        }
        *(vm_interpreted_cur++) = VM_INSTRUCTION_DUBD;
    }
    if (parser_state & PARSER_STATE_COMPILE) {
        if (vm_compiled_cur == vm_compiled_end) {
            parser_status = PARSER_STATUS_END;
            return PARSER_STATUS_COMPILED_OVERFLOW;
        }
        *(vm_compiled_cur++) = VM_INSTRUCTION_DUBD;
    }
    return PARSER_STATUS_OK;
}

int parser_handler_two_slash(void) {
    if (parser_state & PARSER_STATE_INTERPRET) {
        if (vm_interpreted_cur == vm_interpreted_end) {
            parser_status = PARSER_STATUS_END;
            return PARSER_STATUS_INTERPRETED_OVERFLOW;
        }
        *(vm_interpreted_cur++) = VM_INSTRUCTION_HALFD;
    }
    if (parser_state & PARSER_STATE_COMPILE) {
        if (vm_compiled_cur == vm_compiled_end) {
            parser_status = PARSER_STATUS_END;
            return PARSER_STATUS_COMPILED_OVERFLOW;
        }
        *(vm_compiled_cur++) = VM_INSTRUCTION_HALFD;
    }
    return PARSER_STATUS_OK;
}

int parser_handler_ult(void) {
    if (parser_state & PARSER_STATE_INTERPRET) {
        if (vm_interpreted_cur == vm_interpreted_end) {
            parser_status = PARSER_STATUS_END;
            return PARSER_STATUS_INTERPRETED_OVERFLOW;
        }
        *(vm_interpreted_cur++) = VM_INSTRUCTION_ULTD;
    }
    if (parser_state & PARSER_STATE_COMPILE) {
        if (vm_compiled_cur == vm_compiled_end) {
            parser_status = PARSER_STATUS_END;
            return PARSER_STATUS_COMPILED_OVERFLOW;
        }
        *(vm_compiled_cur++) = VM_INSTRUCTION_ULTD;
    }
    return PARSER_STATUS_OK;
}

int parser_handler_unloop(void) {
    if (parser_state & PARSER_STATE_INTERPRET) {
        parser_status = PARSER_STATUS_END;
        return PARSER_STATUS_NOT_IN_COMPILATION_MODE;
    }
    if (vm_compiled_cur == vm_compiled_end) {
        parser_status = PARSER_STATUS_END;
        return PARSER_STATUS_COMPILED_OVERFLOW;
    }
    *(vm_compiled_cur++) = VM_INSTRUCTION_2RMC;
    return PARSER_STATUS_OK;
}

int parser_handler_until(void) {
    if (parser_state & PARSER_STATE_INTERPRET) {
        parser_status = PARSER_STATUS_END;
        return PARSER_STATUS_NOT_IN_COMPILATION_MODE;
    }
    if (
        parser_loop_stack_cur - (sizeof(uintptr_t) + 1) <
        parser_loop_stack) {
        parser_status = PARSER_STATUS_END;
        return PARSER_STATUS_CONTROL_STACK_UNDERFLOW;
    }
    parser_loop_stack_cur -= (sizeof(uintptr_t) + 1);
    if (*parser_loop_stack_cur != PARSER_CONTROL_BEGIN) {
        parser_status = PARSER_STATUS_END;
        return PARSER_STATUS_CONTROL_STACK_MISMATCH;
    }
    if (vm_compiled_cur == vm_compiled_end) {
        parser_status = PARSER_STATUS_END;
        return PARSER_STATUS_COMPILED_OVERFLOW;
    }
    *(vm_compiled_cur++) = VM_INSTRUCTION_JZD;
    *(uint8_t**)(vm_compiled_cur) =
        *(uint8_t**)(parser_loop_stack_cur + 1) - 1;
    if (vm_compiled_cur + sizeof(uintptr_t) > vm_compiled_end) {
        parser_status = PARSER_STATUS_END;
        return PARSER_STATUS_COMPILED_OVERFLOW;
    }
    vm_compiled_cur += sizeof(uintptr_t);
    return PARSER_STATUS_OK;
}

int parser_handler_variable(void) {
    if (parser_state & PARSER_STATE_INTERPRET) {
        parser_state |= PARSER_STATE_CREATE;
        if (vm_interpreted_cur == vm_interpreted_end) {
            parser_status = PARSER_STATUS_END;
            return PARSER_STATUS_INTERPRETED_OVERFLOW;
        }
        *(vm_interpreted_cur++) = VM_INSTRUCTION_DEFA;
    } else {
        *parser_pending |= VM_LOOKUP_META_MEMORY;
    }
    return PARSER_STATUS_OK;
}

int parser_handler_while(void) {
    if (parser_state & PARSER_STATE_INTERPRET) {
        parser_status = PARSER_STATUS_END;
        return PARSER_STATUS_NOT_IN_COMPILATION_MODE;
    }
    if (parser_loop_stack_cur == parser_loop_stack) {
        parser_status = PARSER_STATUS_END;
        return PARSER_STATUS_CONTROL_STACK_UNDERFLOW;
    }
    if (
        *(parser_loop_stack_cur - sizeof(uintptr_t) - 1) !=
        PARSER_CONTROL_BEGIN) {
        parser_status = PARSER_STATUS_END;
        return PARSER_STATUS_CONTROL_STACK_UNDERFLOW;
    }
    if (parser_loop_stack_cur == parser_loop_stack_end) {
        parser_status = PARSER_STATUS_END;
        return PARSER_STATUS_CONTROL_STACK_OVERFLOW;
    }
    *(parser_loop_stack_cur++) = PARSER_CONTROL_WHILE;
    if (vm_compiled_cur == vm_compiled_end) {
        parser_status = PARSER_STATUS_END;
        return PARSER_STATUS_COMPILED_OVERFLOW;
    }
    *(vm_compiled_cur++) = VM_INSTRUCTION_JZD;
    if (parser_loop_stack_cur + sizeof(uintptr_t) > parser_loop_stack_end) {
        parser_status = PARSER_STATUS_END;
        return PARSER_STATUS_CONTROL_STACK_OVERFLOW;
    }
    *(uint8_t**)parser_loop_stack_cur = vm_compiled_cur;
    parser_loop_stack_cur += sizeof(uintptr_t);
    if (vm_compiled_cur + sizeof(uintptr_t) > vm_compiled_end) {
        parser_status = PARSER_STATUS_END;
        return PARSER_STATUS_COMPILED_OVERFLOW;
    }
    vm_compiled_cur += sizeof(uintptr_t);
    return PARSER_STATUS_OK;
}

int parser_handler_xor(void) {
    if (parser_state & PARSER_STATE_INTERPRET) {
        if (vm_interpreted_cur == vm_interpreted_end) {
            parser_status = PARSER_STATUS_END;
            return PARSER_STATUS_INTERPRETED_OVERFLOW;
        }
        *(vm_interpreted_cur++) = VM_INSTRUCTION_XORD;
    }
    if (parser_state & PARSER_STATE_COMPILE) {
        if (vm_compiled_cur == vm_compiled_end) {
            parser_status = PARSER_STATUS_END;
            return PARSER_STATUS_COMPILED_OVERFLOW;
        }
        *(vm_compiled_cur++) = VM_INSTRUCTION_XORD;
    }
    return PARSER_STATUS_OK;
}

int parser_handler_zero_eq(void) {
    if (parser_state & PARSER_STATE_INTERPRET) {
        if (vm_interpreted_cur == vm_interpreted_end) {
            parser_status = PARSER_STATUS_END;
            return PARSER_STATUS_INTERPRETED_OVERFLOW;
        }
        *(vm_interpreted_cur++) = VM_INSTRUCTION_EQZD;
    }
    if (parser_state & PARSER_STATE_COMPILE) {
        if (vm_compiled_cur == vm_compiled_end) {
            parser_status = PARSER_STATUS_END;
            return PARSER_STATUS_COMPILED_OVERFLOW;
        }
        *(vm_compiled_cur++) = VM_INSTRUCTION_EQZD;
    }
    return PARSER_STATUS_OK;
}

int parser_handler_zero_lt(void) {
    if (parser_state & PARSER_STATE_INTERPRET) {
        if (vm_interpreted_cur == vm_interpreted_end) {
            parser_status = PARSER_STATUS_END;
            return PARSER_STATUS_INTERPRETED_OVERFLOW;
        }
        *(vm_interpreted_cur++) = VM_INSTRUCTION_LTZD;
    }
    if (parser_state & PARSER_STATE_COMPILE) {
        if (vm_compiled_cur == vm_compiled_end) {
            parser_status = PARSER_STATUS_END;
            return PARSER_STATUS_COMPILED_OVERFLOW;
        }
        *(vm_compiled_cur++) = VM_INSTRUCTION_LTZD;
    }
    return PARSER_STATUS_OK;
}

bool parse_token(char *buf, char *buf_end, bool line, FILE *fin) {
    int in = 0;
    for (
        in = fgetc(fin);
        in != -1 &&
        in != '\n' &&
        in != '\t' &&
        in != ' ' &&
        buf != buf_end;
        in = fgetc(fin),
        ++buf) {
        *buf = in;
    }
    if (buf == buf_end) {
        return false;
    }
    *buf = 0;
    if (line) {
        switch (in) {
        case -1:
            parser_status = PARSER_STATUS_REPL_END;
            break;
        case '\n':
            parser_status = PARSER_STATUS_END;
            parser_eos = true;
            break;
        }
    } else {
        if (in == -1) {
            parser_status = PARSER_STATUS_END;
        }
    }
    return true;
}

void next_token(bool line, FILE *fin) {
    int in = 0;
    for (
        in = fgetc(fin);
        in == '\t' ||
        in == ' ';
        in = fgetc(fin));
    if (line) {
        switch (in) {
        case -1:
            parser_status = PARSER_STATUS_REPL_END;
            return;
        case '\n':
            parser_status = PARSER_STATUS_END;
            parser_eos = true;
            return;
        }
    } else {
        if (in == -1) {
            parser_status = PARSER_STATUS_END;
            return;
        }
    }
    ungetc(in, fin);
}

bool lookup_token(char *buf, uint8_t **meta, uintptr_t **addr) {
    uint8_t *iter = vm_lookup;
    for (; iter < vm_lookup_cur; ++iter) {
        if (!strcmp(buf, (const char*)(iter + sizeof(uintptr_t) + 1))) {
            *meta = iter;
            *addr = (uintptr_t*)(iter + 1);
            return true;
        }
        iter += sizeof(uintptr_t) + 1;
        for (; *iter && iter < vm_lookup_cur; ++iter);
    }
    return false;
}

bool parser_int10(char *buf, uintptr_t *out) {
    char *iter = buf;
    bool neg = false;
    *out = 0;
    for (; *iter; ++iter) {
        if (!neg && iter == buf && *iter == '-') {
            neg = true;
            continue;
        }
        if (*iter < '0' || *iter > '9') {
            return false;
        }
        *out *= 10;
        *out += *iter - '0';
    }
    if (neg) {
        *out = -*out;
    }
    return true;
}

bool parser_int16(char *buf, uintptr_t *out) {
    char *iter = buf;
    *out = 0;
    for (; *iter; ++iter) {
        *out <<= 4;
        if (*iter >= '0' && *iter <= '9') {
            *out += *iter - '0';
        } else if (*iter >= 'A' && *iter <= 'F') {
            *out += *iter - 'A' + 0xA;
        } else {
            return false;
        }
    }
    return true;
}

