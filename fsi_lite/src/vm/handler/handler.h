#ifndef VM_HANDLER_H_
#define VM_HANDLER_H_

int vm_handler_ret(void);
int vm_handler_pushd(void);
int vm_handler_pint(void);
int vm_handler_emit(void);
int vm_handler_call(void);
int vm_handler_jz(void);
int vm_handler_jmp(void);

#endif
