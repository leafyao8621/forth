#include <string.h>

#include "../parser/parser.h"
#include "../vm/vm.h"

#include "parser_util.h"

bool parse_token(char *buf, char *buf_end, bool line, FILE *fin) {
    int in = 0;
    for (
        in = fgetc(fin);
        in != -1 &&
        in != '\n' &&
        in != '\t' &&
        in != ' ' &&
        buf != buf_end;
        in = fgetc(fin),
        ++buf) {
        *buf = in;
    }
    if (buf == buf_end) {
        return false;
    }
    *buf = 0;
    if (line) {
        switch (in) {
        case -1:
            parser_status = PARSER_STATUS_REPL_END;
            break;
        case '\n':
            parser_status = PARSER_STATUS_END;
            break;
        }
    } else {
        if (in == -1) {
            parser_status = PARSER_STATUS_END;
        }
    }
    return true;
}

void next_token(bool line, FILE *fin) {
    int in = 0;
    for (
        in = fgetc(fin);
        in == '\t' ||
        in == ' ';
        in = fgetc(fin));
    if (line) {
        switch (in) {
        case -1:
            parser_status = PARSER_STATUS_REPL_END;
            return;
        case '\n':
            parser_status = PARSER_STATUS_END;
            return;
        }
    } else {
        if (in == -1) {
            parser_status = PARSER_STATUS_END;
            return;
        }
    }
    ungetc(in, fin);
}

bool lookup_token(char *buf, uint8_t *meta, uintptr_t *addr) {
    uint8_t *iter = vm_lookup;
    for (; iter < vm_lookup_cur; ++iter) {
        if (!strcmp(buf, (const char*)(iter + sizeof(uintptr_t) + 1))) {
            *meta = *iter;
            *addr = *(uintptr_t*)(iter + 1);
            return true;
        }
        iter += sizeof(uintptr_t) + 1;
        for (; *iter && iter < vm_lookup_cur; ++iter);
    }
    return false;
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
            return false;
        }
        *out *= 10;
        *out += *iter - '0';
    }
    if (neg) {
        *out = -*out;
    }
    return true;
}
