#ifndef UTIL_STATUS_H_
#define UTIL_STATUS_H_

#define PARSER_STATUS_OK 0
#define PARSER_STATUS_END_REPL 1
#define PARSER_STATUS_TOKEN_TOO_LONG 2
#define PARSER_STATUS_INTERPRETED_OVERFLOW 3
#define PARSER_STATUS_COMPILED_OVERFLOW 4
#define PARSER_STATUS_INVALID_BASE 5
#define PARSER_STATUS_INVALID_BASE10 6

#define VM_STATUS_OK 0
#define VM_STATUS_DATA_STACK_OVERFLOW 1
#define VM_STATUS_DATA_STACK_UNDERFLOW 2
#define VM_STATUS_INVALID_BASE 3

extern const char *parser_status_lookup[7];
extern const char *vm_status_lookup[4];

#endif
