#ifndef UTIL_STATUS_H_
#define UTIL_STATUS_H_

#define PARSER_STATUS_OK 0
#define PARSER_STATUS_END_REPL 1
#define PARSER_TOKEN_TOO_LONG 2

extern const char *parser_status_lookup[3];

#endif
