#include "lsmissing.h"

//struct MissingValue
lsmv_t *lsmv_new(bstream_t *var_name,int8_t n,bstream_t **missing_values){
    printf("Entering lsmv_new....\n");
	assert(var_name);
    assert(missing_values);

    lsmv_t *ret=(lsmv_t*)malloc(sizeof(lsmv_t));
    if(!ret)
        return NULL;
	printf("0\n");
    ret->constructed=false;

    ret->var_name=bstream_cnew(var_name);
    if(!ret->var_name){
        free(ret);
        return NULL;
    }
	printf("1\n");

    ret->n=n;

    ret->missing_values=(bstream_t**)calloc(n,sizeof(bstream_t*));
    if(!ret->missing_values){
        bstream_destroy(ret->var_name);
        free(ret);
        return NULL;
    }
	printf("2\n");
    for(int8_t i=0;i<ret->n;i++){
		printf("%dth iteration\n",i);
		printf("missing_values[i]=%s\n",bstream_cstr(*(missing_values[i])));
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
	printf("3\n");

    ret->constructed=true;
	printf("Exiting lsmv_new....\n");
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
	printf("0\n");

	//Read the record type
	int32_t record_type=0,subtype=0,size_check=0,count=0;
	fread(&record_type,sizeof(int32_t),1,handle);
	if(ferror(handle) || feof(handle) || record_type!=LSMVR_RECORD_TYPE){
		printf("Corrupted LSMVR record type...\n");
		return NULL;
	}
	printf("record_type=%d\n",record_type);

	//Read the subtype
	fread(&subtype,sizeof(int32_t),1,handle);
	if(ferror(handle) || feof(handle) || subtype!=LSMVR_SUBTYPE){
		printf("Corrupted LSMVR subtype...\n");
		return NULL;
	}
	printf("subtype=%d\n",subtype);

	//Check byte size
	fread(&size_check,sizeof(int32_t),1,handle);
	if(ferror(handle) || feof(handle) || size_check!=1){
		printf("Corrupted LSMVR size check...\n");
		return NULL;
	}
	printf("size check=%d\n",size_check);

	//How many bytes total are contained in the subsequent section?
	fread(&count,sizeof(int32_t),1,handle);
	if(ferror(handle) || feof(handle)){
		printf("Corrupted LSMVR...\n");
		return NULL;
	}
	printf("%d bytes in total\n",count);

	int32_t index=0;
	lsmv_t **records=NULL;
	int32_t n_records=0;
	//cycle through {count} bytes
	while(index<count){
		//The first 4 bytes are the length of the current variable name
		int32_t var_name_length=0;
		fread(&var_name_length,sizeof(int32_t),1,handle);
		if(ferror(handle) || feof(handle)){
			printf("Error reading lsmissing var name length...\n");
			return NULL;
		}
		index+=sizeof(int32_t);
		printf("var_name_length = %d\n",var_name_length);

		//read in the actual var name
		bstream_t *var_name=bstream_new_wl(var_name_length);
		if(!var_name){
			printf("Error allocating lsmissing var name memory...\n");
			return NULL;
		}
		fread(var_name->stream,sizeof(char),var_name_length,handle);
		if(ferror(handle) || feof(handle)){
			printf("lsmissing file section corrupt...\n");
			if(var_name && var_name->stream)
				bstream_destroy(var_name);
			if(records){
				for(int i=0;i<n_records;i++)
					lsmv_destroy(records[i]);
				free(records);
			}
			return NULL;
		}
		//Increment the index
		index+=var_name_length;
		printf("var_name = ");
		bstream_print(*var_name);
		printf("\n");

		//Read in the number of missing values, stored in a missing byte
		char n_missing_values=0;
		fread(&n_missing_values,sizeof(char),1,handle);
		if(ferror(handle) || feof(handle)){
			printf("lsmissing file section corrupt...\n");
			bstream_destroy(var_name);
			if(records){
				for(int i=0;i<n_records;i++)
					lsmv_destroy(records[i]);
				free(records);
			}
			return NULL;
		}
		//Increment index
		index++;
		//TOOD is n_missing_values within limits?
		printf("n_missing_values = %d\n",n_missing_values);

		int32_t value_len=0;
		fread(&value_len,sizeof(int32_t),1,handle);
		if(ferror(handle) || feof(handle)){
			printf("lsmissing file section corrupt...\n");
			bstream_destroy(var_name);
			if(records){
				for(int i=0;i<n_records;i++)
					lsmv_destroy(records[i]);
				free(records);
			}
			return NULL;
		}
		//Increment index
		index+=sizeof(int32_t);
		printf("value_len = %d\n",value_len);

		//next we read in a stream comprised of n_missing_values of length value_len
		bstream_t *stream=bstream_new_wl(value_len*n_missing_values);
		if(!stream){
			printf("lsmissing error allocating stream memory...\n");
			bstream_destroy(var_name);
			if(records){
				for(int i=0;i<n_records;i++)
					lsmv_destroy(records[i]);
				free(records);
			}
			return NULL;
		}
		fread(stream->stream,sizeof(char),value_len*n_missing_values,handle);
		if(ferror(handle) || feof(handle)){
			printf("lsmissing error reading stream values...\n");
			bstream_destroy(var_name);
			if(records){
				for(int i=0;i<n_records;i++)
					lsmv_destroy(records[i]);
				free(records);
			}
			bstream_destroy(stream);
			return NULL;
		}
		//increment the index
		index+=(value_len*n_missing_values);

		//Split up the stream
		bstream_t **values=bstream_split_count(*stream,value_len);
		if(!values){
			printf("lsmissing error allocating value memory...\n");
			bstream_destroy(var_name);
			if(records){
				for(int i=0;i<n_records;i++)
					lsmv_destroy(records[i]);
				free(records);
			}
			bstream_destroy(stream);
			return NULL;
		}
		printf("values {\n");
		for(char i=0;i<n_missing_values;i++){
			printf("\t");
			bstream_print(*(values[i]));
			printf("\n");
		}
		printf("}\n");

		//Add space for a new record
		n_records++;
		records=(lsmv_t**)((records)?realloc(records,sizeof(lsmv_t*)*n_records):calloc(1,sizeof(lsmv_t*)));
		if(!records){
			printf("error allocating lsmissing record memory...\n");
			bstream_destroy(var_name);
			if(records){
				for(int i=0;i<n_records;i++)
					lsmv_destroy(records[i]);
				free(records);
			}
			bstream_destroy(stream);
			for(char j=0;j<n_missing_values;j++)
				if(values[j])
					bstream_destroy(values[j]);
			free(values);
			return NULL;
		}
		records[n_records-1]=lsmv_new(var_name,n_missing_values,values);
		if(!records[n_records-1]){
			printf("error allocating lsmissing record memory...\n");
			bstream_destroy(var_name);
			if(records){
				for(int i=0;i<n_records;i++)
					lsmv_destroy(records[i]);
				free(records);
			}
			bstream_destroy(stream);
			for(char j=0;j<n_missing_values;j++)
				if(values[j])
					bstream_destroy(values[j]);
			free(values);
			return NULL;
		}
		bstream_destroy(var_name);
		for(char i=0;i<n_missing_values;i++)
			bstream_destroy(values[i]);
		free(values);
	}

	lsmvr_t *ret=lsmvr_new(record_type,subtype,n_records,records);
	if(!ret){
		for(int32_t i=0;i<n_records;i++)
			lsmv_destroy(records[i]);
		free(records);
		return NULL;
	}
	printf("15\n");
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
