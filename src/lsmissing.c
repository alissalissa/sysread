#include "lsmissing.h"

//struct MissingValue
lsmv_t *lsmv_new(bstream_t *var_name,int8_t n,bstream_t **missing_values){
    assert(var_name);
    assert(missing_values);

    lsmv_t *ret=(lsmv_t*)malloc(sizeof(lsmv_t));
    if(!ret)
        return NULL;
    ret->constructed=false;

    ret->var_name=bstream_cnew(var_name);
    if(!ret->var_name){
        free(ret);
        return NULL;
    }

    ret->n=n;

    ret->missing_values=(bstream_t**)calloc(n,sizeof(bstream_t*));
    if(!ret->missing_values){
        bstream_destroy(ret->var_name);
        free(ret);
        return NULL;
    }
    for(int8_t i=0;i<ret->n;i++){
        ret->missing_values[i]=bstream_cnew(missing_values[i]);
        if(!ret->missing_values[i]){
            for(int8_t j=0;j<=i;j++)
                bstream_destroy(ret->missing_values[j]);
            free(ret->missing_values);
            bstream_destroy(ret->var_name);
            free(ret);
            return NULL;
        }
    }

    ret->constructed=true;
    return ret;
}

bool lsmv_destroy(lsmv_t *haystack){
    if(!haystack)
        return false;
    if(!haystack->constructed)
        return false;

    haystack->constructed=false;
    if(!bstream_destroy(haystack->var_name))
        return false;

    for(int8_t i=0;i<haystack->n;i++)
        if(!bstream_destroy(haystack->missing_values[i]))
            return false;
	free(haystack->missing_values);

	free(haystack);
	return true;
}

//struct LSMVRecord
lsmvr_t *lsmvr_new(int32_t record_type,int32_t subtype,int32_t count,lsmv_t **records){
	if(record_type!=LSMVR_RECORD_TYPE || subtype!=LSMVR_SUBTYPE)
		return NULL;

	lsmvr_t *ret=(lsmvr_t*)malloc(sizeof(lsmvr_t));
	if(!ret)
		return NULL;
	ret->constructed=false;

	ret->record_type=record_type;
	ret->subtype=subtype;
	ret->count=count;

	ret->records=(lsmv_t**)calloc(ret->count,sizeof(lsmv_t*));
	if(!ret->records){
		free(ret);
		return NULL;
	}
	for(int32_t i=0;i<ret->count;i++){
		ret->records[i]=lsmv_new(records[i]->var_name,records[i]->n,records[i]->missing_values);
		if(!ret->records[i]){
			for(int32_t j=0;j<=i;j++)
				lsmv_destroy(ret->records[j]);
			free(ret->records);
			free(ret);
			return NULL;	
		}
	}
	
	ret->constructed=true;
	return ret;
}

lsmvr_t *lsmvr_fnew(FILE *handle){
	if(!handle)
		return NULL;

	int32_t record_type=0,subtype=0,size_check=0,count=0;
	fread(&record_type,sizeof(int32_t),1,handle);
	if(ferror(handle) || feof(handle) || record_type!=LSMVR_RECORD_TYPE){
		printf("Corrupted LSMVR record type...\n");
		return NULL;
	}

	fread(&subtype,sizeof(int32_t),1,handle);
	if(ferror(handle) || feof(handle) || subtype!=LSMVR_SUBTYPE){
		printf("Corrupted LSMVR subtype...\n");
		return NULL;
	}

	fread(&size_check,sizeof(int32_t),1,handle);
	if(ferror(handle) || feof(handle) || size_check!=1){
		printf("Corrupted LSMVR size check...\n");
		return NULL;
	}

	fread(&count,sizeof(int32_t),1,handle);
	if(ferror(handle) || feof(handle)){
		printf("Corrupted LSMVR...\n");
		return NULL;
	}

	int32_t index=0;
	lsmv_t **records=NULL;
	int32_t n_records=0;
	while(index<count){
		n_records++;
		bstream_t *var_name=bstream_new();
		fread(&var_name->length,sizeof(int32_t),1,handle);
		index+=sizeof(int32_t);

		var_name=bstream_new_wl(var_name->length);
		if(!var_name){
			for(int32_t i=0;i<n_records;i++)
				lsmv_destroy(records[i]);
			free(records);
			return NULL;
		}
		fread(var_name->stream,sizeof(char),var_name->length,handle);
		index+=var_name->length;

		int8_t n_missing_values=0;
		fread(&n_missing_values,sizeof(int8_t),1,handle);
		index+=sizeof(int8_t);
		if(n_missing_values<1 || n_missing_values>3){
			bstream_destroy(var_name);
			for(int32_t i=0;i<n_records;i++)
				lsmv_destroy(records[i]);
			free(records);
			return NULL;
		}

		int32_t value_length=0;
		fread(&value_length,sizeof(int32_t),1,handle);
		index+=sizeof(int32_t);
		if(value_length!=LSMVR_VALUE_LEN){
			bstream_destroy(var_name);
			for(int32_t i=0;i<n_records;i++)
				lsmv_destroy(records[i]);
			free(records);
			return NULL;
		}

		bstream_t *pre_split_values=bstream_new_wl(value_length*n_missing_values);
		if(!pre_split_values){
			bstream_destroy(var_name);
			for(int32_t i=0;i<n_records;i++)
				lsmv_destroy(records[i]);
			free(records);
			return NULL;
		}
		fread(pre_split_values->stream,sizeof(char),value_length*n_missing_values,handle);
		index+=(value_length*n_missing_values);
		if(!pre_split_values){
			bstream_destroy(var_name);
			for(int32_t i=0;i<n_records;i++)
				lsmv_destroy(records[i]);
			free(records);
			return NULL;
		}
		bstream_t **split_values=(bstream_t**)calloc(n_missing_values,sizeof(bstream_t*));
		if(!split_values){
			bstream_destroy(pre_split_values);
			bstream_destroy(var_name);
			for(int32_t i=0;i<n_records;i++)
				lsmv_destroy(records[i]);
			free(records);
			return NULL;
		}
		for(int8_t i=0;i<n_missing_values;i++){
			split_values[i]=bstream_new_wl(value_length);
			if(!split_values[i]){
				bstream_destroy(pre_split_values);
				bstream_destroy(var_name);
				for(int32_t i=0;i<n_records;i++)
					lsmv_destroy(records[i]);
				free(records);
				for(int8_t j=0;j<=i;j++)
					bstream_destroy(split_values[j]);
				free(split_values);
				return NULL;
			}
			split_values[i]=memcpy(split_values[i],pre_split_values+(value_length*i),value_length);
			if(!split_values[i]){
				bstream_destroy(pre_split_values);
				bstream_destroy(var_name);
				for(int32_t i=0;i<n_records;i++)
					lsmv_destroy(records[i]);
				free(records);
				for(int8_t j=0;j<=i;j++)
					bstream_destroy(split_values[j]);
				free(split_values);
				return NULL;
			}
		}
		records=(lsmv_t**)realloc(records,n_records*sizeof(lsmv_t*));
		if(!records){
			bstream_destroy(pre_split_values);
			bstream_destroy(var_name);
			for(int32_t i=0;i<n_records;i++)
				lsmv_destroy(records[i]);
			free(records);
			for(int8_t j=0;j<=n_missing_values;j++)
				bstream_destroy(split_values[j]);
			free(split_values);
			return NULL;
		}
		records[n_records-1]=lsmv_new(var_name,n_missing_values,split_values);
		bstream_destroy(pre_split_values);
		bstream_destroy(var_name);
		for(int8_t j=0;j<=n_missing_values;j++)
			bstream_destroy(split_values[j]);
		free(split_values);
		if(!records[n_records-1]){
			for(int32_t i=0;i<n_records;i++)
				lsmv_destroy(records[i]);
			free(records);
			return NULL;
		}
	}

	lsmvr_t *ret=lsmvr_new(record_type,subtype,n_records,records);
	if(!ret){
		for(int32_t i=0;i<n_records;i++)
			lsmv_destroy(records[i]);
		free(records);
		return NULL;
	}
	return ret;
}

bool lsmvr_destroy(lsmvr_t *haystack){
	if(!haystack)
		return false;
	if(!haystack->constructed)
		return false;

	for(int32_t i=0;i<haystack->count;i++){
		if(!lsmv_destroy(haystack->records[i]))
			return false;
	}
	free(haystack->records);
	free(haystack);
	return true;
}
