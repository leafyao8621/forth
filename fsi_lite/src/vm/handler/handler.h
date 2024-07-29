#ifndef VM_HANDLER_H_
#define VM_HANDLER_H_

int vm_handler_ret(void);
int vm_handler_pushd(void);
int vm_handler_pint(void);
int vm_handler_emit(void);
int vm_handler_call(void);
int vm_handler_jzd(void);
int vm_handler_jmp(void);
int vm_handler_2pushc(void);
int vm_handler_peekc(void);
int vm_handler_2rmc(void);
int vm_handler_jnec(void);
int vm_handler_incc(void);
int vm_handler_addc(void);

#endif
