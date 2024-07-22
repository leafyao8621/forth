#include "parser.h"
#include "../util/status.h"
#include "../util/parser_util.h"

static char buf[100];
static char *buf_end;

void parser_initialize(void) {
    buf_end = buf + 100;
}

int parser_parse(bool line, FILE *fin) {
    int stat = 0;
    int ret = 0;
    for (stat = next_token(line, fin); !stat; stat = next_token(line, fin)) {
        ret = parse_token(buf, buf_end, line, fin);
        if (ret == 1) {
            return PARSER_TOKEN_TOO_LONG;
        }
        puts((char*)buf);
        if (ret == 2) {
            stat = 1;
            break;
        }
        if (ret == 3) {
            stat = 2;
            break;
        }
    }
    if (stat == 1) {
        return PARSER_STATUS_END_REPL;
    }
    return PARSER_STATUS_OK;
}
