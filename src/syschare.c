#include "syschare.h"

sysce_t *sysce_new(int32_t record_type,int32_t subtype,bstream_t *encoding){
    if(!encoding)
        return NULL;
    sysce_t *ret=(sysce_t*)malloc(sizeof(sysce_t));
    if(!ret)
        return NULL;
    ret->record_type=record_type;
    ret->subtype=subtype;
    ret->encoding=bstream_new_wl(encoding->length);
    if(!ret->encoding){
        free(ret);
        return NULL;
    }
    memcpy(ret->encoding->stream,encoding->stream,ret->encoding->length);
    ret->constructed=true;
    return ret;
}

sysce_t *sysce_fnew(FILE *handle){
    //record type, subtype, size check, count, encoding
    if(!handle)
        return NULL;

    int32_t record_type=-1;
    fread(&record_type,sizeof(int32_t),1,handle);
    if(ferror(handle) || feof(handle) || record_type!=CHARE_RECORD_TYPE){
        printf("Character encoding corrupted!\n");
        return NULL;
    }
    printf("Character encoding record type = %d\n",record_type);

    int32_t subtype=-1;
    fread(&subtype,sizeof(int32_t),1,handle);
    if(ferror(handle) || feof(handle) || subtype!=CHARE_SUBTYPE){
        printf("Character encoding corrupted!\n");
        return NULL;
    }
    printf("Character encoding subtype = %d\n",subtype);

    int32_t size_check=0;
    fread(&size_check,sizeof(int32_t),1,handle);
    if(ferror(handle) || feof(handle) || size_check!=CHARE_SIZE_CHECK){
        printf("Character encoding corrupted!\n");
        return NULL;
    }
    printf("Character encoding size check = %d\n",size_check);

    int32_t length=0;
    fread(&length,sizeof(int32_t),1,handle);
    printf("Encoding length = %d\n",length);

    bstream_t *encoding=NULL;
    if(length>0){
		encoding=bstream_new_wl(length);
		if(!encoding){
			printf("Error allocating character encoding memory....\n");
			return NULL;
		}
		if(!encoding->stream){
			printf("Error allocating character encoding stream memory....\n");
			return NULL;
		}
		fread(encoding->stream,size_check,length,handle);
		printf("Encoding: %s\n",bstream_cstr(*encoding));
    }

	sysce_t *ret=sysce_new(record_type,subtype,encoding);
	bstream_destroy(encoding);
	return ret;
}

bool sysce_destroy(sysce_t *haystack){
    if(!haystack)
        return false;
    if(!haystack->constructed)
        return false;
    if(!bstream_destroy(haystack->encoding))
        return false;
    haystack->constructed=false;
    free(haystack);
    return true;
}