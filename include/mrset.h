//data type and associated functions for multiple record response sets
#ifndef MRSET_H
#define MRSET_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bstream.h"
#include "sfutil.h"

const char MDSET_VARLABELS='D';
const char MDSET_COUNTEDVALUES='E';

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
	bstream_t *set_name;
	int32_t record_type;
	bstream_t *label;
	int32_t count;
	bstream_t **variables;
	bool constructed;
}mcset_t;

//mcset factories, new is only for use by snew
mcset_t *mcset_new(bstream_t*,int32_t,bstream_t*,int32_t,bstream_t**);
mcset_t *mcset_snew(bstream_t*); //snew reads from a byte stream rather than a file handle
bool mcset_destroy(mcset_t*);

/*****************************************************************/

//Multiple Dichotomy Sets
typedef struct mdset {
	bstream_t *set_name;
	int32_t record_type;
	char flag;//Either MDSET_VARLABELS or MDSET_COUNTEDVALUES
	bstream_t *label;//label for the set
	int32_t count;//The number of variables
	bstream_t **variables;//the names of the variables
	bstream_t *counted_value;
	bool constructed;
}mdset_t;

//mdset factories, new is only for use by snew
mdset_t *mdset_new(bstream_t*,int32_t,char,bstream_t*,int32_t,bstream_t**,bstream_t*);
mdset_t *mdset_snew(bstream_t*);
bool mdset_destroy(mdset_t*);

#endif