#include "sysvar.h"

sysvar_t *sysvar_new(int32_t rec_type,int32_t type,int32_t has_var_label,int32_t n_missing_values,int32_t print,int32_t write,char *name,int32_t label_length,char *label,double *missing_values){
	sysvar_t *ret=(sysvar_t*)malloc(sizeof(sysvar_t));
	
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
	ret->missing_values=(double*)calloc(sizeof(float),ret->n_missing_values);
	ret->constructed=true;

	return ret;

}

sysvar_t *sysvar_fnew(FILE *sys_handle){

	int32_t rec_type=-1;
	fread(&rec_type,sizeof(int32_t),1,sys_handle);
	if(ferror(sys_handle) || feof(sys_handle) || rec_type!=2){
		printf("Invalid/corrupted rec_type\n");
		return NULL;
	}

	int32_t type=-2;
	fread(&type,sizeof(int32_t),1,sys_handle);
	if(ferror(sys_handle) || feof(sys_handle) || type<-1){
		printf("Invalid/corrupted type\n");
		return NULL;
	}

	int32_t has_var_label=0;
	int32_t n_missing_values=0;
	int32_t print=-1;
	int32_t write=-1;
	char *name=(char*)calloc(sizeof(char),8);
	int32_t label_length=0;
	char *label=NULL;
	double *missing_values=NULL;

	if(type>=0){

		fread(&has_var_label,sizeof(int32_t),1,sys_handle);
		if(ferror(sys_handle) || feof(sys_handle) || has_var_label<0){
			printf("Invalid/corrupted has_var_label\n");
			free(name);
			return NULL;
		}

		fread(&n_missing_values,sizeof(int32_t),1,sys_handle);
		if(ferror(sys_handle) || feof(sys_handle) || n_missing_values<-3 || n_missing_values>3){
			printf("Invalid/corrupted n_missing_values\n");
			free(name);
			return NULL;
		}

		fread(&print,sizeof(int32_t),1,sys_handle);
		if(ferror(sys_handle) || feof(sys_handle) || print<0 || print>41){
			printf("Invalid/corrupted print format\n");
			free(name);
			return NULL;
		}

		fread(&write,sizeof(int32_t),1,sys_handle);
		if(ferror(sys_handle) || feof(sys_handle) || write<0 || write>41){
			printf("Invalid/corrupted write format\n");
			free(name);
			return NULL;
		}

		fread(name,sizeof(char),VAR_NAME_SIZE,sys_handle);
		if(ferror(sys_handle) || feof(sys_handle)){
			printf("Invalid/corrupted name\n");
			free(name);
			return NULL;
		}

		if(has_var_label){
			fread(&label_length,sizeof(int32_t),1,sys_handle);
			if(ferror(sys_handle) || feof(sys_handle) || label_length<0){
				printf("Invalid/corrupted label_length\n");
				free(name);
				return NULL;
			}
			if(label_length){
				label=(char*)calloc(sizeof(char),label_length);
				fread(label,sizeof(char),label_length,sys_handle);
				if(ferror(sys_handle) || feof(sys_handle)){
					free(label);
					free(name);
					printf("Error reading label\n");
					return NULL;
				}
			}else{
				label=NULL;
			}
		}

		if(n_missing_values){
			missing_values=(double*)calloc(sizeof(double),abs(n_missing_values));
			fread(missing_values,sizeof(double),abs(n_missing_values),sys_handle);
			if(ferror(sys_handle)){
				if(name)
					free(name);
				if(label)
					free(label);
				free(missing_values);
				printf("Error readingg missing_values\n");
				return NULL;
			}
		}
		
	}

	sysvar_t *ret=sysvar_new(rec_type,type,has_var_label,n_missing_values,print,write,name,label_length,label,missing_values);
	if(name)
		free(name);
	if(label)
		free(label);
	if(missing_values)
		free(missing_values);
	return ret;

}

bool sysvar_destroy(sysvar_t *haystack){

	if(!haystack->constructed)
		return false;

	if(haystack->name)
		free(haystack->name);
	if(haystack->label)
		free(haystack->label);
	if(haystack->missing_values)
		free(haystack->missing_values);
	free(haystack);

	return true;

}
