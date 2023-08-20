//Contains variable set data

#ifndef SYSVSET_H
#define SYSVSET_H

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "bstream.h"

#define SVSET_TYPE 7
#define SVSET_SUBTYPE 5
#define SYSVSET_INT_SIZE 1

typedef struct __individualset {
    bstream_t label;
    int32_t count;
    bstream_t *var_names;
    bool constructed;
}svset_t;

svset_t *svset_new(bstream_t*);
void svset_copy(svset_t*,svset_t*);
bool svset_destroy(svset_t*);

typedef struct SysVSetList {
    int32_t rec_type;
    int32_t subtype;
    int32_t count;
    svset_t **sets;
    bool constructed;
}svsetlist_t;

svsetlist_t *svsetlist_new(int32_t,int32_t,int32_t,svset_t**);
svsetlist_t *svsetlist_fnew(FILE*);
bool svsetlist_destroy(svsetlist_t*);

#endif