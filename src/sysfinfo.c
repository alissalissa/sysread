#include "sysfinfo.h"

sysfltinfo_t *sysfltinfo_new(int32_t rec_type,int32_t subtype,int32_t size,int32_t count,double *data){
	sysfltinfo_t *ret=(sysfltinfo_t*)malloc(sizeof(sysfltinfo_t));
	if(!ret) return NULL;
	ret->rec_type=rec_type;
	ret->subtype=subtype;
	ret->size=size;
	ret->count=count;
	//data portion
	ret->missing=data[0];
	ret->highest=data[1];
	ret->lowest=data[2];
	ret->constructed=true;
	return ret;
}

sysfltinfo_t *sysfltinfo_fnew(FILE *handle){
	if(!handle) return NULL;
	int32_t *header=(int32_t*)calloc(4,sizeof(int32_t));
	fread(header,sizeof(int32_t),4,handle);
	if(ferror(handle) || feof(handle)){
		free(header);
		return NULL;
	}else if(header[0]!=7 || header[1]!=4 || header[2]!=8 || header[3]!=3){
		free(header);
		return NULL;
	}
	double *data=(double*)calloc(3,sizeof(double));
	fread(data,sizeof(double),3,handle);
	if(ferror(handle) || feof(handle)){
		free(data);
		free(header);
		return NULL;
	}
	sysfltinfo_t *ret=sysfltinfo_new(header[0],header[1],header[2],header[3],data);
	free(header);
	free(data);
	return ret;
}

bool sysfltinfo_destroy(sysfltinfo_t *haystack){
	if(!haystack) return false;
	else if(!haystack->constructed) return false;
	free(haystack);
	return true;
}
