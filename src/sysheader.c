#include "sysheader.h"

sysheader_t *sysheader_new(bstream_t *rt,bstream_t *pn,int32_t lc,int32_t ncs,int32_t comp,int32_t wi,int32_t nc,float b,bstream_t *cd,bstream_t *ct,bstream_t *fl,bstream_t *pd){

	sysheader_t *ret=(sysheader_t*)malloc(sizeof(sysheader_t));
	if(!ret)
		return NULL;

	//char* elements are NOT c strings, so we can't use strcpy.
	//	Instead, we have to use memcpy
	ret->rec_type=bstream_new_wl(REC_TYPE_SIZE);
	if(!ret->rec_type){
		free(ret);
		return NULL;
	}
	memcpy(ret->rec_type->stream,rt->stream,REC_TYPE_SIZE);

	//Similar process with the other char* variables
	ret->prod_name=bstream_new_wl(PROD_NAME_SIZE);
	if(!ret->prod_name){
		free(ret);
		return NULL;
	}
	memcpy(ret->prod_name->stream,pn->stream,PROD_NAME_SIZE);
	
	ret->creation_date=bstream_new_wl(CREATION_DATE_SIZE);
	if(!ret->creation_date){
		free(ret);
		return NULL;
	}
	memcpy(ret->creation_date->stream,cd->stream,CREATION_DATE_SIZE);
	
	ret->creation_time=bstream_new_wl(CREATION_TIME_SIZE);
	if(!ret->creation_time){
		free(ret);
		return NULL;
	}
	memcpy(ret->creation_time->stream,ct->stream,CREATION_TIME_SIZE);

	ret->file_label=bstream_new_wl(FILE_LABEL_SIZE);
	if(!ret->file_label){
		free(ret);
		return NULL;
	}
	memcpy(ret->file_label->stream,fl->stream,FILE_LABEL_SIZE);
	
	ret->padding=bstream_new_wl(PADDING_SIZE);
	if(!ret->padding){
		free(ret);
		return NULL;
	}
	memcpy(ret->padding->stream,pd->stream,PADDING_SIZE);

	//Then it's just a straight copy for the others
	ret->layout_code=lc;
	ret->nominal_case_size=ncs;
	ret->compression=comp;
	ret->weight_index=wi;
	ret->ncases=nc;
	ret->bias=b;

	//Flag that this struct has been successfully constructed
	ret->constructed=true;

	return ret;

}

sysheader_t *sysheader_fnew(FILE *sys_file_handle){
	//the actual allocation for the sysheader_t will occur within sysheader_new()
	bstream_t *rec_type=bstream_new_wl(REC_TYPE_SIZE);
	if(!rec_type)
		return NULL;
	bstream_t *prod_name=bstream_new_wl(PROD_NAME_SIZE);
	if(!prod_name){
		bstream_destroy(rec_type);
		return NULL;
	}
	int32_t layout_code=-1;
	int32_t nominal_case_size=-1;
	int32_t compression=-1;
	int32_t weight_index=-1;
	int32_t ncases=-1;
	float bias=-1.0;
	bstream_t *creation_date=bstream_new_wl(CREATION_DATE_SIZE);
	if(!creation_date){
		bstream_mass_destroy(2,rec_type,prod_name);
		return NULL;
	}
	bstream_t *creation_time=bstream_new_wl(CREATION_TIME_SIZE);
	if(!creation_time){
		bstream_mass_destroy(3,rec_type,prod_name,creation_date);
		return NULL;
	}
	bstream_t *file_label=bstream_new_wl(FILE_LABEL_SIZE);
	if(!file_label){
		bstream_mass_destroy(4,rec_type,prod_name,creation_date,creation_time);
		return NULL;
	}
	bstream_t *padding=bstream_new_wl(PADDING_SIZE);
	if(!padding){
		bstream_mass_destroy(5,rec_type,prod_name,creation_date,creation_time,file_label);
		return NULL;
	}

	//file operations
	fread(rec_type->stream,sizeof(char),REC_TYPE_SIZE,sys_file_handle);
	if(ferror(sys_file_handle) || feof(sys_file_handle) || strcmp(bstream_cstr(bstream_subset(*rec_type,0,3)),"$FL")){
		bstream_mass_destroy(6,rec_type,prod_name,creation_date,creation_time,file_label,padding);
		return NULL;
	}
	
	fread(prod_name->stream,sizeof(char),PROD_NAME_SIZE,sys_file_handle);
	if(ferror(sys_file_handle) || feof(sys_file_handle) || strcmp(bstream_cstr(bstream_subset(*prod_name,0,19)),"@(#) SPSS DATA FILE")){
		bstream_mass_destroy(6,rec_type,prod_name,creation_date,creation_time,file_label,padding);
		return NULL;
	}

	fread(&layout_code,sizeof(int32_t),1,sys_file_handle);
	if(ferror(sys_file_handle) || feof(sys_file_handle) || layout_code!=2){
		bstream_mass_destroy(6,rec_type,prod_name,creation_date,creation_time,file_label,padding);
		return NULL;
	}

	fread(&nominal_case_size,sizeof(int32_t),1,sys_file_handle);
	if(ferror(sys_file_handle) || feof(sys_file_handle)){
		bstream_mass_destroy(6,rec_type,prod_name,creation_date,creation_time,file_label,padding);
		return NULL;
	}

	fread(&compression,sizeof(int32_t),1,sys_file_handle);
	if(ferror(sys_file_handle) || feof(sys_file_handle) || (compression<0 || compression>2)){
		bstream_mass_destroy(6,rec_type,prod_name,creation_date,creation_time,file_label,padding);
		return NULL;
	}

	fread(&weight_index,sizeof(int32_t),1,sys_file_handle);
	if(ferror(sys_file_handle) || feof(sys_file_handle) || weight_index<0){
		bstream_mass_destroy(6,rec_type,prod_name,creation_date,creation_time,file_label,padding);
		return NULL;
	}

	fread(&ncases,sizeof(int32_t),1,sys_file_handle);
	if(ferror(sys_file_handle) || feof(sys_file_handle) || ncases<-1){
		bstream_mass_destroy(6,rec_type,prod_name,creation_date,creation_time,file_label,padding);
		return NULL;
	}

	fread(&bias,sizeof(float),1,sys_file_handle);
	if(ferror(sys_file_handle) || feof(sys_file_handle)){
		bstream_mass_destroy(6,rec_type,prod_name,creation_date,creation_time,file_label,padding);
		return NULL;
	}
	if(bias!=100.0){
		printf("Either the file is of different endianness,\nor there's a read error.");
		return NULL;
	}

	fread(creation_date->stream,sizeof(char),CREATION_DATE_SIZE,sys_file_handle);
	if(ferror(sys_file_handle) || feof(sys_file_handle) || !verify_date_format(bstream_cstr(*creation_date))){
		bstream_mass_destroy(6,rec_type,prod_name,creation_date,creation_time,file_label,padding);
		return NULL;
	}

	fread(creation_time->stream,sizeof(char),CREATION_TIME_SIZE,sys_file_handle);
	if(ferror(sys_file_handle) || feof(sys_file_handle) || !verify_time_format(bstream_cstr(*creation_time))){
		bstream_mass_destroy(6,rec_type,prod_name,creation_date,creation_time,file_label,padding);
		return NULL;
	}

	fread(file_label->stream,sizeof(char),FILE_LABEL_SIZE,sys_file_handle);
	if(ferror(sys_file_handle) || feof(sys_file_handle)){
		bstream_mass_destroy(6,rec_type,prod_name,creation_date,creation_time,file_label,padding);
		return NULL;
	}

	fread(padding->stream,sizeof(char),PADDING_SIZE,sys_file_handle);
	if(ferror(sys_file_handle)){
		bstream_mass_destroy(6,rec_type,prod_name,creation_date,creation_time,file_label,padding);
		return NULL;
	}

	//There have been no read or EOF errors, so construct the type
	sysheader_t *ret=sysheader_new(rec_type,prod_name,layout_code,nominal_case_size,compression,weight_index,ncases,bias,creation_date,creation_time,file_label,padding);
	bstream_mass_destroy(6,rec_type,prod_name,creation_date,creation_time,file_label,padding);
	return ret;

}

bool sysheader_destroy(sysheader_t *haystack){
	if(!haystack)
		return false;
	if(!haystack->constructed)
		return false;
	bstream_mass_destroy(6,haystack->rec_type,haystack->prod_name,haystack->creation_date,haystack->creation_time,haystack->file_label,haystack->padding);
	free(haystack);
	return true;
}