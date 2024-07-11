//
// Created by alissa on 6/27/24.
//
#include "../include/dfvar.h"

dfvar_t *dfvar_new(int32_t record_type,int32_t subtype,bstream_t *attributes){
	dfvar_t *ret=(dfvar_t*)malloc(sizeof(dfvar_t));
	ret->constructed=false;
	
	ret->record_type=record_type;
	ret->subtype=subtype;
	
	ret->attributes=bstream_cnew(attributes);
	if(!ret->attributes){
		free(ret);
		return NULL;
	}
	
	ret->constructed=true;
	return ret;
}

dfvar_t *dfvar_fnew(FILE *handle){
	int32_t record_type=0;
	fread(&record_type,sizeof(int32_t),1,handle);
	if(feof(handle) || ferror(handle) || record_type!=7){
		printf("Data file variable attributes record corrupted....\n");
		return NULL;
	}
	
	int32_t subtype=0;
	fread(&subtype,sizeof(int32_t),1,handle);
	if(feof(handle) || ferror(handle) || (subtype!=17 && subtype!=18)){
		printf("Data file variable attributes record corrupted....\n");
		return NULL;
	}
	
	int32_t size_check=0;
	fread(&size_check,sizeof(int32_t),1,handle);
	if(feof(handle) || ferror(handle) || size_check!=1){
		printf("Data file variable attributes record corrupted....\n");
		return NULL;
	}
	
	bstream_t *attributes=bstream_new();
	if(!attributes){
		printf("dfvar memory allocation error.....\n");
		return NULL;
	}
	fread(&attributes->length,sizeof(int32_t),1,handle);
	if((feof(handle) && attributes->length!=0) || ferror(handle)){
		printf("Data file variable attributes record corrupted....\n");
		return NULL;
	}
	
	attributes->stream=(char*)calloc(attributes->length,sizeof(char));
	if(!attributes->stream){
		printf("dfvar memory allocation error....\n");
		free(attributes);
		return NULL;
	}
	
	fread(attributes->stream,sizeof(char),attributes->length,handle);
	if(ferror(handle)){
		printf("dfvar corrupted....\n");
		if(attributes->stream)
			free(attributes->stream);
		if(attributes)
			free(attributes);
		return NULL;
	}
	
	dfvar_t *ret=dfvar_new(record_type,subtype,attributes);
	return ret;
}

bool dfvar_destroy(dfvar_t *haystack){
	assert(haystack);
	if(!haystack->constructed)
		return false;
	
	haystack->constructed=false;
	if(haystack->attributes)
		if(!bstream_destroy(haystack->attributes))
			return false;
	
	free(haystack);
	return true;
}
