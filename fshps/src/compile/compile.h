#ifndef COMPILE_COMPILE_H_
#define COMPILE_COMPILE_H_

#define HEADER_KEY 0
#define HEADER_URL 1
#define HEADER_GET 2
#define HEADER_HEAD 3
#define HEADER_POST 4
#define HEADER_PUT 5
#define HEADER_DELETE 6
#define HEADER_CONNECT 7
#define HEADER_OPTIONS 8
#define HEADER_TRACE 9
#define HEADER_PATCH 10

typedef enum ErrCompile {
    ERR_COMPILE_OK,
    ERR_COMPILE_OPEN,
    ERR_COMPILE_OUT_OF_MEMORY,
    ERR_COMPILE_INVALID_HEADER
} ErrCompile;

ErrCompile fshps_compile(void);

#endif
