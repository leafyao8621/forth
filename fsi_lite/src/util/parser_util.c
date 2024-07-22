#include "parser_util.h"

int parse_token(char *buf, char *buf_end, bool line, FILE *fin) {
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
        return 1;
    }
    *buf = 0;
    if (line) {
        switch (in) {
        case -1:
            return 2;
        case '\n':
            return 3;
        }
    } else {
        if (in == -1) {
            return 3;
        }
    }
    return 0;
}

int next_token(bool line, FILE *fin) {
    int in = 0;
    for (
        in = fgetc(fin);
        in == '\t' ||
        in == ' ';
        in = fgetc(fin));
    if (line) {
        switch (in) {
        case -1:
            return 1;
        case '\n':
            return 2;
        }
    } else {
        if (in == -1) {
            return 2;
        }
    }
    ungetc(in, fin);
    return 0;
}
