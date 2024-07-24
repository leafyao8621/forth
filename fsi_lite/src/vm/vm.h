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
#define VM_INSTRUCTION_PINT 3
#define VM_INSTRUCTION_EMIT 4

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
