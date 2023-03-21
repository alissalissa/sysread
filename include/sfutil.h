//utility functions used by other parts of the sysread library
#ifndef SF_UTIL_H
#define SF_UTIL_H

#include <stdarg.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

void mass_free(int,...);

//For use with header loading from file.
//	verifies if a date/time string is formatted correctly
bool verify_date_format(char*);
bool verify_time_format(char*);

#endif