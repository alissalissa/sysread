//struct to contain information about the file itself
#ifndef SYSIINFO_H
#define SYSIINFO_H

#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct sysintinfo {
	//Header data
	int32_t rec_type;
	int32_t subtype;
	int32_t size;
	int32_t count;
	//info data
	int32_t version_major;
	int32_t version_minor;
	int32_t version_revision;
	int32_t machine_code;
	int32_t floating_point_rep;
	int32_t compression_code;
	int32_t endianness;
	int32_t character_code;
	//Construction flag
	bool constructed;
}sysintinfo_t;

//Factory functions.  New is for internal use by fnew only
sysintinfo_t *sysinfo_new(int32_t,int32_t,int32_t,int32_t,int32_t*);
sysintinfo_t *sysinfo_fnew(FILE*);
bool sysinfo_destroy(sysintinfo_t*);

#endif