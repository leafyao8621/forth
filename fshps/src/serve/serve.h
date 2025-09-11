#ifndef SERVE_SERVE_H_
#define SERVE_SERVE_H_

#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <fcntl.h>

#include <http_server/errcode.h>
#include <http_server/server.h>
#include <containers/eq.h>
#include <containers/hash.h>
#include <fshp/fshp.h>

void fshps_serve(
    int port,
    size_t memory,
    size_t lookup,
    size_t literal,
    size_t ext,
    size_t mod,
    size_t mod_so,
    size_t interpreted,
    size_t data_stack,
    size_t float_stack,
    size_t return_stack,
    size_t compiled);

#endif
