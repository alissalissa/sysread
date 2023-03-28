//data type and associated functions for multiple record response sets
#ifndef MRSET_H
#define MRSET_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct sysmrset {
	int32_t record_type;
	int32_t subtype;
	//Number of bytes per element within a set
	int32_t size;
	/*the specification says this is the number of bytes in the total record.
		For the purposes of this struct,
		count refers to the number of lines, rather than total bytes.*/
	int32_t count;
	//number of bytes in each line
	int32_t *length;
	//data
	char **mrsets;
	//Construction flag
	bool constructed;
}sysmrset_t;

//sysmrset_t factories, new is only for use by fnew
sysmrset_t *sysmrset_new(int32_t,int32_t,int32_t,int32_t,int32_t*,char**);
sysmrset_t *sysmrset_fnew(FILE*);
bool sysmrset_destroy(sysmrset_t*);

#endif