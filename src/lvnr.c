//Implementation for long variable name records
#include "lvnr.h"

lvnr_t *lvnr_new(int32_t rec_type,int32_t subtype,int32_t count,bstream_t *keys,bstream_t *values){
	if(rec_type!=7 || subtype!=13)
		return NULL;
	lvnr_t *ret=(lvnr_t*)malloc(sizeof(lvnr_t));
	if(!ret)
		return NULL;

	ret->rec_type=rec_type;
	ret->subtype=subtype;
	ret->count=count;
	
	if(ret->count>0){
		ret->keys=bstream_cnew(keys);
		ret->values=bstream_cnew(values);
		if(!ret->keys || !ret->values){
			if(ret->keys)
				bstream_destroy(ret->keys);
			if(ret->values)
				bstream_destroy(ret->values);
			free(ret);
			return NULL;
		}
	}else{
		ret->keys=NULL;
		ret->values=NULL;
	}
	ret->constructed=true;
	return ret;
}

lvnr_t *lvnr_fnew(FILE *handle){
	assert(handle);
	
	int32_t rec_type=0;
	fread(&rec_type,sizeof(int32_t),1,handle);
	if(rec_type!=7 || feof(handle) || ferror(handle)){
		printf("Long variable name record corrupted in file...\n");
		return NULL;
	}
	printf("rec_type=%d\n",rec_type);

	int32_t subtype=0;
	fread(&subtype,sizeof(int32_t),1,handle);
	if(subtype!=13 || feof(handle) || ferror(handle)){
		printf("Long variable name record corrupted in file...\n");
		return NULL;
	}
	printf("subtype=%d\n",subtype);

	int32_t n=0;
	fread(&n,sizeof(int32_t),1,handle);
	if(ferror(handle)){
		printf("Long variable name record corrupted in file...\n");
		return NULL;
	}

	bstream_t *stream=bstream_new();
	if(!stream)
		return NULL;
	stream->length=n;
	stream->stream=(char*)calloc(n,sizeof(char));
	if(!stream->stream){
		free(stream);
		return NULL;
	}
	
	int32_t n_of_pairs=bstream_count(*stream,0x09)+1;
	bstream_t *keys=(n>0)?(bstream_t*)calloc(n_of_pairs,sizeof(bstream_t)):NULL;
	bstream_t *values=(n>0)?(bstream_t*)calloc(n_of_pairs,sizeof(bstream_t)):NULL;
	if(n_of_pairs>0){
		fread(stream->stream,sizeof(char),n,handle);
		printf("Read %d key-value pairs\n",n_of_pairs);
		bstream_t **pairs=bstream_split(*stream,0x09);
		for(int i=0;i<n_of_pairs;i++){
			bstream_t **pair=bstream_split(*pairs[i],'=');
			keys[i].stream=(char*)calloc(pair[0]->length,sizeof(char));
			values[i].stream=(char*)calloc(pair[1]->length,sizeof(char));
			memcpy(keys[i].stream,pair[0]->stream,pair[0]->length);
			keys[i].length=pair[0]->length;
			memcpy(values[i].stream,pair[1]->stream,pair[1]->length);
			values[i].length=pair[1]->length;
			bstream_destroy(pair[0]);
			bstream_destroy(pair[1]);
			free(pair);
			printf("%s -> %s\n",bstream_cstr(keys[i]),bstream_cstr(values[i]));
		}
		for(int i=0;i<n_of_pairs;i++){
			bstream_destroy(pairs[i]);
			free(pairs);
		}
	}
	lvnr_t *ret=lvnr_new(rec_type,subtype,n_of_pairs,keys,values);
	for(int i=0;i<n_of_pairs;i++){
		free(keys[i].stream);
		free(values[i].stream);
	}
	free(keys);
	free(values);
	bstream_destroy(stream);
	return ret;
}

bool lvnr_destroy(lvnr_t *haystack){
	if(!haystack || !haystack->keys || !haystack->values || !haystack->constructed)
		return false;
	haystack->constructed=false;
	for(int i=0;i<haystack->count;i++){
		if(haystack->keys[i].stream){
			free(haystack->keys[i].stream);
			haystack->keys[i].length=-1;
		}
		if(haystack->values[i].stream){
			free(haystack->values[i].stream);
			haystack->values[i].length=-1;
		}
	}
	haystack->count=-1;
	return true;
}
