#ifndef FSHPS_FSHPS_H_
#define FSHPS_FSHPS_H_

#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <fcntl.h>

#include <http_server/errcode.h>
#include <http_server/server.h>
#include <containers/eq.h>
#include <containers/hash.h>
#include <fshp/fshp.h>

extern size_t memory;
extern size_t lookup;
extern size_t literal;
extern size_t ext;
extern size_t mod;
extern size_t mod_so;
extern size_t interpreted;
extern size_t data_stack;
extern size_t float_stack;
extern size_t return_stack;
extern size_t compiled;

#endif
