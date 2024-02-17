//Implementations for a long string value labels record
#include "lsvlabel.h"

//lsvlabel_t
lsvlabel_t *lsvlabel_new(bstream_t *value,bstream_t *label){
	lsvlabel_t *ret=(lsvlabel_t*)malloc(sizeof(lsvlabel_t));
	if(!ret)
		return NULL;
	ret->constructed=false;
	ret->value=bstream_cnew(value);
	if(!ret->value){
		free(ret);
		return NULL;
	}
	ret->label=bstream_cnew(label);
	if(!ret->label){
		bstream_destroy(ret->value);
		free(ret);
		return NULL;
	}
	ret->constructed=true;
	return ret;
}

lsvlabel_t *lsvlabel_cnew(lsvlabel_t *haystack){
	assert(haystack);
	lsvlabel_t *ret=lsvlabel_new(haystack->value,haystack->label);
	return ret;
}

bool lsvlabel_destroy(lsvlabel_t *haystack){
	if(!haystack)
		return false;
	if(!haystack->constructed)
		return false;
	haystack->constructed=false;
	if(!bstream_destroy(haystack->value))
		return false;
	if(!bstream_destroy(haystack->label))
		return false;
	free(haystack);
	return true;
}

//lsvar_t
lsvar_t *lsvar_new(bstream_t *var_name,int32_t size,lsvlabel_t **labels){
	assert(var_name);
	assert(labels);
	lsvar_t *ret=(lsvar_t*)malloc(sizeof(lsvar_t));
	if(!ret)
		return NULL;
	ret->constructed=false;
	ret->var_name=bstream_cnew(var_name);
	if(!ret->var_name){
		free(ret);
		return NULL;
	}
	ret->size=size;
	ret->labels=(lsvlabel_t**)calloc(ret->size,sizeof(lsvlabel_t*));
	if(!ret->labels){
		bstream_destroy(ret->var_name);
		free(ret);
		return NULL;
	}
	for(int32_t i=0;i<ret->size;i++){
		ret->labels[i]=lsvlabel_cnew(labels[i]);
		if(!ret->labels[i]){
			for(int32_t j=0;j<=i;j++)
				lsvlabel_destroy(ret->labels[j]);
			free(ret->labels);
			bstream_destroy(ret->var_name);
			free(ret);
			return NULL;
		}
	}
	ret->constructed=true;
	return ret;
}

bool lsvar_destroy(lsvar_t *haystack){
	if(!haystack)
		return false;
	if(!haystack->constructed)
		return false;
	haystack->constructed=false;
	if(!bstream_destroy(haystack->var_name))
		return false;
	for(int i=0;i<haystack->size;i++)
		if(!lsvlabel_destroy(haystack->labels[i]))
			return false;
	free(haystack->labels);
	free(haystack);
	return true;
}

//lsvlabel_list_t
lsvlabel_list_t *lsvlabel_list_new(int32_t record_type,int32_t subtype,int32_t size,lsvar_t **variables){
	if(record_type!=LSVLABEL_RECORD_TYPE || subtype!=LSVLABEL_SUBTYPE)
		return NULL;
	lsvlabel_list_t *ret=(lsvlabel_list_t*)malloc(sizeof(lsvlabel_list_t));
	if(!ret)
		return NULL;
	ret->constructed=false;
	ret->record_type=record_type;
	ret->subtype=subtype;
	ret->size=size;
	ret->variables=(lsvar_t**)calloc(ret->size,sizeof(lsvar_t*));
	if(!ret->variables){
		free(ret);
		return NULL;
	}
	for(int32_t i=0;i<ret->size;i++){
		ret->variables[i]=lsvar_new(variables[i]->var_name,variables[i]->size,variables[i]->labels);
		if(!ret->variables[i]){
			for(int32_t j=0;j<=i;j++)
				lsvar_destroy(ret->variables[j]);
			free(ret->variables);
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
	printf("record type = %d\n",record_type);

	fread(&subtype,sizeof(int32_t),1,handle);
	if(subtype!=LSVLABEL_SUBTYPE || ferror(handle) || feof(handle)){
		printf("LSVLabel file segment corrupted...\n");
		return NULL;
	}
	printf("subtype = %d\n",subtype);

	fread(&size_check,sizeof(int32_t),1,handle);
	if(size_check!=1 || ferror(handle) || feof(handle)){
		printf("LSVLabel file segment corrupted...\n");
		return NULL;
	}
	printf("size check = %d\n",size_check);

	//Section length
	int32_t count=-1;
	fread(&count,sizeof(int32_t),1,handle);
	if(count<0 || ferror(handle)){
		printf("LSVLabel file segment corrupted...\n");
		return NULL;
	}
	printf("%d bytes to be read...\n",count);

	int32_t marker=0;
	lsvar_t **variables=NULL;
	int32_t variable_count=0;
	while(marker<count){
		int32_t var_name_length=-1;
		fread(&var_name_length,sizeof(int32_t),1,handle);
		marker+=sizeof(int32_t);
		printf("var_name_length = %d\nmarker=%d\n",var_name_length,marker);
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
		printf("var name = %s\nmarker=%d\n",bstream_cstr(*var_name),marker);

		int32_t var_width=-1;
		fread(&var_width,sizeof(int32_t),1,handle);
		if(var_width<LSVLABEL_MIN_WIDTH || var_width>LSVLABEL_MAX_WIDTH){
			printf("LSVLabel variable width out of bounds...\n");
			bstream_destroy(var_name);
			return NULL;
		}else if(ferror(handle) || feof(handle)){
			printf("LSVLabel file segment corrupted...\n");
			bstream_destroy(var_name);
			return NULL;
		}
		marker+=sizeof(int32_t);
		printf("var width = %d\nmarker=%d\n",var_width,marker);

		int32_t n_labels=-1;
		fread(&n_labels,sizeof(int32_t),1,handle);
		if(n_labels<0 || ferror(handle) || feof(handle)){
			printf("LSVLabel file segment corrupted...\n");
			bstream_destroy(var_name);
			return NULL;
		}
		marker+=sizeof(int32_t);
		printf("%d labels\nmarker=%d\n",n_labels,marker);

		lsvlabel_t **labels=(lsvlabel_t**)calloc(n_labels,sizeof(lsvlabel_t*));
		if(!labels){
			printf("Memory allocation error...\n");
			bstream_destroy(var_name);
			return NULL;
		}
		for(int32_t i=0;i<n_labels;i++){
			int32_t value_length=-1;
			fread(&value_length,sizeof(int32_t),1,handle);
			if(ferror(handle) || feof(handle) || value_length<0){
				printf("Label corrupted....\n");
				for(int32_t j=0;j<=i;j++)
					lsvlabel_destroy(labels[j]);
				free(labels);
				bstream_destroy(var_name);
				return NULL;
			}
			marker+=sizeof(int32_t);
			printf("Value length = %d\nmarker=%d\\n",value_length,marker);

			bstream_t *value=bstream_new_wl((size_t)value_length);
			if(!value){
				for(int32_t j=0;j<=i;j++)
					lsvlabel_destroy(labels[j]);
				free(labels);
				bstream_destroy(var_name);
				//bstream_destroy(value);
				return NULL;
			}
			fread(value->stream,sizeof(char),value->length,handle);
			if(ferror(handle) || feof(handle)){
				printf("Label corrupted....\n");
				for(int32_t j=0;j<=i;j++)
					lsvlabel_destroy(labels[j]);
				free(labels);
				bstream_destroy(var_name);
				bstream_destroy(value);
				return NULL;
			}
			marker+=value->length;
			printf("value = %s\nmarker=%d\n",bstream_cstr(*value),marker);

			int32_t label_length=-1;
			fread(&label_length,sizeof(int32_t),1,handle);
			if(ferror(handle) || feof(handle) || label_length<0){
				printf("Label corrupted....\n");
				for(int32_t j=0;j<=i;j++)
					lsvlabel_destroy(labels[j]);
				free(labels);
				bstream_destroy(var_name);
				bstream_destroy(value);
				return NULL;
			}
			marker+=sizeof(int32_t);
			printf("label length = %d\nmarker=%d\n",label_length,marker);

			bstream_t *label=bstream_new_wl((size_t)label_length);
			if(!label){
				for(int32_t j=0;j<=i;j++)
					lsvlabel_destroy(labels[j]);
				free(labels);
				bstream_destroy(var_name);
				bstream_destroy(value);
				return NULL;
			}
			fread(label->stream,sizeof(char),label->length,handle);
			if(ferror(handle) || feof(handle)){
				printf("Label corrupted....\n");
				for(int32_t j=0;j<=i;j++)
					lsvlabel_destroy(labels[j]);
				free(labels);
				bstream_destroy(var_name);
				bstream_destroy(value);
				bstream_destroy(label);
				return NULL;
			}
			marker+=label_length;
			printf("label = %s\nmarker=%d\n",bstream_cstr(*label),marker);

			labels[i]=lsvlabel_new(value,label);
			bstream_mass_destroy(2,value,label);
			if(!labels[i]){
				printf("LSVLabel memory allocation error....\n");
				bstream_destroy(var_name);
				for(int32_t j=0;j<=i;j++)
					lsvlabel_destroy(labels[j]);
				free(labels);
				return NULL;
			}
		}
		printf("First loop exit\n");
		variable_count++;
		variables=(lsvar_t**)realloc(variables,sizeof(lsvar_t*)*variable_count);
		if(!variables){
			for(int32_t i=0;i<variable_count;i++)
				lsvar_destroy(variables[i]);
			for(int32_t i=0;i<n_labels;i++)
				lsvlabel_destroy(labels[i]);
			free(variables);
			free(labels);
			bstream_destroy(var_name);
			return NULL;
		}
		
		variables[variable_count-1]=lsvar_new(var_name,n_labels,labels);
		for(int32_t i=0;i<n_labels;i++)
			lsvlabel_destroy(labels[i]);
		free(labels);
		bstream_destroy(var_name);
	}
	printf("Second loop exit.\n");
	lsvlabel_list_t *ret=lsvlabel_list_new(record_type,subtype,variable_count,variables);
	printf("constructed\n");
	for(int32_t i=0;i<variable_count;i++)
		lsvar_destroy(variables[i]);
	printf("Individual variables freed\n");
	free(variables);
	printf("Overarch free\n");
	ret->constructed=true;
	return ret;
}

bool lsvlabel_list_destroy(lsvlabel_list_t *haystack){
	printf("Entering destroy!\n");
	if(!haystack)
		return false;
	if(!haystack->constructed)
		return false;
	haystack->constructed=false;
	printf("haystack->size=%d\n",haystack->size);
	for(int i=0;i<haystack->size;i++){
		if(!lsvar_destroy(haystack->variables[i]))
			return false;
		printf("%d\t",i);
	}
	printf("lsvar_destroy loop complete\n");
	free(haystack->variables);
	printf("haystack->variable freed\n");
	if(haystack)
		free(haystack);
	printf("haystack freed\n");
	return true;
}
