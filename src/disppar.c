//Implementation for variable display parameters
#include "disppar.h"

disppar_t *disppar_new(int32_t record_type,int32_t subtype,int32_t count,int32_t *measures,int32_t *widths,int32_t *alignments){
	if(count>0){
		if(!measures || !widths || !alignments)
			return NULL;
	}else if(count<0)
		return NULL;
	
	disppar_t *ret=(disppar_t*)malloc(sizeof(disppar_t));
	if(!ret) return NULL;
	ret->record_type=record_type;
	ret->subtype=subtype;
	ret->count=count;

	if(count==0){
		ret->measures=NULL;
		ret->widths=NULL;
		ret->alignments=NULL;
	}else{
		ret->measures=(int32_t*)calloc(ret->count,sizeof(int32_t));
		if(!ret->measures){
			free(ret);
			return NULL;
		}
		ret->widths=(int32_t*)calloc(ret->count,sizeof(int32_t));
		if(!ret->widths){
			free(ret->measures);
			free(ret);
			return NULL;
		}
		ret->alignments=(int32_t*)calloc(ret->count,sizeof(int32_t));
		if(!ret->alignments){
			free(ret->widths);
			free(ret->measures);
			free(ret);
			return NULL;
		}
		memcpy(ret->measures,measures,sizeof(int32_t)*ret->count);
		memcpy(ret->widths,widths,sizeof(int32_t)*ret->count);
		memcpy(ret->alignments,alignments,sizeof(int32_t)*ret->count);
	}
	ret->constructed=true;
	return ret;
}

bool disppar_destroy(disppar_t *haystack){
	if(!haystack) return false;
	if(!haystack->constructed) return false;
	free(haystack->measures);
	free(haystack->widths);
	free(haystack->alignments);
	free(haystack);
	return true;
}