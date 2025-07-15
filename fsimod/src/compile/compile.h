#ifndef COMPILE_COMPILE_H_
#define COMPILE_COMPILE_H_

#define COMPILE_MODE_PARSE 0x80
#define COMPILE_MODE_RUN 0x40

typedef enum ErrCompile {
    ERR_COMPILE_OK,
    ERR_COMPILE_OPEN,
    ERR_COMPILE_OUT_OF_MEMORY,
    ERR_COMPILE_INVALID_MODE
} ErrCompile;

typedef enum CompileState {
    COMPILE_STATE_MODE,
    COMPILE_STATE_NAME,
    COMPILE_STATE_PARSE,
    COMPILE_STATE_RUN,
} CompileState;

ErrCompile compile(void);

#endif
