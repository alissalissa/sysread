//Display parameters for the variable records
#ifndef DISPPAR_H
#define DISPPAR_H

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "sfutil.h"

const int32_t MEASURE_UNKNOWN=0;
const int32_t MEASURE_NOMINAL=1;
const int32_t MEASURE_ORDINAL=2;
const int32_t MEASURE_SCALE=3;

const int32_t ALIGN_LEFT=0;
const int32_t ALIGN_RIGHT=1;
const int32_t ALIGN_CENTER=2;

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