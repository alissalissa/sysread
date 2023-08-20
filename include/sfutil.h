//utility functions used by other parts of the sysread library
#ifndef SF_UTIL_H
#define SF_UTIL_H

#include <stdarg.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "bstream.h"

void mass_free(int,...);

//For use with header loading from file.
//	verifies if a date/time string is formatted correctly
bool verify_date_format(char*);
bool verify_time_format(char*);

//manipulates a non-null-terminated byte string
char *subset(char*,int,int);
//first element of return value is guaranteed to be present
//  it is the number of elements.
//  Remaining elements are the lengths of the names,
//      Which results in a size of number of labels +1
//int *split(char**,char*,int,char);
//Finds the first occurrence
//int find_first(char*,int,char);

//Is this value one of the following?
bool one_of(int,int,...);
bool bstream_one_of(bstream_t,int,...);

#endif