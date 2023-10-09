//implementation for bstream_t

#include "bstream.h"

bstream_t *bstream_new(void){
    bstream_t *ret=(bstream_t*)malloc(sizeof(bstream_t));
    ret->length=0;
    ret->stream=NULL;
    return ret;
}

bstream_t *bstream_new_wl(size_t s){
	bstream_t *ret=(bstream_t*)malloc(sizeof(bstream_t));
	ret->length=s;
	ret->stream=(char*)calloc(s,sizeof(char));
	return ret;
}

bstream_t *bstream_char_new(char *haystack,int32_t length){
	bstream_t *ret=bstream_new();
	ret->length=length;
	ret->stream=(char*)calloc(length,sizeof(char));
	memcpy(ret->stream,haystack,length);
	return ret;
}

bstream_t *bstream_cnew(bstream_t *haystack){
    bstream_t *ret=bstream_new();
    if(!ret) return NULL;
    ret->length=haystack->length;
	ret->stream=(char*)calloc(haystack->length,sizeof(char));
    memcpy(ret->stream,haystack->stream,haystack->length);
    return ret;
}

bool bstream_destroy(bstream_t *haystack){
    if(!haystack) return false;
    free(haystack);
    return true;
}

bool bstream_mass_destroy(int n,...){
	va_list args;
	va_start(args,n);
	for(int i=0;i<n;i++)
		if(!bstream_destroy(va_arg(args,bstream_t*)))
			return false;
	va_end(args);
	return true;
}

bstream_t *bstream_push(bstream_t *haystack,char byte){
    //@FIXME what if the haystack is empty?
	haystack->length++;
    char *buffer=malloc(sizeof(char)*haystack->length);
    if(!buffer) return false;
    for(int i=0;i<(haystack->length-1);i++)
        buffer[i+1]=haystack->stream[i];
    buffer[0]=byte;
    free(haystack->stream);
    haystack->stream=calloc(haystack->length,sizeof(char));
    if(!haystack->stream){
        free(buffer);
        return NULL;
    }
    memcpy(haystack->stream,buffer,haystack->length);
    return haystack;
}

bstream_t *bstream_append(bstream_t *haystack,char c){
    if(!haystack->stream)
		haystack->stream=(char*)malloc(sizeof(char));
	haystack->length++;
	haystack->stream=(char*)realloc(haystack->stream,haystack->length);
	if(!haystack->stream) return NULL;
    haystack->stream[haystack->length-1]=c;
    return haystack;
}

char bstream_pop(bstream_t *haystack){
	if(!haystack->stream || !haystack->length) return -1;
    char ret=haystack->stream[haystack->length-1];
    char *buffer=calloc(haystack->length,sizeof(char));
    if(!buffer) return -1;
    memcpy(buffer,haystack->stream,haystack->length);
    free(haystack);
    haystack->stream=(char*)calloc(haystack->length-1,sizeof(char));
    if(!haystack->stream){
        free(buffer);
        return -1;
    }
    memcpy(haystack->stream,buffer,haystack->length-1);
    haystack->length--;
    return ret;
}

bstream_t **bstream_split(bstream_t haystack,char delimiter){
	int count=bstream_count(haystack,delimiter);
	bstream_t **dest=(bstream_t**)calloc(count+1,sizeof(bstream_t*));
    bstream_t *modable=bstream_cnew(&haystack);
	for(int i=0;i<count;i++){
		bstream_t subset_buffer=bstream_subset(*modable,0,bstream_find(*modable,delimiter));
        dest[i]=bstream_cnew(&subset_buffer);
        bstream_t modable_buffer=bstream_subset(*modable,bstream_find(*modable,delimiter)+1,-1);
		bstream_destroy(modable);
        modable=bstream_cnew(&modable_buffer);
	}
	dest[count]=bstream_cnew(modable);
    return dest;
}

bstream_t bstream_subset(bstream_t haystack,int start,int length){
    bstream_t ret;
	if(length==0){
		ret.length=0;
		ret.stream=NULL;
		return ret;
	}
    ret.length=(length>=0?length:(haystack.length-start));
    ret.stream=(char*)calloc((length>=0)?length:(haystack.length-start),sizeof(char));
    for(int i=start;i<((length>=0 && length<=(haystack.length-start))?(start+length):(haystack.length));i++)
        ret.stream[i-start]=haystack.stream[i];
    return ret;
}

int bstream_find(bstream_t haystack,char delimiter){
    for(int i=0;i<haystack.length;i++)
        if(haystack.stream[i]==delimiter)
            return i;
    return -1;
}

int bstream_count(bstream_t haystack,char delimiter){
    int ret=0;
    for(int i=0;i<haystack.length;i++)
        if(haystack.stream[i]==delimiter)
            ret++;
    return ret; 
}

//Converters
char *bstream_cstr(bstream_t haystack){
	char *ret=(char*)calloc(haystack.length+1,sizeof(char));
	memcpy(ret,haystack.stream,haystack.length);
	ret[haystack.length]='\0';
	return ret;
}

int btoi(bstream_t haystack){
	char *buffer=bstream_cstr(haystack);
	int ret=atoi(buffer);
	free(buffer);
	return ret;
}

bool bstream_cmp(bstream_t first,bstream_t second){
	if(first.length!=second.length) return false;
	for(int i=0;i<first.length;i++)
		if(first.stream[i]!=second.stream[i])
			return false;
	return true;
}
