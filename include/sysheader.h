//Defines a type to contain header data and read said data from a FILE

#ifndef SYS_HEADER_H
#define SYS_HEADER_H

#define REC_TYPE_SIZE 4
#define PROD_NAME_SIZE 60
#define PROD_NAME_CHECK_SIZE 19
#define CREATION_DATE_SIZE 9
#define CREATION_TIME_SIZE 8
#define FILE_LABEL_SIZE 64
#define PADDING_SIZE 3

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "bstream.h"
#include "sfutil.h"

typedef struct sysheader {
	bstream_t *rec_type;
	bstream_t *prod_name;
	int32_t layout_code;
	int32_t nominal_case_size;
	int32_t	compression;
	int32_t weight_index;
	int32_t ncases;
	float bias;
	bstream_t *creation_date;
	bstream_t *creation_time;
	bstream_t *file_label;
	bstream_t *padding;
	bool constructed;
}sysheader_t;

//Construction/destruction
sysheader_t *sysheader_new(bstream_t*,bstream_t*,int32_t,int32_t,int32_t,int32_t,int32_t,float,bstream_t*,bstream_t*,bstream_t*,bstream_t*); //only intended for internal use with sysheader_fnew
//Generates a sysheader_t by reading from a file handle
sysheader_t *sysheader_fnew(FILE*);
bool sysheader_destroy(sysheader_t*);

#endif