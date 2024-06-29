//
// Data file and variable attributes record
//

#ifndef SYSREAD_DFVAR_H
#define SYSREAD_DFVAR_H

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bstream.h"

typedef struct DFVAR {
	int32_t record_type;
	int32_t subtype;
	bstream_t *attributes;
	bool constructed;
}dfvar_t;

dfvar_t *dfvar_new(int32_t,int32_t,int32_t,char*);
dfvar_t *dfvar_fnew(FILE*);
bool dfvar_destroy(dfvar_t*);

#endif //SYSREAD_DFVAR_H
