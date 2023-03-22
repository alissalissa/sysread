#include "sysinfo.h"

sysinfo_t *sysinfo_new(int32_t rec_type,int32_t subtype,int32_t size,int32_t count,int32_t *data){
	if(rec_type!=7 || subtype!=3 || size!=4 || count!=8) return NULL;

	sysinfo_t *ret=malloc(sizeof(sysinfo_t));
	//header assignment
	ret->rec_type=rec_type;
	ret->subtype=subtype;
	ret->size=size;
	ret->count=count;

	//data assignment
	ret->version_major=data[0];
	ret->version_minor=data[1];
	ret->version_revision=data[2];
	ret->machine_code=data[3];
	ret->floating_point_rep=data[4];
	ret->compression_code=data[5];
	ret->endianness=data[6];
	ret->character_code=data[7];

	ret->constructed=true;
	return ret;
}

sysinfo_t *sysinfo_fnew(FILE *handle){
	if(!handle) return NULL;
	int32_t rec_type=-1,subtype=-1,size=-1,count=-1;
	fread(&rec_type,sizeof(int32_t),1,handle);
	if(ferror(handle) || feof(handle) || rec_type!=7){
		printf("Sysfile integer info record corrupted...\n");
		return NULL;
	}
	fread(&subtype,sizeof(int32_t),1,handle);
	if(ferror(handle) || feof(handle) || subtype!=3){
		printf("Sysfile integer info record corrupted...\n");
		return NULL;
	}
	fread(&size,sizeof(int32_t),1,handle);
	if(ferror(handle) || feof(handle) || size!=4){
		printf("Sysfile integer info record corrupted...\n");
		return NULL;
	}
	fread(&count,sizeof(int32_t),1,handle);
	if(ferror(handle) || feof(handle) || count!=8){
		printf("Sysfile integer info record corrupted...\n");
		return NULL;
	}
	int32_t *data=(int32_t*)calloc(8,sizeof(int32_t));
	fread(data,sizeof(int32_t),8,handle);
	if(ferror(handle) || feof(handle)){
		printf("Error reading integer data...\n");
		free(data);
		return NULL;
	}

	sysinfo_t *ret=sysinfo_new(rec_type,subtype,size,count,data);
	free(data);
	return ret;
}

bool sysinfo_destroy(sysinfo_t *haystack){
	if(!haystack) return false;
	else if(!haystack->constructed) return false;
	free(haystack);
	return true;
}
