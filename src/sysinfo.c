#include "sysinfo.h"

sysinfo_t *sysinfo_new(int32_t rec_type,int32_t subtype,int32_t size,int32_t count,...){
	if(rec_type!=7 || subtype!=3 || size!=4 || count!=8) return NULL;
	
	int32_t *data=calloc(8,sizeof(int32_t));
	va_list va_data;
	va_start(va_data,count);
	for(int i=0;i<count;i++)
		data[i]=(int32_t)va_arg(va_data,int32_t);
	va_end(va_data);

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

	free(data);
	ret->constructed=true;
	return ret;
}