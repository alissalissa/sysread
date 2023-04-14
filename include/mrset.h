//data type and associated functions for multiple record response sets
#ifndef MRSET_H
#define MRSET_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*typedef struct sysmrset {
	int32_t record_type;
	int32_t subtype;
	//Number of bytes per element within a set -- always 1
	int32_t size;
	//Number of bytes in the data portion
	int32_t count;
	//data
	struct sysmrset *response_sets;
	//Construction flag
	bool constructed;
}sysmrset_t;

//sysmrset_t factories, new is only for use by fnew
sysmrset_t *sysmrset_new(int32_t,int32_t,int32_t,int32_t,sysmrset_t*);
sysmrset_t *sysmrset_fnew(FILE*);
bool sysmrset_destroy(sysmrset_t*);

//an individual record
typedef struct sysmr{

}sysmr_t;*/

//Multiple category Set
typedef struct mcset{
	int32_t set_name_length;
	char *set_name;
	int32_t record_type;
	int32_t label_length;
	char *label;
	int32_t count;
	int32_t *var_lengths;
	char **variables;
	bool constructed;
}mcset_t;

//mcset factories, new is only for use by snew
mcset_t *mcset_new(int32_t,char*,int32_t,int32_t,char*,int32_t,int32_t*,char**);
mcset_t *mcset_snew(int32_t,char*); //snew reads from a byte stream rather than a file handle
bool mcset_destroy(mcset_t*);

#endif