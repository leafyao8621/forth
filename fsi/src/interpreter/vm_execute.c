#include <stdio.h>

#include <fsi/interpreter/vm.h>
#include <fsi/util/interpreter_handlers.h>

static ForthInterpreterHandler interpreter_handler[3] =
    {
        NULL,
        interpreter_handle_print_string,
        interpreter_handle_carriage_return
    };

ForthVMErr ForthVM_execute(ForthVM *vm) {
    char opcode = 0;
    ForthVMErr err = FORTHVM_ERR_OK;
    for (
        ;
        !(vm->ip & IP_COMPILED) &&
        (vm->interpreted.data[vm->ip] != OPCODE_TERMINATE);
        vm->ip = (vm->ip & IP_COMPILED) | ((vm->ip & ~IP_COMPILED) + 1)) {
        opcode =
            !(vm->ip & IP_COMPILED) ?
            vm->interpreted.data[vm->ip] :
            vm->compiled.data[vm->ip & ~IP_COMPILED];
        err = interpreter_handler[(size_t)opcode](vm);
        if (err) {
            return err;
        }
    }
    return FORTHVM_ERR_OK;
}
