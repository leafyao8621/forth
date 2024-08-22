#ifndef VM_HANDLER_H_
#define VM_HANDLER_H_

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

#endif
