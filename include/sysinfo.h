//struct to contain information about the file itself
#ifndef SYSINFO_H
#define SYSINFO_H

#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct sysinfo {
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
}sysinfo_t;

//Factory functions.  New is for internal use by fnew only
sysinfo_t *sysinfo_new(int32_t,int32_t,int32_t,int32_t,int32_t*);
sysinfo_t *sysinfo_fnew(FILE*);
bool sysinfo_destroy(sysinfo_t*);

#endif