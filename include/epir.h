//Handles extra product info for a sysfile
#ifndef EPIR_H
#define EPIR_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "bstream.h"

#define EPIR_TYPE 7
#define EPIR_SUBTYPE 10
#define EPIR_SIZE 1

typedef struct SysEPIR {
	int32_t record_type;
	int32_t subtype;
	bstream_t *info;
	bool constructed;
}sysepir_t;

sysepir_t *sysepir_new(int32_t,int32_t,bstream_t*);
sysepir_t *sysepir_fnew(FILE*);
bool sysepir_destroy(sysepir_t*);

#endif