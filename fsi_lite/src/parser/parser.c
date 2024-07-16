#include "parser.h"

int parser_parse(FILE *fin) {
    int in = 0;
    for (in = fgetc(fin); in != -1; in = fgetc(fin)) {
        printf("%02x\n", in);
    }
    return 0;
}
