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
    char *keywords[5] =
        {
            ".\"",
            "cr",
            ".",
            ":",
            ";"
        };
    char **iter_keywords = keywords;
    size_t offset = 0;
    char chr = 0;
    for (size_t i = 0; i < 5; ++i, ++iter_keywords) {
        ret =
            DArrayChar_push_back_batch(
                &vm->words, *iter_keywords, strlen(*iter_keywords));
        if (ret) {
            return FORTHVM_ERR_OUT_OF_MEMORY;
        }
        ret = DArrayChar_push_back(&vm->words, &chr);
        if (ret) {
            return FORTHVM_ERR_OUT_OF_MEMORY;
        }
        offset = i | OFFSET_BUILTIN;
        ret = DArrayOffset_push_back(&vm->offset, &offset);
        if (ret) {
            return FORTHVM_ERR_OUT_OF_MEMORY;
        }
    }
    ret = DArrayChar_push_back_batch(&vm->words, "base", 5);
    if (ret) {
        return FORTHVM_ERR_OUT_OF_MEMORY;
    }
    size_t cell = OFFSET_MEMORY;
    ret = DArrayOffset_push_back(&vm->offset, &cell);
    if (ret) {
        return FORTHVM_ERR_OUT_OF_MEMORY;
    }
    ret = DArrayChar_initialize(&vm->literal, 1);
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
    ret = DArrayChar_initialize(&vm->memory, sizeof(size_t));
    if (ret) {
        return FORTHVM_ERR_OUT_OF_MEMORY;
    }
    cell = 10;
    ret = DArrayChar_push_back_batch(&vm->memory, (char*)&cell, sizeof(size_t));
    if (ret) {
        return FORTHVM_ERR_OUT_OF_MEMORY;
    }
    return FORTHVM_ERR_OK;
}

void ForthVM_finalize(ForthVM *vm) {
    DArrayChar_finalize(&vm->words);
    DArrayOffset_finalize(&vm->offset);
    DArrayChar_finalize(&vm->literal);
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
    vm->ip = 0;
    ForthParser parser;
    ForthVMErr err = ForthParser_initialize(&parser);
    if (err) {
        ForthParser_finalize(&parser);
        return err;
    }
    DArrayChar_clear(&vm->interpreted);
    err = ForthParser_parse(&parser, str, vm);
    ForthParser_finalize(&parser);
    if (err) {
        return err;
    }
    char chr = OPCODE_TERMINATE;
    int ret = DArrayChar_push_back(&vm->interpreted, &chr);
    if (ret) {
        return FORTHVM_ERR_OUT_OF_MEMORY;
    }
    return FORTHVM_ERR_OK;
}
