#ifndef COMPILE_COMPILE_H_
#define COMPILE_COMPILE_H_

#define HEADER_URL 0
#define HEADER_GET 1
#define HEADER_HEAD 2
#define HEADER_POST 3
#define HEADER_PUT 4
#define HEADER_DELETE 5
#define HEADER_CONNECT 6
#define HEADER_OPTIONS 7
#define HEADER_TRACE 8
#define HEADER_PATCH 9

typedef enum ErrCompile {
    ERR_COMPILE_OK,
    ERR_COMPILE_OPEN,
    ERR_COMPILE_OUT_OF_MEMORY,
    ERR_COMPILE_INVALID_HEADER
} ErrCompile;

ErrCompile compile(void);

#endif
