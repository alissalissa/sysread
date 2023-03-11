#ifndef SYS_HEADER_H
#define SYS_HEADER_H

#define REC_TYPE_SIZE 4
#define PROD_NAME_SIZE 60
#define CREATION_DATE_SIZE 9
#define CREATION_TIME_SIZE 8
#define FILE_LABEL_SIZE 64
#define PADDING_SIZE 3

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

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
sysheader_t sysheader_new(char*,char*,int32_t,int32_t,int32_t,int32_t,int32_t,float,char*,char*,char*,char*); //only intended for internal use
bool sysheader_fnew(sysheader_t*,FILE*);
bool sysheader_destroy(sysheader_t*);

#endif