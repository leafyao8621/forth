#ifndef COMPILE_COMPILE_H_
#define COMPILE_COMPILE_H_

typedef enum ErrCompile {
    ERR_COMPILE_OK,
    ERR_COMPILE_MKDIR,
    ERR_COMPILE_CREATE
} ErrCompile;

ErrCompile compile(void);

#endif
