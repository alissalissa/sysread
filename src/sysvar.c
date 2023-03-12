#include "sysvar.h"

sysvar_t *sysvar_new(int32_t rec_type,int32_t type,int32_t has_var_label,int32_t n_missing_values,int32_t print,int32_t write,char *name,int32_t label_length,char *label,float *missing_values){
	sysvar_t *ret=(char*)malloc(sizeof(sysvar_t));
	
	ret->rec_type=rec_type;
	ret->type=type;
	ret->has_var_label=has_var_label;
	ret->n_missing_values=n_missing_values;
	ret->print=print;
	ret->write=write;
	ret->name=(char*)calloc(sizeof(char),VAR_NAME_SIZE);
	memcpy(ret->name,name,VAR_NAME_SIZE);
	ret->label_length=label_length;
	ret->label=(char*)calloc(sizeof(char),ret->label_length);
	memcpy(ret->label,label,ret->label_length);
	ret->missing_values=(float*)calloc(sizeof(float),ret->n_missing_values);
	ret->constructed=true;

	return ret;

}