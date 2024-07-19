#include "parser.h"
#include "../util/status.h"

int parser_parse(bool line, FILE *fin) {
    int in = 0;
    for (in = fgetc(fin); in != -1 && !(line && in == '\n'); in = fgetc(fin)) {
        printf("%02x\n", in);
    }
    if (line && (in == -1)) {
        return PARSER_STATUS_END_REPL;
    }
    return PARSER_STATUS_OK;
}
