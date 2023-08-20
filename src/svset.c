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

void svset_copy(svset_t *dest,svset_t *src){
    assert(src);
    assert(src->count>=0);
    if(!dest)
        dest=(svset_t*)malloc(sizeof(svset_t));
    dest->label=src->label;
    dest->count=src->count;
    if(src->count==0){
        dest->var_names=NULL;
    }else{
        dest->var_names=(bstream_t*)calloc(dest->count,sizeof(bstream_t));
        if(!dest->var_names){
			free(dest);
			return;
		}
        for(int i=0;i<src->count;i++){
            dest->var_names[i].stream=(char*)calloc(src->var_names[i].length,sizeof(char));
            //TODO insert error handling for calloc
			memcpy(dest->var_names[i].stream,src->var_names[i].stream,src->var_names[i].length);
            dest->var_names[i].length=src->var_names[i].length;
        }
    }
    dest->constructed=true;
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
			svset_copy(ret->sets[i],sets[i]);
			//TODO Error handling for copy failure
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