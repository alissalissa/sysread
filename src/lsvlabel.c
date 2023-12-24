//Implementations for a long string value labels record
#include "lsvlabel.h"

//lsvlabel_t
lsvlabel_t *lsvlabel_new(bstream_t *value,bstream_t *label){
	lsvlabel_t *ret=(lsvlabel_t*)malloc(sizeof(lsvlabel_t));
	if(!ret)
		return NULL;
	ret->constructed=false;
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

bool lsvlabel_destroy(lsvlabel_t *haystack){
	if(!haystack)
		return false;
	if(!haystack->constructed)
		return false;
	if(!bstream_destroy(haystack->value))
		return false;
	haystack->constructed=false;
	if(!bstream_destroy(haystack->label))
		return false;
	free(haystack);
	return true;
}