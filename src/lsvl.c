//Implementation for long string values record
#include "lsvl.h"

//Individual label
syslsl_t *syslsl_new(bstream_t *value,bstream_t *label){
	syslsl_t *ret=(syslsl_t*)malloc(sizeof(syslsl_t));
	if(!ret)
		return NULL;
	ret->value=bstream_cnew(value);
	if(!ret->value){
		free(ret);
		return NULL;
	}
	ret->label=bstream_cnew(label);
	if(!ret->label){
		bstream_destroy(ret->value);
		free(ret);
		return NULL;
	}
	ret->constructed=true;
	return ret;
}

bool syslsl_destroy(syslsl_t *haystack){
	if(!haystack)
		return false;
	if(!haystack->constructed)
		return false;
	haystack->constructed=false;
	if(!bstream_destroy(haystack->value))
		return false;
	if(!bstream_destroy(haystack->label))
		return false;
	return true;
}