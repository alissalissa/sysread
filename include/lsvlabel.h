//Long string value labels record
#ifndef __SYSREAD_LSVALUE_H__
#define __SYSREAD_LSVALUE_H__

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bstream.h"

typedef struct LSVLabel {
	bstream_t *value;
	bstream_t *label;
	bool constructed;
}lsvlabel_t;

lsvlabel_t *lsvlabel_new(bstream_t*,bstream_t*);
bool lsvlabel_destroy(lsvlabel_t*);

typedef struct LSVLabelList {
	//Header
	int32_t record_type;
	int32_t subtype;

	//Values
	int32_t n_labels;
	lsvlabel_t **labels;
	
	bool constructed;
}lsvlabel_list_t;

lsvlabel_list_t *lsvlabel_list_new(int32_t,int32_t,int32_t,lsvlabel_t**);
lsvlabel_list_t *lsvlabel_list_fnew(FILE*);
bool lsvlabel_list_destroy(lsvlabel_list_t*);

#endif //__SYSREAD_LSVALUE_H__