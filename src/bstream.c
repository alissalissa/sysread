//implementation for bstream_t

#include "bstream.h"

bstream_t *bstream_new(void){
    bstream_t *ret=(bstream_t*)malloc(sizeof(bstream_t));
    ret->length=0;
    ret->stream=NULL;
    return ret;
}

bool bstream_destroy(bstream_t *haystack){
    if(!haystack) return false;
    free(haystack);
    return true;
}

bool bstream_push(bstream_t *haystack,char byte){
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
    return true;
}

char bstream_pop(bstream_t *haystack){
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
