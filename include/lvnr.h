//Long variable names record

#ifndef __SYSREAD_LVNR_H__
#define __SYSREAD_LVNR_H__

#define LVNR_SIZE_CHECK 1

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bstream.h"

typedef struct {
	int32_t rec_type; //Should be always 7
	int32_t subtype; //Should be always 13
	int32_t count; //Keys and values should always be of length count
	bstream_t *keys; //At most 8 bits wide
	bstream_t *values; //At most 64 bits wide
	bool constructed;
}lvnr_t;

lvnr_t *lvnr_new(int32_t,int32_t,int32_t,bstream_t*,bstream_t*);
lvnr_t *lvnr_fnew(FILE*);
bool lvnr_destroy(lvnr_t*);

#endif