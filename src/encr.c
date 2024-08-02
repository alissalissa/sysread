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

encr_t *encr_fnew(FILE *handle){
	if(!handle)
		return NULL;
	
	int32_t record_type=0;
	fread(&record_type,sizeof(int32_t),1,handle);
	if(ferror(handle) || feof(handle) || record_type!=7){
		printf("encr record type corrupted!\n");
		return NULL;
	}
	
	int32_t subtype=0;
	fread(&subtype,sizeof(int32_t),1,handle);
	if(ferror(handle) || feof(handle) || subtype!=16){
		printf("encr subtype corrupted!\n");
		return NULL;
	}
	
	int32_t size_check=0;
	fread(&size_check,sizeof(int32_t),1,handle);
	if(ferror(handle) || feof(handle) || size_check!=ENCR_SIZE_CHECK){
		printf("encr size check corrupted!\n");
		return NULL;
	}
	
	int32_t count=0;
	fread(&count,sizeof(int32_t),1,handle);
	if(ferror(handle) || feof(handle) || count!=ENCR_DATA_COUNT){
		printf("encr data count corrupted!\n");
		return NULL;
	}
	
	//This value is of unknown purpose, but must be set to 1
	int64_t unknown=0;
	fread(&unknown,sizeof(int64_t),1,handle);
	if(ferror(handle) || feof(handle) || unknown!=1){
		printf("encr unknown marker corrupted!\n");
		return NULL;
	}
	
	int64_t ncases=-2;
	fread(&ncases,sizeof(int64_t),1,handle);
	if(ferror(handle) || feof(handle) || ncases<-1){
		printf("encr ncases corrupted\n");
		return NULL;
	}
	
	encr_t *ret=encr_new(record_type,subtype,ncases);
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
