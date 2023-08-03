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

#include "sfutil.h"

//TODO refactor to use bstreams
typedef struct sysheader {
	char *rec_type;
	char *prod_name;
	int32_t layout_code;
	int32_t nominal_case_size;
	int32_t	compression;
	int32_t weight_index;
	int32_t ncases;
	float bias;
	char *creation_date;
	char *creation_time;
	char *file_label;
	char *padding;
	bool constructed;
}sysheader_t;

//Construction/destruction
sysheader_t *sysheader_new(char*,char*,int32_t,int32_t,int32_t,int32_t,int32_t,float,char*,char*,char*,char*); //only intended for internal use with sysheader_fnew
//Generates a sysheader_t by reading from a file handle
sysheader_t *sysheader_fnew(FILE*);
bool sysheader_destroy(sysheader_t*);

#endif