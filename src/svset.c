#include "svset.h"

//Factory for an individual set
svset_t *svset_new(bstream_t *src){
    svset_t *ret=malloc(sizeof(svset_t));
    int equals=bstream_find(*src,'=');
    ret->label=bstream_subset(*src,0,equals);
    printf("set label = ");
    fwrite(ret->label.stream,sizeof(char),ret->label.length,stdout);
    printf("\n");
    //get rid of the '=' and ' ' 
    if(src->stream[equals+1]!=' '){
        free(ret->label.stream);
        free(ret);
        return NULL;
    }
    bstream_t revised_src=bstream_subset(*src,equals+2,-1);

    //Split up the revised source and allocate some memory
    ret->count=bstream_count(revised_src,' ')+1;
    bstream_t **var_names_buffer=bstream_split(revised_src,' ');
    ret->var_names=(bstream_t*)calloc(ret->count,sizeof(bstream_t));

    //Copy over the memory out of the buffer pointer
    for(int i=0;i<ret->count;i++){
        ret->var_names[i].stream=calloc(var_names_buffer[i]->length,sizeof(char));
        memcpy(ret->var_names[i].stream,var_names_buffer[i]->stream,var_names_buffer[i]->length);
        ret->var_names[i].length=var_names_buffer[i]->length;
        fwrite(ret->var_names[i].stream,sizeof(char),ret->var_names[i].length,stdout);
        printf("\n");
    }
    for(int i=0;i<ret->count;i++)
        bstream_destroy(var_names_buffer[i]);
    free(var_names_buffer);

    ret->constructed=true;

    return ret;
}

bool svset_destroy(svset_t *haystack){
    assert(haystack);
    if(!haystack->constructed) return false;
    if(haystack->count>0){
        for(int i=0;i<haystack->count;i++){
            if(haystack->var_names[i].stream)
                free(haystack->var_names[i].stream);
        }
    }
    free(haystack->var_names);
    free(haystack);
    return true;
}

int svset_copy(svset_t *dest,svset_t *src){
    assert(src);
    assert(src->count>=0);
    assert(dest);
    dest->label=src->label;
    dest->count=src->count;
    if(src->count==0){
        dest->var_names=NULL;
    }else{
        dest->var_names=(bstream_t*)calloc(dest->count,sizeof(bstream_t));
        if(!dest->var_names){
			free(dest);
			return 1;
		}
        for(int i=0;i<src->count;i++){
            dest->var_names[i].stream=(char*)calloc(src->var_names[i].length,sizeof(char));
            if(!dest->var_names[i].stream){
				for(int j=0;j<i;j++)
					free(dest->var_names[j].stream);
				free(dest);
				return 2;
			}
			memcpy(dest->var_names[i].stream,src->var_names[i].stream,src->var_names[i].length);
            dest->var_names[i].length=src->var_names[i].length;
        }
    }
    dest->constructed=true;
	return 0;
}

//Set list stuff
svsetlist_t *svsetlist_new(int32_t rec_type,int32_t subtype,int32_t count,svset_t **sets){
	if(rec_type!=SVSET_TYPE || subtype!=SVSET_SUBTYPE) return NULL;
	svsetlist_t *ret=(svsetlist_t*)malloc(sizeof(svsetlist_t));
	ret->rec_type=rec_type;
	ret->subtype=subtype;
	ret->count=count;
	if(ret->count>0){
		ret->sets=(svset_t**)calloc(ret->count,sizeof(svset_t*));
		if(!ret->sets){
			free(ret);
			return NULL;
		}
		for(int i=0;i<ret->count;i++){
			ret->sets[i]=(svset_t*)malloc(sizeof(svset_t));
			int copy_success=svset_copy(ret->sets[i],sets[i]);
			printf("Copying %ssuccessful: %d\n",(copy_success)?"Un":"",copy_success);
			if(!ret->sets[i]){
				for(int j=0;j<i;j++)
					svset_destroy(ret->sets[j]);
				free(ret->sets);
				free(ret);
				return NULL;
			}
		}
	}else if(ret->count==0){
		ret->sets=NULL;
	}else{
		free(ret);
		return NULL;
	}
	ret->constructed=true;
	return ret;
}

svsetlist_t *svsetlist_fnew(FILE *sys_handle){
	assert(sys_handle);
	int32_t rec_type=-1;
	int32_t subtype=-1;
	fread(&rec_type,sizeof(int32_t),1,sys_handle);
	if(feof(sys_handle) || ferror(sys_handle) || rec_type!=SVSET_TYPE){
		printf("Corrupted SVSET record type...\n");
		return NULL;
	}
	printf("Record type = %d\n",rec_type);

	fread(&subtype,sizeof(int32_t),1,sys_handle);
	if(feof(sys_handle) || ferror(sys_handle) || subtype!=SVSET_SUBTYPE){
		printf("Corrupted SVSET subtype...\n");
		return NULL;
	}
	printf("Subtype = %d\n",subtype);
	
	//size check
	int32_t size_check=-1;
	fread(&size_check,sizeof(int32_t),1,sys_handle);
	if(feof(sys_handle) || ferror(sys_handle) || size_check!=SYSVSET_BYTE_SIZE){
		printf("Corrupted SVSET byte size...\n");
		return NULL;
	}
	printf("size check = %d\n",size_check);

	//How many bytes of data?
	int32_t bytes=-1;
	fread(&bytes,sizeof(int32_t),1,sys_handle);
	if(bytes<0){
		printf("Invalid number of SVSET bytes to be read...\n");
		return NULL;
	}
	printf("Number of bytes: %d\n",bytes);

	//read in the stream of bytes
	bstream_t *raw_stream=(bstream_t*)malloc(sizeof(bstream_t));
	if(!raw_stream) return NULL;
	raw_stream->length=bytes;
	raw_stream->stream=(char*)calloc(bytes,sizeof(char));
	if(!raw_stream->stream){
		free(raw_stream);
		return NULL;
	}
	fread(raw_stream->stream,sizeof(char),raw_stream->length,sys_handle);
	if(ferror(sys_handle)){
		free(raw_stream->stream);
		free(raw_stream);
		return NULL;
	}

	//Parse the stream
	int32_t count=bstream_count(*raw_stream,0x0a)+1;
	printf("Number of lines: %d\n",count);

	bstream_t **lines=bstream_split(*raw_stream,0x0a);
	svset_t **sets=(svset_t**)calloc(count,sizeof(svset_t*));
	for(int i=0;i<count;i++)
		sets[i]=svset_new(lines[i]);

	for(int i=0;i<count;i++)
		bstream_destroy(lines[i]);
	free(lines);
	bstream_destroy(raw_stream);

	svsetlist_t *ret=svsetlist_new(rec_type,subtype,count,sets);
	//if svsetlist_new() fails, this will end up returning NULL anyway
	return ret;
}

bool svsetlist_destroy(svsetlist_t *haystack){
    assert(haystack);
    if(!haystack->constructed) return false;
    if(haystack->count>0){
        for(int i=0;i<haystack->count;i++)
            svset_destroy(haystack->sets[i]);
        if(haystack->sets)
            free(haystack->sets);
    }
    free(haystack);
    return true;
}