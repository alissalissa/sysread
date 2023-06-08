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

/************************************************
 * @TODO this factory function relies on an end 0x0a byte
 * This needs to be refactored to be based on absolute stream length instead
 * Because 0x0a is a delimeter that won't be present in the passed stream
 * ***********************************************/
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
	ret->counted_value=bstream_copy(counted_value);
	ret->constructed=true;
	return ret;
}

//@TODO update to function independent of the passed flag
mdset_t *mdset_snew(bstream_t *haystack){
	assert(haystack);
	if(haystack->stream[0]!='$' || haystack->length<2)
		return NULL;
	int seek=0;
	bstream_t set_name=bstream_subset(*haystack,1,bstream_find(*haystack,'=')-1);
	printf("Set name = ");
	fwrite(set_name.stream,sizeof(char),set_name.length,stdout);
	printf("\n");
	seek=1+set_name.length;
	if(haystack->stream[seek]!='=')
		return NULL;
	seek++;
	if(seek>=haystack->length)
		return NULL;
	char flag=haystack->stream[seek];
	printf("flag = %c\n",flag);
	if(flag==MCSET_FLAG){
		printf("You have passed an MCSET bstream_t to an mdset_t factory...");
		return NULL;
	}
	if(flag!=MDSET_VARLABELS && flag!=MDSET_COUNTEDVALUES)
		return NULL;
	seek++;
	if(seek>haystack->length || haystack->stream[seek]!=' ')
		return NULL;
	seek++;
	if(seek>haystack->length)
		return NULL;
	bstream_t haystack_buffer=bstream_subset(*haystack,seek,-1);
	seek=0;
	bstream_t counted_value=bstream_subset(haystack_buffer,0,bstream_find(haystack_buffer,' '));
	printf("Counted value = ");
	fwrite(counted_value.stream,sizeof(char),counted_value.length,stdout);
	printf("\n");

	haystack_buffer=bstream_subset(haystack_buffer,counted_value.length+1,-1);
	//seek=0; This is an unnecessary line, since seek hasn't been modified since being set to zero a few lines ago
	bstream_t label_len_stream;
	label_len_stream.length=bstream_find(haystack_buffer,' ');
	if(label_len_stream.length<0) return NULL;
	label_len_stream.stream=NULL;
	if(label_len_stream.length>0)
		label_len_stream=bstream_subset(haystack_buffer,0,label_len_stream.length);
	//This sets the stream to just past the space on the other side of the length
	haystack_buffer=bstream_subset(haystack_buffer,label_len_stream.length+1,-1);

	//label length as an integer
	label_len_stream.length++;
	label_len_stream.stream=(char*)realloc(label_len_stream.stream,label_len_stream.length);
	label_len_stream.stream[label_len_stream.length-1]='\0';
	int llen=atoi(label_len_stream.stream);
	printf("label length=%d\n",llen);

	//Get the label
	bstream_t label;
	label.length=0;
	label.stream=NULL;
	printf("label=");
	if(llen>0){
		label=bstream_subset(haystack_buffer,0,llen);
		fwrite(label.stream,sizeof(char),label.length,stdout);
	}
	printf("\n");

	//haystack_buffer now contains a space-delimited stream of variable names
	int32_t count=bstream_count(haystack_buffer,' ')+1;
	bstream_t **variables=bstream_split(haystack_buffer,' ');
	if(!variables) return NULL;

	mdset_t *ret=mdset_new(&set_name,7,flag,&label,count,variables,&counted_value);
	for(int i=0;i<count;i++){
		fwrite(variables[i]->stream,sizeof(char),variables[i]->length,stdout);
		printf("\n");
		bstream_destroy(variables[i]);
	}
	free(variables);

	return ret;
}

bool mdset_destroy(mdset_t *haystack){
	if(!haystack->constructed) return false;
	if(!bstream_destroy(haystack->set_name)) return false;
	if(!bstream_destroy(haystack->label)) return false;
	if(!bstream_destroy(haystack->counted_value)) return false;
	for(int i=0;i<haystack->count;i++)
		if(!bstream_destroy(haystack->variables[i]))
			return false;
	free(haystack->variables);
	haystack->constructed=false;
	free(haystack);
	return true;
}
