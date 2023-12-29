//Implementations for a long string value labels record
#include "lsvlabel.h"

//lsvlabel_t
lsvlabel_t *lsvlabel_new(bstream_t *var_name,int32_t count,bstream_t **value,bstream_t **label){
	lsvlabel_t *ret=(lsvlabel_t*)malloc(sizeof(lsvlabel_t));
	if(!ret)
		return NULL;
	ret->constructed=false;
	ret->var_name=bstream_cnew(var_name);
	if(!ret->var_name){
		free(ret);
		return NULL;
	}
	ret->count=count;
	ret->value=(bstream_t**)calloc(ret->count,sizeof(bstream_t*));
	if(!ret->value){
		bstream_destroy(ret->var_name);
		free(ret);
		return NULL;
	}
	ret->label=(bstream_t**)calloc(ret->count,sizeof(bstream_t*));
	if(!ret->label){
		free(ret->value);
		bstream_destroy(ret->var_name);
		free(ret);
		return NULL;
	}
	for(int32_t i=0;i<ret->count;i++){
		ret->value[i]=bstream_cnew(value[i]);
		ret->label[i]=bstream_cnew(label[i]);
		if(!ret->value[i] || !ret->label[i]){
			for(int32_t j=0;j<=i;j++)
				bstream_mass_destroy(2,ret->value[j],ret->label[j]);
			bstream_destroy(var_name);
			free(ret);
			return NULL;
		}
	}
	return ret;
}

bool lsvlabel_destroy(lsvlabel_t *haystack){
	if(!haystack)
		return false;
	if(!haystack->constructed)
		return false;
	haystack->constructed=false;
	for(int32_t i=0;i<haystack->count;i++)
		if(!bstream_mass_destroy(2,haystack->value[i],haystack->label[i]))
			return false;
	free(haystack->value);
	free(haystack->label);
	if(!bstream_destroy(haystack->var_name))
		return false;
	free(haystack);
	return true;
}

//lsvlabel_list_t
lsvlabel_list_t *lsvlabel_list_new(int32_t record_type,int32_t subtype,int32_t n_labels,bstream_t *var_name,lsvlabel_t **labels){
	if(record_type!=LSVLABEL_RECORD_TYPE || subtype!=LSVLABEL_SUBTYPE)
		return NULL;
	lsvlabel_list_t *ret=(lsvlabel_list_t*)malloc(sizeof(lsvlabel_t));
	if(!ret)
		return NULL;
	ret->constructed=false;
	ret->record_type=record_type;
	ret->subtype=subtype;
	ret->n_labels=n_labels;
	ret->labels=(lsvlabel_t**)calloc(ret->n_labels,sizeof(lsvlabel_t*));
	for(int i=0;i<ret->n_labels;i++){
		ret->labels[i]=lsvlabel_new(labels[i]->var_name,labels[i]->count,labels[i]->value,labels[i]->label);
		if(!ret->labels[i]){
			for(int j=0;j<=i;j++)
				lsvlabel_destroy(ret->labels[j]);
			free(ret->labels);
			free(ret);
			return NULL;
		}
	}
	ret->constructed=true;
	return ret;
}

lsvlabel_list_t *lsvlabel_list_fnew(FILE *handle){
	assert(handle);
	int32_t record_type=-1;
	int32_t subtype=-1;
	int32_t size_check=-1;

	fread(&record_type,sizeof(int32_t),1,handle);
	if(record_type!=LSVLABEL_RECORD_TYPE || ferror(handle) || feof(handle)){
		printf("LSVLabel file segment corrupted...\n");
		return NULL;
	}

	fread(&subtype,sizeof(int32_t),1,handle);
	if(subtype!=LSVLABEL_SUBTYPE || ferror(handle) || feof(handle)){
		printf("LSVLabel file segment corrupted...\n");
		return NULL;
	}

	fread(&size_check,sizeof(int32_t),1,handle);
	if(size_check!=1 || ferror(handle) || feof(handle)){
		printf("LSVLabel file segment corrupted...\n");
		return NULL;
	}

	//Section length
	int32_t count=-1;
	fread(&count,sizeof(int32_t),1,handle);
	if(count<0 || ferror(handle)){
		printf("LSVLabel file segment corrupted...\n");
		return NULL;
	}

	long marker=0;
	while(marker<count){
		int32_t var_name_length=-1;
		fread(&var_name_length,sizeof(int32_t),1,handle);
		marker+=sizeof(int32_t);
		if(var_name_length<=0 || ferror(handle) || feof(handle)){
			printf("LSVLabel file segment corrupted...\n");
			return NULL;
		}
		bstream_t *var_name=bstream_new_wl(var_name_length);
		if(!var_name){
			printf("Error allocating memory for LSVLabel List var name....\n");
			return NULL;
		}
		fread(var_name->stream,sizeof(char),var_name_length,handle);
		marker+=var_name_length;
	}
}

bool lsvlabel_list_destroy(lsvlabel_list_t *haystack){
	if(!haystack)
		return false;
	if(!haystack->constructed)
		return false;
	haystack->constructed=false;
	for(int i=0;i<haystack->n_labels;i++)
		if(!lsvlabel_destroy(haystack->labels[i]))
			return false;
	free(haystack->labels);
	free(haystack);
	return true;
}
