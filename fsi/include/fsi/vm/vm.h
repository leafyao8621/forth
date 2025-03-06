#ifndef VM_VM_H_
#define VM_VM_H_

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include <fsi/util/status.h>

#define VM_LOOKUP_META_BUILTIN 0x1
#define VM_LOOKUP_META_MEMORY 0x2
#define VM_LOOKUP_META_CALL 0x4
#define VM_LOOKUP_META_CREATE 0x8
#define VM_LOOKUP_META_DOES 0x10
#define VM_LOOKUP_META_INDIRECT 0x20

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
#define VM_INSTRUCTION_JNED 10
#define VM_INSTRUCTION_2PUSHR 11
#define VM_INSTRUCTION_PEEKR 12
#define VM_INSTRUCTION_2RMR 13
#define VM_INSTRUCTION_JNER 14
#define VM_INSTRUCTION_INCR 15
#define VM_INSTRUCTION_ADDR 16
#define VM_INSTRUCTION_3PEEKR 17
#define VM_INSTRUCTION_DEF 18
#define VM_INSTRUCTION_DEFA 19
#define VM_INSTRUCTION_ALLOC 20
#define VM_INSTRUCTION_ALLOCC 21
#define VM_INSTRUCTION_MULTCSD 22
#define VM_INSTRUCTION_LDD 23
#define VM_INSTRUCTION_STD 24
#define VM_INSTRUCTION_LDCD 25
#define VM_INSTRUCTION_STCD 26
#define VM_INSTRUCTION_2LDD 27
#define VM_INSTRUCTION_2STD 28
#define VM_INSTRUCTION_INCMD 29
#define VM_INSTRUCTION_PUSHM 30
#define VM_INSTRUCTION_PUSHCM 31
#define VM_INSTRUCTION_ADDD 32
#define VM_INSTRUCTION_SUBD 33
#define VM_INSTRUCTION_MULTD 34
#define VM_INSTRUCTION_DIVD 35
#define VM_INSTRUCTION_MODD 36
#define VM_INSTRUCTION_INCD 37
#define VM_INSTRUCTION_INCCD 38
#define VM_INSTRUCTION_DECD 39
#define VM_INSTRUCTION_DUBD 40
#define VM_INSTRUCTION_HALFD 41
#define VM_INSTRUCTION_LSLD 42
#define VM_INSTRUCTION_LSRD 43
#define VM_INSTRUCTION_ANDD 44
#define VM_INSTRUCTION_ORD 45
#define VM_INSTRUCTION_XORD 46
#define VM_INSTRUCTION_NOTD 47
#define VM_INSTRUCTION_NEGD 48
#define VM_INSTRUCTION_GTD 49
#define VM_INSTRUCTION_LTD 50
#define VM_INSTRUCTION_EQD 51
#define VM_INSTRUCTION_ULTD 52
#define VM_INSTRUCTION_LTZD 53
#define VM_INSTRUCTION_EQZD 54
#define VM_INSTRUCTION_MIND 55
#define VM_INSTRUCTION_MAXD 56
#define VM_INSTRUCTION_DUPD 57
#define VM_INSTRUCTION_DROPD 58
#define VM_INSTRUCTION_SWAPD 59
#define VM_INSTRUCTION_OVERD 60
#define VM_INSTRUCTION_NIPD 61
#define VM_INSTRUCTION_2DUPD 62
#define VM_INSTRUCTION_2DROPD 63
#define VM_INSTRUCTION_2SWAPD 64
#define VM_INSTRUCTION_2OVERD 65
#define VM_INSTRUCTION_DUPNZD 66
#define VM_INSTRUCTION_ROTD 67
#define VM_INSTRUCTION_PICKD 68
#define VM_INSTRUCTION_PUSHDD 69
#define VM_INSTRUCTION_PUSHR 70
#define VM_INSTRUCTION_POPR 71
#define VM_INSTRUCTION_2POPR 72

typedef enum ForthVMState {
    VM_STATE_RUNNING,
    VM_STATE_HALTED
} ForthVMState;

typedef struct ForthVM {
    uint8_t *mem;
    uint8_t *lookup;
    uint8_t *lookup_cur;
    uint8_t *lookup_end;
    uint8_t *literal;
    uint8_t *literal_cur;
    uint8_t *literal_end;
    uint8_t *interpreted;
    uint8_t *interpreted_cur;
    uint8_t *interpreted_end;
    uint8_t *data_stack;
    uint8_t *data_stack_cur;
    uint8_t *data_stack_end;
    uint8_t *return_stack;
    uint8_t *return_stack_cur;
    uint8_t *return_stack_end;
    uint8_t *compiled;
    uint8_t *compiled_cur;
    uint8_t *compiled_end;
    uint8_t *memory;
    uint8_t *memory_cur;
    uint8_t *memory_end;
    uint8_t *ip;
    ForthVMState state;
} ForthVM;

ForthVMStatus vm_initialize(
    ForthVM *vm,
    size_t memory,
    size_t lookup,
    size_t literal,
    size_t interpreted,
    size_t data_stack,
    size_t return_stack,
    size_t compiled);
void vm_finalize(ForthVM *vm);
void vm_log(ForthVM *vm);
void vm_reset(ForthVM *vm);
ForthVMStatus vm_run(ForthVM *vm, bool debug);

#endif
