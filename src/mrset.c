#include "mrset.h"

/*sysmrset_t *sysmrset_new(int32_t record_type,int32_t subtype,int32_t size,int32_t count,sysmr_t *response_sets){
	sysmrset_t *ret=(sysmrset_t*)malloc(sizeof(sysmrset_t));
	ret->record_type=record_type;
	ret->subtype=subtype;
	ret->size=size;
	ret->count=count;
	
	ret->constructed=true;
	return ret;
}

sysmrset_t *sysmrset_fnew(FILE *handle){
	if(!handle) return NULL;
	
	//const values
	int32_t record_type=0;
	int32_t subtype=0;
	int32_t size=0;
	
	fread(&record_type,sizeof(int32_t),1,handle);
	if(ferror(handle) || feof(handle) || record_type!=7){
		printf("Corrupted multiple response set record...\n");
		return NULL;
	}

	fread(&subtype,sizeof(int32_t),1,handle);
	if(ferror(handle) || feof(handle) || (subtype!=7 && subtype!=19)){
		printf("Corrupted multiple response set record...\n");
		return NULL;
	}

	fread(&size,sizeof(int32_t),1,handle);
	if(ferror(handle) || feof(handle) || size!=1){
		printf("Corrupted multiple response set record...\n");
		return NULL;
	}

	//dynamic values


}*/

//Multiple Category Sets
mcset_t *mcset_new(bstream_t *set_name,int32_t record_type,bstream_t *label,int32_t count,bstream_t **variables){
	
	mcset_t *ret=(mcset_t*)malloc(sizeof(mcset_t));
	ret->set_name=bstream_copy(set_name);
	if(!ret->set_name){
		free(ret);
		return NULL;
	}
	ret->record_type=record_type;
	ret->label=bstream_copy(label);
	if(!ret->label){
		bstream_destroy(ret->set_name);
		free(ret);
		return NULL;
	}
	ret->count=count;
	ret->variables=(bstream_t**)calloc(ret->count,sizeof(bstream_t*));
	if(!ret->variables){
		bstream_destroy(ret->set_name);
		bstream_destroy(ret->label);
		free(ret);
		return NULL;
	}
	for(int i=0;i<count;i++){
		ret->variables[i]=bstream_copy(variables[i]);
		if(!ret->variables[i]){
			for(int j=0;j<i;j++)
				bstream_destroy(ret->variables[j]);
			bstream_destroy(ret->set_name);
			bstream_destroy(ret->label);
			free(ret);
			return NULL;
		}
	}

	ret->constructed=true;
	return ret;

}

mcset_t *mcset_snew(bstream_t *stream){
	if(stream->length<=2) return NULL;
	if(stream->stream[0]!='$') return NULL;

	//Read the name of the set from the stream
	int32_t set_name_length=0;
	for(set_name_length=1;set_name_length<(int32_t)stream->length && stream->stream[set_name_length]!='=';set_name_length++);
	set_name_length--;
	//unit test code
	printf("set_name_length=%d\n",set_name_length);
	if(stream->stream[set_name_length+1]!='=') return NULL;
	bstream_t set_name=bstream_subset(*stream,1,set_name_length);

	//Verify formatting
	int seek_index=set_name_length+2;
	if(seek_index>=(int)stream->length){
		printf("seek_index>=len at 91\n");
		return NULL;
	}
	if(stream->stream[seek_index]!='C'){
		printf("stream->stream[seek_index]!=\'C\' at 95\n");
		return NULL;
	}
	++seek_index;
	if(seek_index>=(int)stream->length){
		printf("error at 100\n");
		return NULL;
	}
	if(stream->stream[seek_index]!=' '){
		printf("error at 104\n");
		return NULL;
	}

	//Verify and read the label
	seek_index++;
	bstream_t *label_length_stream=bstream_new();
	while(seek_index<(int)stream->length && stream->stream[seek_index]!=' '){
		bstream_append(label_length_stream,stream->stream[seek_index]);
		++seek_index;
	}
	bstream_append(label_length_stream,'\0');
	int32_t label_length=(int32_t)atoi(label_length_stream->stream);
	bstream_destroy(label_length_stream);
	if(seek_index>=(int)stream->length || stream->stream[seek_index]!=' '){
		printf("error at 130\n");
		return NULL;
	}
	printf("label length = %d\n",label_length);
	bstream_t *label=bstream_new();
	if((seek_index+label_length)>=(int)stream->length){
		printf("error at 136\n");
		free(label);
		return NULL;
	}
	printf("label=");
	for(int i=1;i<=label_length;i++){
		bstream_append(label,stream->stream[seek_index+i]);
		printf("%c",label->stream[i]);
	}
	printf("\n");
	
	//Verify formatting
	seek_index+=label_length;
	++seek_index;
	if(seek_index>=(int)stream->length || stream->stream[seek_index]!=' '){
		printf("error at 144\n");
		free(label);
		return NULL;
	}

	//Seek until the end of the stream to get the length of variable name section
	int end_pos=0;
	for(end_pos=seek_index;end_pos<(int)stream->length && stream->stream[end_pos]!=0x0A;end_pos++);
	printf("end_pos=%d\n",end_pos);
	if(stream->stream[end_pos]!=0x0A){
		printf("Error in the seeking at 161\n");
		free(label);
		return NULL;
	}
	//memory allocation happens within the subset function
	bstream_t var_label_stream=bstream_subset(*stream,seek_index+1,(end_pos-seek_index)-1);
	//test code
	printf("var label string = \n");
	for(int i=0;i<((end_pos-seek_index)-1);i++)
		printf("%c",var_label_stream.stream[i]);
	printf("\n");
	bstream_t **labels=bstream_split(var_label_stream,' ');
	int32_t var_label_count=bstream_count(var_label_stream,' ')+1;
	
	//test code
	printf("Labels:\n");
	for(int i=0;i<var_label_count;i++){
		for(int j=0;j<(int)(labels[i]->length);j++)
			printf("%c",labels[i]->stream[j]);
		printf("\n");
	}

	return mcset_new(&set_name,7,label,var_label_count,labels);

}

bool mcset_destroy(mcset_t *haystack){
	if(!haystack->constructed) return false;
	if(!bstream_destroy(haystack->set_name)) return false;
	if(!bstream_destroy(haystack->label)) return false;
	if(haystack->count>0)
		for(int i=0;i<haystack->count;i++)
			if(!bstream_destroy(haystack->variables[i])) return false;
	free(haystack->variables);
	haystack->constructed=false;
	free(haystack);
	return true;
}

/***********************************************************/
//Multiple Dichotomy Sets
mdset_t *mdset_new(bstream_t* set_name,int32_t record_type,char flag,bstream_t *label,int32_t count,bstream_t **variables,bstream_t *counted_value){
	if(!set_name || !label || !variables || !counted_value) return NULL;
	mdset_t *ret=(mdset_t*)malloc(sizeof(mdset_t));
	if(!ret) return NULL;

	ret->set_name=bstream_copy(set_name);
	if(!ret->set_name){
		free(ret);
		return NULL;
	}
	ret->record_type=record_type;
	ret->flag=flag;
	ret->label=bstream_copy(label);
	if(!ret->label){
		free(ret->set_name);
		free(ret);
		return NULL;
	}
	ret->count=count;
	ret->variables=(bstream_t**)calloc(count,sizeof(bstream_t*));
	if(!ret->variables){
		free(ret->label);
		free(ret->set_name);
		free(ret);
		return NULL;
	}
	for(int i=0;i<count;i++){
		//Memory is allocated within bstream_copy
		ret->variables[i]=bstream_copy(variables[i]);
		if(!ret->variables[i]){
			for(int j=0;j<i;j++)
				bstream_destroy(ret->variables[i]);
			free(ret->set_name);
			free(ret->label);
			free(ret->variables);
			free(ret);
			return NULL;

		}
	}
	ret->counted_value=counted_value;
	ret->constructed=true;
	return ret;
}