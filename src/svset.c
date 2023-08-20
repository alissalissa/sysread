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
    free(haystack->var_names);
    free(haystack);
    return true;
}