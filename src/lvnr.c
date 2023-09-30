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
		ret->keys=(bstream_t*)calloc(ret->count,sizeof(bstream_t));
		ret->values=(bstream_t*)calloc(ret->count,sizeof(bstream_t));
		for(int i=0;i<ret->count;i++){
			ret->keys[i].stream=(char*)calloc(keys[i].length,sizeof(char));
			ret->values[i].stream=(char*)calloc(values[i].length,sizeof(char));
			if(!ret->keys || !ret->values){
				if(ret->keys){
					for(int j=0;j<=i;j++)
						free(ret->keys[i].stream);
					free(ret->keys);
				}
				if(ret->values){
					for(int j=0;j<=i;j++)
						free(ret->values[i].stream);
					free(ret->values);
				}
				free(ret);
				return NULL;
			}
			memcpy(ret->keys[i].stream,keys[i].stream,keys[i].length);
			ret->keys[i].length=keys[i].length;
			memcpy(ret->values[i].stream,values[i].stream,values[i].length);
			ret->values[i].length=values[i].length;	
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
	
	bstream_t *keys=NULL;
	bstream_t *values=NULL;
	int n_of_pairs=0;
	if(n>0){
		fread(stream->stream,sizeof(char),n,handle);
		n_of_pairs=bstream_count(*stream,0x09)+1;
		keys=calloc(n_of_pairs,sizeof(bstream_t));
		values=calloc(n_of_pairs,sizeof(bstream_t));
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
		for(int i=0;i<n_of_pairs;i++)
			bstream_destroy(pairs[i]);
		free(pairs);
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
	for(int i=0;i<haystack->count;i++){ //FIXME there's a memory error / segfault somewhere in here
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
	free(haystack);
	return true;
}
