//Long string missing values record
#ifndef __LSMV_RECORD_H__
#define __LSMV_RECORD_H__

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "bstream.h"

#define LSMVR_RECORD_TYPE 7
#define LSMVR_SUBTYPE 22
#define LSMVR_MIN_N 1
#define LSMVR_MAX_N 3
#define LSMVR_VALUE_LEN 8

typedef struct MissingValue {
    bstream_t *var_name;
    int8_t n;
    bstream_t **missing_values;
    bool constructed;
}lsmv_t;

lsmv_t *lsmv_new(bstream_t*,int8_t,bstream_t**);
bool lsmv_destroy(lsmv_t*);

typedef struct LSMVRecord {
    int32_t record_type;
    int32_t subtype;
    int32_t count;
    lsmv_t **records;
    bool constructed;
}lsmvr_t;

lsmvr_t *lsmvr_new(int32_t,int32_t,int32_t,lsmv_t**);
lsmvr_t *lsmvr_fnew(FILE*);
bool lsmvr_destroy(lsmvr_t*);

#endif