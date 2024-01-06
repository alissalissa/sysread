//Long string value labels record
#ifndef __SYSREAD_LSVALUE_H__
#define __SYSREAD_LSVALUE_H__

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bstream.h"

#define LSVLABEL_RECORD_TYPE 7
#define LSVLABEL_SUBTYPE 21

#define LSVLABEL_MIN_WIDTH 9
#define LSVLABEL_MAX_WIDTH 32767

typedef struct LSVLabel {
	bstream_t *value;
	bstream_t *label;
	bool constructed;
}lsvlabel_t;

lsvlabel_t *lsvlabel_new(bstream_t*,bstream_t*);
lsvlabel_t *lsvlabel_cnew(lsvlabel_t*); //copy factory
bool lsvlabel_destroy(lsvlabel_t*);

typedef struct LSVariable {
	bstream_t *var_name;
	int32_t size;
	lsvlabel_t **labels;
	bool constructed;
}lsvar_t;

lsvar_t *lsvar_new(bstream_t*,int32_t,lsvlabel_t**);
bool lsvar_destroy(lsvar_t*);

typedef struct LSVLabelList {
	//Header
	int32_t record_type;
	int32_t subtype;

	//Values
	int32_t n_labels;
	lsvlabel_t **labels;
	
	bool constructed;
}lsvlabel_list_t;

lsvlabel_list_t *lsvlabel_list_new(int32_t,int32_t,int32_t,bstream_t*,lsvlabel_t**);
lsvlabel_list_t *lsvlabel_list_fnew(FILE*);
bool lsvlabel_list_destroy(lsvlabel_list_t*);

#endif //__SYSREAD_LSVALUE_H__