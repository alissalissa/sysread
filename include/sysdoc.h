//type to hold the document record(s)

#ifndef SYSDOC_H
#define SYSDOC_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DOC_LINE_LENGTH 80

typedef struct sysdoc{
	int32_t record_type;
	int32_t n_lines;
	char **lines;
	bool constructed;
}sysdoc_t;

//This factory method is for internal calling only
sysdoc_t *sysdoc_new(int32_t,int32_t,char**);
sysdoc_t *sysdoc_fnew(FILE*);
bool sysdoc_destroy(sysdoc_t*);

#endif