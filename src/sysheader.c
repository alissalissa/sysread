#include "include/sysheader.h"

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