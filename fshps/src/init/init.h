#ifndef INIT_INIT_H_
#define INIT_INIT_H_

typedef enum ErrInit {
    ERR_INIT_OK,
    ERR_INIT_MKDIR,
    ERR_INIT_CREATE,
    ERR_INIT_WRITE
} ErrInit;

ErrInit fshps_init(void);

#endif
