#ifndef SYSVL_H
#define SYSVL_H

#define VL_VALUE_LENGTH 8

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//an individual label
typedef struct sysvarlabel {
	char *value;
	char label_length;
	char *label;
}sysvarlabel_t;

//This function is for internal use by the fnew function only
sysvarlabel_t *sysvarlabel_new(char*,char,char*);
sysvarlabel_t *sysvarlabel_fnew(FILE*);
bool sysvarlabel_destroy(sysvarlabel_t*);

//Collection of all the labels
typedef struct syslabels{
	char record_type;
	int32_t label_count;
	sysvarlabel_t *labels;
}syslabels_t;

//This function is for internal use by the fnew function only
syslabels_t *syslabels_new(char,int32_t,sysvarlabel_t*);
syslabels_t *syslabels_fnew(FILE*);
bool syslabels_destroy(syslabels_t*);

#endif
