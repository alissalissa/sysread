//Display parameters for the variable records
#ifndef DISPPAR_H
#define DISPPAR_H

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "sfutil.h"

#define MEASURE_UNKNOWN (int32_t)0
#define MEASURE_NOMINAL (int32_t)1
#define MEASURE_ORDINAL (int32_t)2
#define MEASURE_SCALE (int32_t)3

#define ALIGN_LEFT (int32_t)0
#define ALIGN_RIGHT (int32_t)1
#define ALIGN_CENTER (int32_t)2

//TODO implement a unit test for this struct
typedef struct DisplayParameters {
	int32_t record_type;
	int32_t subtype;
	int32_t count;
	int32_t *measures;
	int32_t *widths;
	int32_t *alignments;
	bool constructed;
}disppar_t;

disppar_t *disppar_new(int32_t,int32_t,int32_t,int32_t*,int32_t*,int32_t*);
disppar_t *disppar_fnew(FILE*);
bool disppar_destroy(disppar_t*);

#endif