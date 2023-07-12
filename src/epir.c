//Implementation for sysepir_t
#include "epir.h"

sysepir_t *sysepir_new(int32_t record_type,int32_t subtype,bstream_t *info){
	sysepir_t *ret=(sysepir_t*)malloc(sizeof(sysepir_t));
	if(!ret){
		printf("Error allocating memory for sysepir_t...\n");
		return NULL;
	}
	ret->record_type=record_type;
	ret->subtype=subtype;
	ret->info=bstream_new();
	ret->info=bstream_cnew(info);
	if(!ret->info){
		printf("Error copying info stream in sysepir_t...\n");
		free(ret);
		return NULL;
	}
	ret->constructed=true;
	return ret;
}

sysepir_t *sysepir_fnew(FILE *sys_handle){
	if(!sys_handle) return NULL;
	int32_t record_type=-1;
	int32_t subtype=-1;
	int32_t size_check=-1;
	bstream_t *info=bstream_new();

	fread(&record_type,sizeof(int32_t),1,sys_handle);
	if(record_type!=EPIR_TYPE || ferror(sys_handle) || feof(sys_handle)){
		bstream_destroy(info);
		return NULL;
	}
	printf("EPIR type = %d\n",record_type);

	fread(&subtype,sizeof(int32_t),1,sys_handle);
	if(subtype!=EPIR_SUBTYPE || ferror(sys_handle) || feof(sys_handle)){
		bstream_destroy(info);
		return NULL;
	}
	printf("EPIR subtype = %d\n",subtype);

	fread(&size_check,sizeof(int32_t),1,sys_handle);
	if(size_check!=EPIR_SIZE || ferror(sys_handle) || feof(sys_handle)){
		bstream_destroy(info);
		return NULL;
	}
	printf("EPIR size check = %d\n",size_check);

	fread(&(info->length),sizeof(int32_t),1,sys_handle);
	printf("EPIR count = %d\n",info->length);
	info->stream=calloc(info->length,sizeof(char));
	fread(info->stream,sizeof(char),info->length,sys_handle);
	printf("EPIR string = %s\n",bstream_cstr(*info));

	sysepir_t *ret=sysepir_new(record_type,subtype,info);
	bstream_destroy(info);
	return ret;
}

bool sysepir_destroy(sysepir_t *haystack){
	if(!haystack->constructed) return false;
	bstream_destroy(haystack->info);
	free(haystack);
	return true;
}