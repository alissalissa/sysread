//Implementations for a long string value labels record
#include "lsvlabel.h"

//lsvlabel_t
lsvlabel_t *lsvlabel_new(bstream_t *value,bstream_t *label){
	lsvlabel_t *ret=(lsvlabel_t*)malloc(sizeof(lsvlabel_t));
	if(!ret)
		return NULL;
	ret->constructed=false;
	ret->value=bstream_cnew(value);
	if(!ret->value){
		free(ret);
		return NULL;
	}
	ret->label=bstream_cnew(label);
	if(!ret->label){
		bstream_destroy(ret->value);
		free(ret);
		return NULL;
	}
	ret->constructed=true;
	return ret;
}

bool lsvlabel_destroy(lsvlabel_t *haystack){
	if(!haystack)
		return false;
	if(!haystack->constructed)
		return false;
	if(!bstream_destroy(haystack->value))
		return false;
	haystack->constructed=false;
	if(!bstream_destroy(haystack->label))
		return false;
	free(haystack);
	return true;
}

//lsvlabel_list_t
lsvlabel_list_t *lsvlabel_list_new(int32_t record_type,int32_t subtype,int32_t n_labels,lsvlabel_t **labels){
	if(record_type!=LSVLABEL_RECORD_TYPE || subtype!=LSVLABEL_SUBTYPE)
		return NULL;
	lsvlabel_list_t *ret=(lsvlabel_list_t*)malloc(sizeof(lsvlabel_t));
	if(!ret)
		return NULL;
	ret->constructed=false;
	ret->record_type=record_type;
	ret->subtype=subtype;
	ret->n_labels=n_labels;
	ret->labels=(lsvlabel_t**)calloc(ret->n_labels,sizeof(lsvlabel_t*));
	for(int i=0;i<ret->n_labels;i++){
		ret->labels[i]=lsvlabel_new(labels[i]->value,labels[i]->label);
		if(!ret->labels[i]){
			for(int j=0;j<=i;j++)
				lsvlabel_destroy(ret->labels[j]);
			free(ret->labels);
			free(ret);
			return NULL;
		}
	}
	ret->constructed=true;
	return ret;
}

bool lsvlabel_list_destroy(lsvlabel_list_t *haystack){
	if(!haystack)
		return false;
	if(!haystack->constructed)
		return false;
	haystack->constructed=false;
	for(int i=0;i<haystack->n_labels;i++)
		if(!lsvlabel_destroy(haystack->labels[i]))
			return false;
	free(haystack->labels);
	free(haystack);
	return true;
}
