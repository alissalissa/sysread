#include "sysvl.h"

//sysvarlabel_t functions
sysvarlabel_t *sysvarlabel_new(char *value,char label_length,char *label){
	
	if(value==NULL || label==NULL) return NULL;

	sysvarlabel_t *ret=(sysvarlabel_t*)malloc(sizeof(sysvarlabel_t));
	
	ret->value=(char*)calloc(VL_VALUE_LENGTH,sizeof(char));
	memcpy(ret->value,value,VL_VALUE_LENGTH);

	ret->label_length=label_length;

	ret->label=(char*)calloc(ret->label_length,sizeof(char));
	memcpy(ret->label,label,ret->label_length);

	return ret;
}