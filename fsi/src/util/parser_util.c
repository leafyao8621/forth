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

// bool lookup_token(char *buf, uint8_t **meta, uintptr_t **addr) {
//     uint8_t *iter = vm_lookup;
//     for (; iter < vm_lookup_cur; ++iter) {
//         if (!strcmp(buf, (const char*)(iter + sizeof(uintptr_t) + 1))) {
//             *meta = iter;
//             *addr = (uintptr_t*)(iter + 1);
//             return true;
//         }
//         iter += sizeof(uintptr_t) + 1;
//         for (; *iter && iter < vm_lookup_cur; ++iter);
//     }
//     return false;
// }

// bool parser_int10(char *buf, uintptr_t *out) {
//     char *iter = buf;
//     bool neg = false;
//     *out = 0;
//     for (; *iter; ++iter) {
//         if (!neg && iter == buf && *iter == '-') {
//             neg = true;
//             continue;
//         }
//         if (*iter < '0' || *iter > '9') {
//             return false;
//         }
//         *out *= 10;
//         *out += *iter - '0';
//     }
//     if (neg) {
//         *out = -*out;
//     }
//     return true;
// }

// bool parser_int16(char *buf, uintptr_t *out) {
//     char *iter = buf;
//     *out = 0;
//     for (; *iter; ++iter) {
//         *out <<= 4;
//         if (*iter >= '0' && *iter <= '9') {
//             *out += *iter - '0';
//         } else if (*iter >= 'A' && *iter <= 'F') {
//             *out += *iter - 'A' + 0xA;
//         } else {
//             return false;
//         }
//     }
//     return true;
// }
