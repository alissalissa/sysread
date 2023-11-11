#include "vlsr.h"

vlsr_t *vlsr_new(int32_t record_type,int32_t subtype,int32_t n,bstream_t *keys,bstream_t *values){
	if(record_type!=VLSR_RECORD_TYPE || subtype!=VLSR_SUBTYPE)
		return NULL;
	vlsr_t *ret=(vlsr_t*)malloc(sizeof(vlsr_t));
	ret->record_type=record_type;
	ret->subtype=subtype;
	ret->n=n;
	if(ret->n>0){
		if(!keys || !values){
			free(ret);
			return NULL;
		}
		ret->keys=(bstream_t*)calloc(ret->n,sizeof(bstream_t));
		ret->values=(bstream_t*)calloc(ret->n,sizeof(bstream_t));
		for(int i=0;i<ret->n;i++){
			ret->keys[i].length=keys[i].length;
			ret->keys[i].stream=(char*)calloc(ret->keys[i].length,sizeof(char));
			memcpy(ret->keys[i].stream,keys[i].stream,ret->keys[i].length);
			ret->values[i].length=values[i].length;
			ret->values[i].stream=(char*)calloc(ret->values[i].length,sizeof(char));
			memcpy(ret->values[i].stream,values[i].stream,ret->values[i].length);
		}
	}
	ret->constructed=true;
	return ret;
}

bool vlsr_destroy(vlsr_t *haystack){
	if(!haystack)
		return false;
	if(!haystack->constructed)
		return false;
	if(haystack->keys){
		for(int i=0;i<haystack->n;i++)
			if(haystack->keys[i].stream)
				free(haystack->keys[i].stream);
		free(haystack->keys);
	}
	if(haystack->values){
		for(int i=0;i<haystack->n;i++)
			if(haystack->values[i].stream)
				free(haystack->values[i].stream);
		free(haystack->values);
	}
	haystack->constructed=false;
	free(haystack);
	return true;
}