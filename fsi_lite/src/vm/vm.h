#ifndef VM_VM_H_
#define VM_VM_H_

#include <stdint.h>
#include <stdbool.h>

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

extern uint8_t *vm_lookup;
extern uint8_t *vm_lookup_cur;
extern uint8_t *vm_lookup_end;
extern uint8_t *vm_literal;
extern uint8_t *vm_literal_cur;
extern uint8_t *vm_literal_end;
extern uint8_t *vm_compiled;
extern uint8_t *vm_compiled_cur;
extern uint8_t *vm_compiled_end;
extern uint8_t *vm_interpreted;
extern uint8_t *vm_interpreted_cur;
extern uint8_t *vm_interpreted_end;
extern uint8_t *vm_memory;
extern uint8_t *vm_memory_cur;
extern uint8_t *vm_memory_end;
extern uint8_t *vm_data_stack;
extern uint8_t *vm_data_stack_cur;
extern uint8_t *vm_data_stack_end;
extern uint8_t *vm_control_stack;
extern uint8_t *vm_control_stack_cur;
extern uint8_t *vm_control_stack_end;

extern uint8_t *vm_ip;
extern uint8_t vm_state;

void vm_initialize(void);
void vm_log(void);
void vm_reset(void);
int vm_run(bool debug);

#endif
