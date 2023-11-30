//Character encoding information

#ifndef __SYSCHARE_H__
#define __SYSCHARE_H__

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "bstream.h"

#define CHARE_RECORD_TYPE 7
#define CHARE_SUBTYPE 20
#define CHARE_SIZE_CHECK 1

typedef struct SysCharEncoding {
    int32_t record_type;
    int32_t subtype;
    bstream_t *encoding;
    bool constructed;
}sysce_t;

sysce_t *sysce_new(int32_t,int32_t,bstream_t*);
sysce_t *sysce_fnew(FILE*);
bool sysce_destroy(sysce_t*);

#endif