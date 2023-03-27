//Reads and defines the float information for a system file

#ifndef SYSFINFO_H
#define SYSFINFO_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct sysfltinfo {
	//header
	int32_t rec_type;
	int32_t subtype;
	int32_t size;
	int32_t count;
	//data
	double missing;
	double highest;
	double lowest;
	//Construction flag
	bool constructed;
}sysfltinfo_t;

//Factory functions.  new is only for internal use by fnew.  Follows the intinfo schema (sysiinfo.h)
sysfltinfo_t *sysfltinfo_new(int32_t,int32_t,int32_t,int32_t,double*);
sysfltinfo_t *sysfltinfo_fnew(FILE*);
bool sysfltinfo_destroy(sysfltinfo_t*);

#endif