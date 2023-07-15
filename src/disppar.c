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

disppar_t *disppar_fnew(FILE *sys_handle){
	if(!sys_handle)
		return NULL;
	int32_t record_type=-1;
	int32_t subtype=-1;
	int32_t size_check=-1;
	int32_t count=-1;

	fread(&record_type,sizeof(int32_t),1,sys_handle);
	if(record_type!=7 || ferror(sys_handle) || feof(sys_handle))
		return NULL;

	fread(&subtype,sizeof(int32_t),1,sys_handle);
	if(subtype!=11 || ferror(sys_handle) || feof(sys_handle))
		return NULL;

	fread(&size_check,sizeof(int32_t),1,sys_handle);
	if(size_check!=sizeof(int32_t) || ferror(sys_handle) || feof(sys_handle))
		return NULL;
	
	fread(&count,sizeof(int32_t),1,sys_handle);
	if(count<0 || ferror(sys_handle) || feof(sys_handle))
		return NULL;
	
	int32_t *measures=(count>0)?((int32_t*)calloc(count,sizeof(int32_t))):NULL;
	int32_t *widths=(count>0)?((int32_t*)calloc(count,sizeof(int32_t))):NULL;
	int32_t *alignments=(count>0)?((int32_t*)calloc(count,sizeof(int32_t))):NULL;

	if(count>0){
		for(int i=0;i<count;i++){
			fread(&(measures[i]),sizeof(int32_t),1,sys_handle);
			if(!one_of(measures[i],4,MEASURE_NOMINAL,MEASURE_ORDINAL,MEASURE_SCALE,MEASURE_UNKNOWN) || ferror(sys_handle) || feof(sys_handle)){
				free(measures);
				free(widths);
				free(alignments);
				return NULL;
			}
			fread(&(widths[i]),sizeof(int32_t),1,sys_handle);
			if(ferror(sys_handle) || feof(sys_handle)){
				free(measures);
				free(widths);
				free(alignments);
				return NULL;
			}
			fread(&(alignments[i]),sizeof(int32_t),1,sys_handle);
			if(!one_of(alignments[i],3,ALIGN_CENTER,ALIGN_LEFT,ALIGN_RIGHT) || ferror(sys_handle)){
				free(measures);
				free(widths);
				free(alignments);
				return NULL;
			}
		}
	}

	disppar_t *ret=disppar_new(record_type,subtype,count,measures,widths,alignments);
	free(widths);
	free(measures);
	free(alignments);
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