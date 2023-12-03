//Long string values label record
#ifndef __SYS_LSVL_H__
#define __SYS_LSVL_H__

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bstream.h"

//An individual label
typedef struct LongStringLabel {
	bstream_t *value;
	bstream_t *label;
	bool constructed;
}syslsl_t;

syslsl_t *syslsl_new(bstream_t*,bstream_t*);
bool syslsl_destroy(syslsl_t*);

//The container for a set of labels
typedef struct LongStringValueRecord {
	int32_t record_type;
	int32_t subtype;
	int32_t n;
	bstream_t **var_name;
	int32_t *var_width;
	syslsl_t **labels;
	bool constructed;
}lsvr_t;

lsvr_t *lslr_new(int32_t,int32_t,int32_t,bstream_t**,int32_t*,syslsl_t**);
lsvr_t *lslr_fnew(FILE*);
bool lslr_destroy(lsvr_t*);

#endif //__SYS_LSVL_H__