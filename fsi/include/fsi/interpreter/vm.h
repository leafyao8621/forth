#ifndef INTERPRETER_VM_H_
#define INTERPRETER_VM_H_

#include <fsi/util/containers.h>
#include <fsi/util/errcode.h>

#define OFFSET_BUILTIN 0x0000000000000001
#define OFFSET_MEMORY 0x0000000000000002
#define OFFSET_PENDING 0x0000000000000004
#define OFFSET_DOES 0x0000000000000008

#define OPCODE_TERMINATE 0
#define OPCODE_PRINT_STRING 1
#define OPCODE_CARRIAGE_RETURN 2
#define OPCODE_PUSH 3
#define OPCODE_PRINT_INT 4
#define OPCODE_CALL 5
#define OPCODE_RETURN 6
#define OPCODE_ADD 7
#define OPCODE_SUBTRACT 8
#define OPCODE_MULTIPLY 9
#define OPCODE_DIVIDE 10
#define OPCODE_INCREMENT 11
#define OPCODE_DECREMENT 12

#define IP_COMPILED 0x8000000000000000

typedef struct ForthVM {
    String words;
    DArrayOffset offset;
    String offset_flags;
    String literal;
    String compiled;
    String interpreted;
    size_t ip;
    DArrayOffset data_stack;
    DArrayOffset return_stack;
    DArrayOffset control_stack;
    String memory;
} ForthVM;

ForthVMErr ForthVM_initialize(ForthVM *vm);
void ForthVM_finalize(ForthVM *vm);
ForthVMErr ForthVM_load(ForthVM *vm, char *str);
void ForthVM_log(ForthVM *vm);
ForthVMErr ForthVM_execute(ForthVM *vm);

#endif
