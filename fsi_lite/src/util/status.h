#ifndef UTIL_STATUS_H_
#define UTIL_STATUS_H_

#define PARSER_STATUS_OK 0
#define PARSER_STATUS_END_REPL 1
#define PARSER_STATUS_TOKEN_TOO_LONG 2
#define PARSER_STATUS_INTERPRETED_OVERFLOW 3
#define PARSER_STATUS_COMPILED_OVERFLOW 4

extern const char *parser_status_lookup[5];

#endif
