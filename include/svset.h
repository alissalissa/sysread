//Contains variable set data

#ifndef SYSVSET_H
#define SYSVSET_H

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "bstream.h"

#define SYSVSET_INT_SIZE 1

typedef struct __individualset {
    bstream_t label;
    int32_t count;
    bstream_t *var_names;
    bool constructed;
}svset_t;

svset_t *svset_new(bstream_t*);
bool svset_destroy(svset_t*);

typedef struct SysVSetList {
    int32_t rec_type;
    int32_t subtype;
    svset_t *sets;
}svsetlist_t;

#endif