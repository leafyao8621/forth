#include <stdio.h>

#include <fsi/interpreter/vm.h>

void ForthVM_log(ForthVM *vm) {
    puts("Words:");
    char *iter_words = vm->words.data;
    size_t *iter_offset = vm->offset.data;
    for (size_t i = 0; i < vm->offset.size; ++i, ++iter_words, ++iter_offset) {
        printf("0x%016lx:\n", i);
        puts(iter_words);
        for (; *iter_words; ++iter_words);
        printf("0x%016lx", *iter_offset & OFFSET_OFFSET);
        if (*iter_offset & OFFSET_BUILTIN) {
            printf(" %s", "BUILTIN");
        }
        if (*iter_offset & OFFSET_MEMORY) {
            printf(" %s", "MEMORY");
        }
        putchar(10);
    }
    puts("Literal:");
    char *iter_literal = vm->literal.data;
    char *iter_iter_literal = 0;
    for (size_t i = 0; i < vm->literal.size; ++i, ++iter_literal) {
        printf("0x%016lx:\n", i);
        iter_iter_literal = iter_literal;
        for (; *iter_iter_literal; ++iter_iter_literal) {
            printf("%02hhx", *iter_iter_literal);
        }
        putchar(10);
        iter_iter_literal = iter_literal;
        for (; *iter_iter_literal; ++iter_iter_literal) {
            if (*iter_iter_literal >= 31 && *iter_iter_literal < 127) {
                putchar(*iter_iter_literal);
            } else {
                printf("\\x%02hhx", *iter_iter_literal);
            }
        }
        putchar(10);
        for (; *iter_literal; ++i, ++iter_literal);
    }
    puts("Compiled:");
    char *iter_compiled = vm->compiled.data;
    for (size_t i = 0; i < vm->compiled.size; ++i, ++iter_compiled) {
        printf("0x%016lx: ", i);
        switch (*iter_compiled) {
        case OPCODE_TERMINATE:
            puts("TERMINATE");
            break;
        case OPCODE_PRINT_STRING:
            printf(".\" 0x%016lx\n", *(size_t*)(iter_compiled + 1));
            iter_compiled += sizeof(size_t);
            i += sizeof(size_t);
            break;
        case OPCODE_CARRIAGE_RETURN:
            puts("cr");
            break;
        case OPCODE_PUSH:
            printf("PUSH 0x%016lx\n", *(size_t*)(iter_compiled + 1));
            iter_compiled += sizeof(size_t);
            i += sizeof(size_t);
            break;
        case OPCODE_PRINT_INT:
            puts(".");
            break;
        case OPCODE_RETURN:
            puts("RETURN");
            break;
        case OPCODE_CALL:
            printf("CALL 0x%016lx\n", *(size_t*)(iter_compiled + 1));
            iter_compiled += sizeof(size_t);
            i += sizeof(size_t);
            break;
        case OPCODE_ADD:
            puts("+");
            break;
        case OPCODE_SUBTRACT:
            puts("-");
            break;
        case OPCODE_MULTIPLY:
            puts("*");
            break;
        case OPCODE_DIVIDE:
            puts("/");
            break;
        case OPCODE_INCREMENT:
            puts("1+");
            break;
        case OPCODE_DECREMENT:
            puts("1-");
            break;
        default:
            puts("UNKNOWN");
        }
    }
    puts("Interpreted:");
    char *iter_interpreted = vm->interpreted.data;
    for (size_t i = 0; i < vm->interpreted.size; ++i, ++iter_interpreted) {
        printf("0x%016lx: ", i);
        switch (*iter_interpreted) {
        case OPCODE_TERMINATE:
            puts("TERMINATE");
            break;
        case OPCODE_PRINT_STRING:
            printf(".\" 0x%016lx\n", *(size_t*)(iter_interpreted + 1));
            iter_interpreted += sizeof(size_t);
            i += sizeof(size_t);
            break;
        case OPCODE_CARRIAGE_RETURN:
            puts("cr");
            break;
        case OPCODE_PUSH:
            printf("PUSH 0x%016lx\n", *(size_t*)(iter_interpreted + 1));
            iter_interpreted += sizeof(size_t);
            i += sizeof(size_t);
            break;
        case OPCODE_PRINT_INT:
            puts(".");
            break;
        case OPCODE_CALL:
            printf("CALL 0x%016lx\n", *(size_t*)(iter_interpreted + 1));
            iter_interpreted += sizeof(size_t);
            i += sizeof(size_t);
            break;
        case OPCODE_ADD:
            puts("+");
            break;
        case OPCODE_SUBTRACT:
            puts("-");
            break;
        case OPCODE_MULTIPLY:
            puts("*");
            break;
        case OPCODE_DIVIDE:
            puts("/");
            break;
        case OPCODE_INCREMENT:
            puts("1+");
            break;
        case OPCODE_DECREMENT:
            puts("1-");
            break;
        default:
            puts("UNKNOWN");
        }
    }
}
