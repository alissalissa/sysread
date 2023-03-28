#include "mrset.h"

sysmrset_t *sysmrset_new(int32_t record_type,int32_t subtype,int32_t size,int32_t count,int32_t *length,char **mrsets){
	sysmrset_t *ret=(sysmrset_t*)malloc(sizeof(sysmrset_t));
	ret->record_type=record_type;
	ret->subtype=subtype;
	ret->size=size;
	ret->count=count;
	if(!length && count){
		free(ret);
		printf("Multiple response set record corrupted...\n");
		return NULL;
	}else if(!count){
		ret->length=NULL;
		ret->mrsets=NULL;
	}else{
		ret->length=(int32_t*)calloc(count,sizeof(int32_t));
		memcpy(ret->length,length,sizeof(int32_t)*count);
		ret->mrsets=(char**)calloc(count,sizeof(char*));
		for(int i=0;i<count;i++){
			ret->mrsets[i]=(char*)calloc(ret->length[i],sizeof(char));
			memcpy(ret->mrsets[i],mrsets[i],length[i]);
		}
	}
	ret->constructed=true;
	return ret;
}
