//Defines a type to hold variable and case data and read same from a FILE

#ifndef SYSVAR_H
#define SYSVAR_H

#define VAR_NAME_SIZE 8

#include <stdbool.h>
#include <stdlib.h>
#include "stdint.h"
#include "stdio.h"

typedef struct sysvar{
	int32_t rec_type;
	int32_t type;
	int32_t has_var_label;
	int32_t n_missing_values;
	int32_t print;
	int32_t write;
	char *name;
	//Optionals
	int32_t label_length;
	char *label;
	float *missing_values;
	bool constructed;
}sysvar_t;

//sysvar_new() is only intended for internal use, to be called by fnew
sysvar_t *sysvar_new(int32_t,int32_t,int32_t,int32_t,int32_t,int32_t,char*,float*);
sysvar_t *sysvar_fnew(FILE*);
bool sysvar_destroy(sysvar_t*);

#endif