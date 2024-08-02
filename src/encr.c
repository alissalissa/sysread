//
// Created by alissa on 8/2/24.
//

#include "../include/encr.h"

encr_t *encr_new(int32_t record_type,int32_t subtype,int64_t ncases){
	if(record_type!=7 || subtype!=16 || ncases<-1)
		return NULL;
	encr_t *ret=malloc(sizeof(encr_t));
	if(!ret)
		return NULL;
	ret->constructed=false;
	
	ret->record_type=record_type;
	ret->subtype=subtype;
	ret->ncases=ncases;
	ret->constructed=true;
	
	return ret;
}

bool encr_destroy(encr_t *haystack){
	if(!haystack)
		return false;
	if(!haystack->constructed)
		return false;
	
	haystack->constructed=false;
	free(haystack);
	
	return true;
}
