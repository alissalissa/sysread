#include "vlsr.h"

vlsr_t *vlsr_new(int32_t record_type,int32_t subtype,int32_t n,bstream_t *keys,bstream_t *values){
	if(record_type!=VLSR_RECORD_TYPE || subtype!=VLSR_SUBTYPE)
		return NULL;
	vlsr_t *ret=(vlsr_t*)malloc(sizeof(vlsr_t));
	ret->record_type=record_type;
	ret->subtype=subtype;
	ret->n=n;
	if(ret->n>0){
		if(!keys || !values){
			free(ret);
			return NULL;
		}
		ret->keys=(bstream_t*)calloc(ret->n,sizeof(bstream_t));
		ret->values=(bstream_t*)calloc(ret->n,sizeof(bstream_t));
		for(int i=0;i<ret->n;i++){
			ret->keys[i].length=keys[i].length;
			ret->keys[i].stream=(char*)calloc(ret->keys[i].length,sizeof(char));
			memcpy(ret->keys[i].stream,keys[i].stream,ret->keys[i].length);
			ret->values[i].length=values[i].length;
			ret->values[i].stream=(char*)calloc(ret->values[i].length,sizeof(char));
			memcpy(ret->values[i].stream,values[i].stream,ret->values[i].length);
		}
	}
	ret->constructed=true;
	return ret;
}

//TODO add memory allocation error handling
vlsr_t *vlsr_fnew(FILE *handle){
	if(!handle)
		return NULL;
	int32_t record_type=-1;
	fread(&record_type,sizeof(int32_t),1,handle);
	if(ferror(handle) || feof(handle) || record_type!=VLSR_RECORD_TYPE){
		printf("Corrupted very long string record!\n");
		return NULL;
	}
	printf("record type = %d\n",record_type);

	int32_t subtype=-1;
	fread(&subtype,sizeof(int32_t),1,handle);
	if(ferror(handle) || feof(handle) || subtype!=VLSR_SUBTYPE){
		printf("Corrupted very long string record!\n");
		return NULL;
	}
	printf("subtype = %d\n",subtype);

	int32_t size_check=0;
	fread(&size_check,sizeof(int32_t),1,handle);
	if(ferror(handle) || feof(handle) || size_check!=sizeof(char)){
		printf("Corrupted very long string record!\n");
		return NULL;
	}
	printf("Size check passed!\n");

	int32_t byte_count=0;
	fread(&byte_count,sizeof(int32_t),1,handle);
	if(ferror(handle)){
		printf("Corrupted very long string record!\n");
		return NULL;
	}
	printf("%d bytes in the stream\n",byte_count);
	bstream_t *vlsr_stream=bstream_new_wl(byte_count);
	//FIXME for some reason the whole file isn't being read in.
	size_t read_count=fread(vlsr_stream->stream,sizeof(char),vlsr_stream->length,handle);
	printf("%d bytes read in by fread\n",read_count);
	if(ferror(handle)){
		printf("File corrupted!\n");
		return NULL;
	}
	if(feof(handle)){
		printf("End of file reached!\n");
		return NULL;
	}
	printf("Whole stream:\n\n%s\n\n",bstream_cstr(*vlsr_stream));

	bstream_t *delimiter=bstream_new_wl(2);
	delimiter->stream[0]=(char)0x00;
	delimiter->stream[1]=(char)0x09;

	int number_of_pairs=bstream_count_str(*vlsr_stream,*delimiter)+1;
	bstream_t **pairs=bstream_split_str(*vlsr_stream,*delimiter);
	bstream_t *keys=(bstream_t*)calloc(number_of_pairs,sizeof(bstream_t));
	bstream_t *values=(bstream_t*)calloc(number_of_pairs,sizeof(bstream_t));
	for(int i=0;i<number_of_pairs;i++){
		int split_point=bstream_find(*(pairs[i]),'=');
		bstream_t temp_key=bstream_subset(*(pairs[i]),0,split_point);
		bstream_t temp_value=bstream_subset(*(pairs[i]),split_point+1,-1);
		keys[i].stream=calloc(temp_key.length,sizeof(char));
		values[i].stream=calloc(temp_value.length,sizeof(char));
		keys[i].length=temp_key.length;
		values[i].length=temp_value.length;
		memcpy(keys[i].stream,temp_key.stream,temp_key.length);
		memcpy(values[i].stream,temp_value.stream,temp_value.length);
	}
	vlsr_t *ret=vlsr_new(record_type,subtype,number_of_pairs,keys,values);
	
	//Cleanup
	bstream_destroy(vlsr_stream);
	bstream_destroy(delimiter);
	for(int i=0;i<number_of_pairs;i++){
		free(keys[i].stream);
		free(values[i].stream);
		bstream_destroy(pairs[i]);
	}
	free(keys);
	free(values);
	free(pairs);
	
	return ret;
}

bool vlsr_destroy(vlsr_t *haystack){
	if(!haystack)
		return false;
	if(!haystack->constructed)
		return false;
	if(haystack->keys){
		for(int i=0;i<haystack->n;i++)
			if(haystack->keys[i].stream)
				free(haystack->keys[i].stream);
		free(haystack->keys);
	}
	if(haystack->values){
		for(int i=0;i<haystack->n;i++)
			if(haystack->values[i].stream)
				free(haystack->values[i].stream);
		free(haystack->values);
	}
	haystack->constructed=false;
	free(haystack);
	return true;
}