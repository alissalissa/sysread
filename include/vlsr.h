//Very Long String Record
#ifndef __SYS_VLSR_H__
#define __SYS_VLSR_H__

#define VLSR_RECORD_TYPE 7
#define VLSR_SUBTYPE 14

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "bstream.h"

typedef struct SYSVLSR {
	int32_t record_type;
	int32_t subtype;
	int32_t n;
	bstream_t *keys;
	bstream_t *values;
	bool constructed;
}vlsr_t;

vlsr_t *vlsr_new(int32_t,int32_t,int32_t,bstream_t*,bstream_t*);
vlsr_t *vlsr_fnew(FILE*);
bool vlsr_destroy(vlsr_t*);

#endif