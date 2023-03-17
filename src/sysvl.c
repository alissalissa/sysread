#include "sysvl.h"

//sysvarlabel_t functions
sysvarlabel_t *sysvarlabel_new(char *value,char label_length,char *label){
	
	if(!value || !label) return NULL;

	sysvarlabel_t *ret=(sysvarlabel_t*)malloc(sizeof(sysvarlabel_t));
	
	ret->value=(char*)calloc(VL_VALUE_LENGTH,sizeof(char));
	memcpy(ret->value,value,VL_VALUE_LENGTH);

	ret->label_length=label_length;

	ret->label=(char*)calloc(ret->label_length,sizeof(char));
	memcpy(ret->label,label,ret->label_length);

	ret->constructed=true;
	return ret;
}

sysvarlabel_t *sysvarlabel_fnew(FILE *handle){

	if(!handle) return NULL;

	char *value=(char*)calloc(VL_VALUE_LENGTH,sizeof(char));
	fread(value,sizeof(char),VL_VALUE_LENGTH,handle);
	if(ferror(handle) || feof(handle)){
		printf("Label value corrupted...\n");
		free(value);
		return NULL;
	}

	char label_length='\0';
	fread(&label_length,sizeof(char),1,handle);
	if(ferror(handle) || feof(handle) || label_length<0){
		printf("Label length corrupted...\n");
		free(value);
		return NULL;
	}

	char *label=(char*)calloc((size_t)label_length,sizeof(char));
	fread(label,sizeof(char),(size_t)label_length,handle);
	if(ferror(handle) || feof(handle)){
		printf("Unable to read label...\n");
		free(value);
		free(label);
		return NULL;
	}

	sysvarlabel_t *ret=sysvarlabel_new(value,label_length,label);
	free(value);
	free(label);
	return ret;

}

bool sysvarlabel_destroy(sysvarlabel_t *haystack){
	if(!haystack) return false;
	else if(!haystack->constructed) return false;
	free(haystack->value);
	free(haystack->label);
	free(haystack);
	return true;
}

//syslabels_t functions
syslabels_t *syslabels_new(char record_type,int32_t label_count,sysvarlabel_t* labels){
	if(!labels) return NULL;
	else if(record_type!=2) return NULL;
	syslabels_t *ret=(syslabels_t*)malloc(sizeof(syslabels_t));
	ret->record_type=record_type;
	ret->label_count=label_count;
	ret->labels=(sysvarlabel_t*)calloc(ret->label_count,sizeof(sysvarlabel_t));
	memcpy(ret->labels,labels,ret->label_count);
	return ret;
}
