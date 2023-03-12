#include "sysheader.h"

sysheader_t *sysheader_new(char *rt,char *pn,int32_t lc,int32_t ncs,int32_t comp,int32_t wi,int32_t nc,float b,char *cd,char *ct,char *fl,char *pd){

	sysheader_t *ret=(sysheader_t*)malloc(sizeof(sysheader_t));

	//char* elements are NOT c strings, so we can't use strcpy.
	//	Instead, we have to copy element by element using the macros from sysheader.h
	ret->rec_type=(char*)calloc(REC_TYPE_SIZE,sizeof(char));
	for(int i=0;i<REC_TYPE_SIZE;i++)
		ret->rec_type[i]=rt[i];

	//Similar process with the other char* variables
	ret->prod_name=(char*)calloc(PROD_NAME_SIZE,sizeof(char));
	for(int i=0;i<PROD_NAME_SIZE;i++)
		ret->prod_name[i]=pn[i];
	
	ret->creation_date=(char*)calloc(CREATION_DATE_SIZE,sizeof(char));
	for(int i=0;i<CREATION_DATE_SIZE;i++)
		ret->creation_date[i]=cd[i];
	
	ret->creation_time=(char*)calloc(CREATION_TIME_SIZE,sizeof(char));
	for(int i=0;i<CREATION_TIME_SIZE;i++)
		ret->creation_time[i]=ct[i];

	ret->file_label=(char*)calloc(FILE_LABEL_SIZE,sizeof(char));
	for(int i=0;i<FILE_LABEL_SIZE;i++)
		ret->file_label[i]=fl[i];
	
	ret->padding=(char*)calloc(PADDING_SIZE,sizeof(char));
	for(int i=0;i<PADDING_SIZE;i++)
		ret->padding[i]=pd[i];

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
	char *rec_type=calloc(REC_TYPE_SIZE,sizeof(char));
	char *prod_name=calloc(PROD_NAME_SIZE,sizeof(char));
	int32_t layout_code=-1;
	int32_t nominal_case_size=-1;
	int32_t compression=-1;
	int32_t weight_index=-1;
	int32_t ncases=-1;
	float bias=-1.0;
	char *creation_date=calloc(CREATION_DATE_SIZE,sizeof(char));
	char *creation_time=calloc(CREATION_TIME_SIZE,sizeof(char));
	char *file_label=calloc(FILE_LABEL_SIZE,sizeof(char));
	char *padding=calloc(PADDING_SIZE,sizeof(char));

	//file operations
	for(int i=0;i<REC_TYPE_SIZE;i++)
		rec_type[i]=(char)fgetc(sys_file_handle);
	if(ferror(sys_file_handle) || feof(sys_file_handle)){
		mass_free(6,rec_type,prod_name,creation_date,creation_time,file_label,padding);
		return NULL;
	}
	if(rec_type[0]!='$' || rec_type[1]!='F' || rec_type[2]!='L'){
		mass_free(6,rec_type,prod_name,creation_date,creation_time,file_label,padding);
		return NULL;
	}
	if(rec_type[3]!='2' && rec_type[3]!='3'){
		mass_free(6,rec_type,prod_name,creation_date,creation_time,file_label,padding);
		return NULL;
	}
	
	for(int i=0;i<PROD_NAME_SIZE;i++)
		prod_name[i]=(char)fgetc(sys_file_handle);
	if(ferror(sys_file_handle) || feof(sys_file_handle)){
		mass_free(6,rec_type,prod_name,creation_date,creation_time,file_label,padding);
		return NULL;
	}
	char *prod_name_test="@(#) SPSS DATA FILE";
	for(int i=0;i<PROD_NAME_CHECK_SIZE;i++){
		if(prod_name[i]!=prod_name_test[i]){
			mass_free(6,rec_type,prod_name,creation_date,creation_time,file_label,padding);
			return NULL;
		}
	}

	fread(&layout_code,sizeof(int32_t),1,sys_file_handle);
	if(ferror(sys_file_handle) || feof(sys_file_handle) || layout_code!=2){
		mass_free(6,rec_type,prod_name,creation_date,creation_time,file_label,padding);
		return NULL;
	}

	fread(&nominal_case_size,sizeof(int32_t),1,sys_file_handle);
	if(ferror(sys_file_handle) || feof(sys_file_handle)){
		mass_free(6,rec_type,prod_name,creation_date,creation_time,file_label,padding);
		return NULL;
	}

	fread(&compression,sizeof(int32_t),1,sys_file_handle);
	if(ferror(sys_file_handle) || feof(sys_file_handle) || (compression<0 || compression>2)){
		mass_free(6,rec_type,prod_name,creation_date,creation_time,file_label,padding);
		return NULL;
	}

	fread(&weight_index,sizeof(int32_t),1,sys_file_handle);
	if(ferror(sys_file_handle) || feof(sys_file_handle) || weight_index<0){
		mass_free(6,rec_type,prod_name,creation_date,creation_time,file_label,padding);
		return NULL;
	}

	fread(&ncases,sizeof(int32_t),1,sys_file_handle);
	if(ferror(sys_file_handle) || feof(sys_file_handle) || ncases<-1){
		mass_free(6,rec_type,prod_name,creation_date,creation_time,file_label,padding);
		return NULL;
	}

	fread(&bias,sizeof(float),1,sys_file_handle);
	if(ferror(sys_file_handle) || feof(sys_file_handle)){
		mass_free(6,rec_type,prod_name,creation_date,creation_time,file_label,padding);
		return NULL;
	}
	if(bias!=100.0){
		printf("Either the file is of different endianness,\nor there's a read error.");
		return NULL;
	}

	for(int i=0;i<CREATION_DATE_SIZE;i++)
		creation_date[i]=fgetc(sys_file_handle);
	if(ferror(sys_file_handle) || feof(sys_file_handle) || !verify_date_format(creation_date)){
		mass_free(6,rec_type,prod_name,creation_date,creation_time,file_label,padding);
		return NULL;
	}

	for(int i=0;i<CREATION_TIME_SIZE;i++)
		creation_time[i]=fgetc(sys_file_handle);
	if(ferror(sys_file_handle) || feof(sys_file_handle) || !verify_time_format(creation_time)){
		mass_free(6,rec_type,prod_name,creation_date,creation_time,file_label,padding);
		return NULL;
	}

	for(int i=0;i<FILE_LABEL_SIZE;i++)
		file_label[i]=fgetc(sys_file_handle);
	if(ferror(sys_file_handle) || feof(sys_file_handle)){
		mass_free(6,rec_type,prod_name,creation_date,creation_time,file_label,padding);
		return NULL;
	}

	for(int i=0;i<PADDING_SIZE;i++)
		padding[i]=fgetc(sys_file_handle);
	if(ferror(sys_file_handle) || feof(sys_file_handle)){
		mass_free(6,rec_type,prod_name,creation_date,creation_time,file_label,padding);
		return NULL;
	}

	//There have been no read or EOF errors, so construct the type
	sysheader_t *ret=sysheader_new(rec_type,prod_name,layout_code,nominal_case_size,compression,weight_index,ncases,bias,creation_date,creation_time,file_label,padding);
	mass_free(6,rec_type,prod_name,creation_date,creation_time,file_label,padding);
	return ret;

}