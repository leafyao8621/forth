#include <string.h>

#include <fsi/parser/parser.h>
#include <fsi/vm/vm.h>

#include <fsi/util/parser_util.h>

bool read_file(FILE *fin, String *buf) {
    DArrayChar_clear(buf);
    char local[1000];
    for (
        size_t read = fread(local, 1, 1000, fin);
        read;
        read = fread(local, 1, 1000, fin)) {
        int ret = DArrayChar_push_back_batch(buf, local, read);
        if (ret) {
            return false;
        }
    }
    char chr = 0;
    int ret = DArrayChar_push_back(buf, &chr);
    if (ret) {
        return false;
    }
    return true;
}

int get_input(String *buf) {
    DArrayChar_clear(buf);
    char local[1000];
    char *read = 0;
    for (
        read = fgets(local, 1000, stdin);
        read;
        read = fgets(local, 1000, stdin)) {
        size_t len = strlen(local);
        int ret = DArrayChar_push_back_batch(buf, local, len);
        if (ret) {
            return 2;
        }
        if (local[len - 1] == '\n') {
            break;
        }
    }
    if (!read) {
        return 1;
    }
    char chr = 0;
    int ret = DArrayChar_push_back(buf, &chr);
    if (ret) {
        return 2;
    }
    return 0;
}

bool parse_token(ForthParser *parser, bool line, char **str) {
    DArrayChar_clear(&parser->buf);
    for (
        ;
        **str &&
        **str != '\n' &&
        **str != '\t' &&
        **str != ' ';
        ++(*str)) {
        if (DArrayChar_push_back(&parser->buf, *str)) {
            return false;
        }
    }
    char chr = 0;
    if (DArrayChar_push_back(&parser->buf, &chr)) {
        return false;
    }
    if (line) {
        switch (**str) {
        case 0:
        case '\n':
            parser->status = PARSER_STATUS_END;
            break;
        }
    } else {
        if (!**str) {
            parser->status = PARSER_STATUS_END;
        }
    }
    return true;
}

void next_token(ForthParser *parser, bool line, char **str) {
    for (
        ;
        **str == '\t' ||
        **str == ' ' ||
        **str == '\n';
        ++(*str));
    if (line) {
        switch (**str) {
        case 0:
        case '\n':
            parser->status = PARSER_STATUS_END;
            return;
        }
    } else {
        if (!**str) {
            parser->status = PARSER_STATUS_END;
            return;
        }
    }
}

bool lookup_token(ForthVM *vm, char *buf, uint8_t **meta, uintptr_t **addr) {
    uint8_t *iter = vm->lookup;
    for (; iter < vm->lookup_cur; ++iter) {
        if (
            (*iter & VM_LOOKUP_META_INDIRECT) ||
            (*iter & VM_LOOKUP_META_PARSEEXT)) {
            if (
                !strcmp(
                    buf,
                    (const char*)
                        (iter + sizeof(uintptr_t) + 1 + sizeof(uintptr_t)))) {
                *meta = iter;
                *addr = (uintptr_t*)(iter + 1);
                return true;
            }
            iter += sizeof(uintptr_t) + 1 + sizeof(uintptr_t);
        } else {
            if (!strcmp(buf, (const char*)(iter + sizeof(uintptr_t) + 1))) {
                *meta = iter;
                *addr = (uintptr_t*)(iter + 1);
                return true;
            }
            iter += sizeof(uintptr_t) + 1;
        }
        for (; *iter && iter < vm->lookup_cur; ++iter);
    }
    return false;
}

static bool parser_double(char *buf, double *out) {
    char *iter = buf;
    bool decimal = false;
    bool e = false;
    for (; *iter; ++iter) {
        if (*iter >= '0' && *iter <= '9') {
            continue;
        }
        switch (*iter) {
        case '.':
            if (decimal) {
                return false;
            }
            decimal = true;
            break;
        case 'E':
            if (e) {
                return false;
            }
            e = true;
            break;
        case '+':
        case '-':
            if ((iter == buf) || (iter[-1] == 'E')) {
                continue;
            }
            return false;
        default:
            return false;
        }
    }
    sscanf(buf, "%lf", out);
    return true;
}

bool parser_int10(char *buf, uintptr_t *out) {
    char *iter = buf;
    bool neg = false;
    *out = 0;
    for (; *iter; ++iter) {
        if (!neg && iter == buf && *iter == '-') {
            neg = true;
            continue;
        }
        if (*iter < '0' || *iter > '9') {
            switch (*iter) {
            case '.':
            case 'E':
                return parser_double(buf, (double*)out);
            default:
                return false;
            }
        }
        *out *= 10;
        *out += *iter - '0';
    }
    if (neg) {
        *out = -*out;
    }
    return true;
}

bool parser_int16(char *buf, uintptr_t *out) {
    char *iter = buf;
    *out = 0;
    for (; *iter; ++iter) {
        *out <<= 4;
        if (*iter >= '0' && *iter <= '9') {
            *out += *iter - '0';
        } else if (*iter >= 'A' && *iter <= 'F') {
            *out += *iter - 'A' + 0xA;
        } else {
            return false;
        }
    }
    return true;
}

void next_function(uint8_t **addr) {
    for (; **addr; ++(*addr)) {
        switch (**addr) {
        case VM_INSTRUCTION_PUSHD:
        case VM_INSTRUCTION_PUSHID:
        case VM_INSTRUCTION_CALL:
        case VM_INSTRUCTION_JZD:
        case VM_INSTRUCTION_JNZD:
        case VM_INSTRUCTION_JNED:
        case VM_INSTRUCTION_JMP:
        case VM_INSTRUCTION_JNER:
            *addr += sizeof(uintptr_t);
        }
    }
    ++(*addr);
}
