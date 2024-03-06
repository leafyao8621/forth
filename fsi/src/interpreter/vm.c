#include <fsi/interpreter/vm.h>
#include <fsi/parser/parser.h>

ForthVMErr ForthVM_initialize(ForthVM *vm) {
    if (!vm) {
        return FORTHVM_ERR_NULL_PTR;
    }
    int ret = DArrayChar_initialize(&vm->words, 1);
    if (ret) {
        return FORTHVM_ERR_OUT_OF_MEMORY;
    }
    ret = DArrayOffset_initialize(&vm->offset, 1);
    if (ret) {
        return FORTHVM_ERR_OUT_OF_MEMORY;
    }
    ret = DArrayChar_initialize(&vm->compiled, 1);
    if (ret) {
        return FORTHVM_ERR_OUT_OF_MEMORY;
    }
    ret = DArrayChar_initialize(&vm->interpreted, 1);
    if (ret) {
        return FORTHVM_ERR_OUT_OF_MEMORY;
    }
    vm->ip = 0;
    ret = DArrayOffset_initialize(&vm->operation_stack, 1);
    if (ret) {
        return FORTHVM_ERR_OUT_OF_MEMORY;
    }
    ret = DArrayOffset_initialize(&vm->control_stack, 1);
    if (ret) {
        return FORTHVM_ERR_OUT_OF_MEMORY;
    }
    ret = DArrayOffset_initialize(&vm->return_stack, 1);
    if (ret) {
        return FORTHVM_ERR_OUT_OF_MEMORY;
    }
    ret = DArrayChar_initialize(&vm->memory, 1);
    if (ret) {
        return FORTHVM_ERR_OUT_OF_MEMORY;
    }
    return FORTHVM_ERR_OK;
}

void ForthVM_finalize(ForthVM *vm) {
    DArrayChar_finalize(&vm->words);
    DArrayOffset_finalize(&vm->offset);
    DArrayChar_finalize(&vm->compiled);
    DArrayChar_finalize(&vm->interpreted);
    DArrayOffset_finalize(&vm->operation_stack);
    DArrayOffset_finalize(&vm->control_stack);
    DArrayOffset_finalize(&vm->return_stack);
    DArrayChar_finalize(&vm->memory);
}

ForthVMErr ForthVM_load(ForthVM *vm, char *str) {
    if (!vm || !str) {
        return FORTHVM_ERR_NULL_PTR;
    }
    ForthParser parser;
    ForthVMErr err = ForthParser_initialize(&parser);
    if (err) {
        ForthParser_finalize(&parser);
        return err;
    }
    err = ForthParser_parse(&parser, str, vm);
    ForthParser_finalize(&parser);
    return err;
}
