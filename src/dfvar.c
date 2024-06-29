//
// Created by alissa on 6/27/24.
//
#include "../include/dfvar.h"

dfvar_t *dfvar_new(int32_t record_type,int32_t subtype,int32_t count,char *attributes){
	dfvar_t *ret=(dfvar_t*)malloc(sizeof(dfvar_t));
	ret->constructed=false;
	
	ret->record_type=record_type;
	ret->subtype=subtype;
	
	ret->attributes=bstream_new_wl(count);
	memcpy(ret->attributes->stream,attributes,ret->attributes->length);
	if(!ret->attributes){
		free(ret);
		return NULL;
	}
	
	ret->constructed=true;
	return ret;
}

bool dfvar_destroy(dfvar_t *haystack){
	assert(haystack);
	if(!haystack->constructed)
		return false;
	
	haystack->constructed=false;
	if(haystack->attributes) {
		if(!bstream_destroy(haystack->attributes))
			return false;
	}
	
	free(haystack);
	return true;
}
