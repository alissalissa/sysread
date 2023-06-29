#include "mrset.h"

//One struct to rule them all
mrset_t *mrset_new(int32_t record_type,int32_t subtype,int32_t mcset_c,mcset_t **mcs,int32_t mdset_c,mdset_t **mds){
	mrset_t *ret=malloc(sizeof(mrset_t));
	if(!ret) return NULL;
	
	ret->record_type=record_type;
	ret->subtype=subtype;

	ret->mcset_c=mcset_c;
	ret->mcs=(mcset_t**)calloc(mcset_c,sizeof(mcset_t*));
	if(!ret->mcs){
		free(ret);
		return NULL;
	}
	for(int i=0;i<ret->mcset_c;i++){
		ret->mcs[i]=mcset_cnew(mcs[i]);
		if(!ret->mcs[i]){
			for(int j=0;j<i;j++)
				mdset_destroy(ret->mcs[j]);
			free(ret->mcs);
			return NULL;
		}
	}

	ret->mdset_c=mdset_c;
	ret->mds=(mdset_t**)calloc(ret->mdset_c,sizeof(mdset_t*));
	if(!ret->mds){
		for(int i=0;i<ret->mcset_c;i++)
			mcset_destroy(ret->mcs[i]);
		free(ret->mcs);
		free(ret);
		return NULL;
	}
	for(int i=0;i<ret->mdset_c;i++){
		ret->mds[i]=mdset_cnew(mds[i]);
		if(!ret->mds[i]){
			for(int j=0;j<i;j++)
				mdset_destroy(ret->mds[j]);
			free(ret->mds);
			for(int j=0;j<ret->mcset_c;j++)
				mcset_destroy(ret->mcs[j]);
			free(ret->mcs);
			return NULL;
		}
	}

	ret->constructed=true;
	return ret;
}

//Multiple Category Sets
mcset_t *mcset_new(bstream_t *set_name,int32_t record_type,bstream_t *label,int32_t count,bstream_t **variables){
	
	mcset_t *ret=(mcset_t*)malloc(sizeof(mcset_t));
	ret->set_name=bstream_cnew(set_name);
	if(!ret->set_name){
		free(ret);
		return NULL;
	}
	ret->record_type=record_type;
	ret->label=bstream_cnew(label);
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
		ret->variables[i]=bstream_cnew(variables[i]);
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

mcset_t *mcset_snew(bstream_t *haystack){
	assert(haystack);
	if(haystack->stream[0]!='$'){
		printf("Invalid stream in mcset construction...\n");
		return NULL;
	}
	
	//Get the name of the set
	bstream_t set_name=bstream_subset(*haystack,1,bstream_find(*haystack,'=')-1);
	printf("set name=");
	fwrite(set_name.stream,sizeof(char),set_name.length,stdout);
	printf("\n");
	//Verify
	if(haystack->stream[set_name.length+1]!='=' || haystack->stream[set_name.length+2]!=MCSET_FLAG){
		printf("Corrupted mcset stream...\n");
		return NULL;
	}

	//Trim the haystack
	bstream_t haystack_modable=bstream_subset(*haystack,set_name.length+4,-1);
	fwrite(haystack_modable.stream,sizeof(char),haystack_modable.length,stdout);
	printf("\n");
	//This trim gets us onto the other side of the space, right before the label length

	//Get the length of the label
	bstream_t label_length_stream=bstream_subset(haystack_modable,0,bstream_find(haystack_modable,' '));
	char *label_length_string=(char*)calloc(label_length_stream.length+1,sizeof(char));
	memcpy(label_length_string,label_length_stream.stream,label_length_stream.length);
	label_length_string[label_length_stream.length]='\0';
	int label_length=atoi(label_length_string);
	printf("label length = %d\n",label_length);

	//Adjust the stream
	haystack_modable=bstream_subset(haystack_modable,label_length_stream.length+1,-1);
	fwrite(haystack_modable.stream,sizeof(char),haystack_modable.length,stdout);
	printf("\n");

	//Get the label
	bstream_t label=bstream_subset(haystack_modable,0,label_length);
	printf("label = ");
	fwrite(label.stream,sizeof(char),label.length,stdout);
	printf("\n");

	//Adjust the stream
	haystack_modable=bstream_subset(haystack_modable,label.length+1,-1);
	//Split up the variable names
	int32_t count=bstream_count(haystack_modable,' ')+1;
	bstream_t **variables=bstream_split(haystack_modable,' ');

	//Just for testing purposes,
	//	print out the variables
	for(int i=0;i<count;i++){
		fwrite(variables[i]->stream,sizeof(char),variables[i]->length,stdout);
		printf("\n");
	}

	mcset_t *ret=mcset_new(&set_name,7,&label,count,variables);
	for(int i=0;i<count;i++)
		bstream_destroy(variables[i]);
	free(variables);

	return ret;

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

mcset_t *mcset_cnew(mcset_t *haystack){
	if(!haystack) return NULL;
	
	mcset_t *ret=(mcset_t*)malloc(sizeof(mcset_t));
	if(!ret) return NULL;
	
	ret->set_name=bstream_cnew(haystack->set_name);
	if(!ret->set_name){
		free(ret);
		return NULL;
	}

	ret->record_type=haystack->record_type;

	ret->label=bstream_cnew(haystack->label);
	if(!ret->label){
		free(ret->set_name);
		free(ret);
		return NULL;
	}

	ret->count=haystack->count;

	ret->variables=(bstream_t**)calloc(ret->count,sizeof(bstream_t*));
	if(!ret->variables){
		free(ret->label);
		free(ret->set_name);
		free(ret);
		return NULL;
	}
	for(int i=0;i<ret->count;i++){
		ret->variables[i]=bstream_cnew(haystack->variables[i]);
		if(!ret->variables[i]){
			for(int j=0;j<i;j++)
				free(ret->variables[j]);
			free(ret->label);
			free(ret->set_name);
			free(ret);
			return NULL;
		}

	}

	ret->constructed=true;
	return ret;
}

/***********************************************************/
//Multiple Dichotomy Sets
mdset_t *mdset_new(bstream_t* set_name,int32_t record_type,char flag,bstream_t *label,int32_t count,bstream_t **variables,bstream_t *counted_value){
	if(!set_name || !label || !variables || !counted_value) return NULL;
	mdset_t *ret=(mdset_t*)malloc(sizeof(mdset_t));
	if(!ret) return NULL;

	ret->set_name=bstream_cnew(set_name);
	if(!ret->set_name){
		free(ret);
		return NULL;
	}
	ret->record_type=record_type;
	ret->flag=flag;
	ret->label=bstream_cnew(label);
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
		ret->variables[i]=bstream_cnew(variables[i]);
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
	ret->counted_value=bstream_cnew(counted_value);
	ret->constructed=true;
	return ret;
}

mdset_t *mdset_snew(bstream_t *haystack){
	assert(haystack);
	if(haystack->stream[0]!='$' || haystack->length<2)
		return NULL;
	//int seek=0;
	bstream_t set_name=bstream_subset(*haystack,1,bstream_find(*haystack,'=')-1);
	printf("Set name = ");
	fwrite(set_name.stream,sizeof(char),set_name.length,stdout);
	printf("\n");
	bstream_t haystack_buffer=bstream_subset(*haystack,2+set_name.length,-1);

	bstream_t flag_stream=bstream_subset(haystack_buffer,0,bstream_find(haystack_buffer,' '));
	char flag=flag_stream.stream[0];
	printf("flag = %c\n",flag);
	if(flag==MCSET_FLAG){
		printf("You have passed an MCSET bstream_t to an mdset_t factory...");
		return NULL;
	}
	if(flag!=MDSET_VARLABELS && flag!=MDSET_COUNTEDVALUES)
		return NULL;
	
	haystack_buffer=bstream_subset(haystack_buffer,flag_stream.length+1,-1);

	//Is LABELSOURCE==VARLABEL
	short label_source=-1;
	if(flag==MDSET_COUNTEDVALUES){
		bstream_t label_source_stream=bstream_subset(haystack_buffer,0,bstream_find(haystack_buffer,' '));
		label_source=(short)btoi(label_source_stream);
		haystack_buffer=bstream_subset(haystack_buffer,label_source_stream.length+1,-1);
	}
	printf("label_source=%d\n",label_source);

	//If label_source==1||11 then we need to get a counted value length
	//	also some error checking
	if(label_source!=-1 && label_source!=1 && label_source!=11){
		printf("Corrupted label source!\n");
		return NULL;
	}
	//Is label source 1 or 11? -1 is the only other option after that error check
	int cv_length=-1;
	if(label_source>0){
		bstream_t cv_length_stream=bstream_subset(haystack_buffer,0,bstream_find(haystack_buffer,' '));
		cv_length=btoi(cv_length_stream);
		haystack_buffer=bstream_subset(haystack_buffer,cv_length_stream.length+1,-1);
	}

	bstream_t counted_value=bstream_subset(haystack_buffer,0,(cv_length>=0)?cv_length:bstream_find(haystack_buffer,' '));
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

	haystack_buffer=bstream_subset(haystack_buffer,llen+1,-1);

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

mdset_t *mdset_cnew(mdset_t *haystack){
	if(!haystack) return NULL;

	mdset_t *ret=(mdset_t*)malloc(sizeof(mdset_t));
	if(!ret) return NULL;

	ret->set_name=bstream_cnew(haystack->set_name);
	if(!ret->set_name){
		free(ret);
		return NULL;
	}

	ret->record_type=haystack->record_type;
	ret->flag=haystack->flag;

	ret->label=bstream_cnew(haystack->label);
	if(!ret->label){
		bstream_destroy(ret->set_name);
		free(ret);
		return NULL;
	}

	ret->count=haystack->count;
	
	ret->counted_value=bstream_cnew(haystack->counted_value);
	if(!ret->counted_value){
		bstream_destroy(ret->label);
		bstream_destroy(ret->set_name);
		free(ret);
		return NULL;
	}

	ret->variables=(bstream_t**)calloc(ret->count,sizeof(bstream_t*));
	if(!ret->variables){
		bstream_destroy(ret->counted_value);
		bstream_destroy(ret->set_name);
		bstream_destroy(ret->label);
		free(ret);
		return NULL;
	}
	for(int i=0;i<ret->count;i++){
		ret->variables[i]=bstream_cnew(haystack->variables[i]);
		if(!ret->variables[i]){
			for(int j=0;j<i;j++)
				bstream_destroy(ret->variables[j]);
			bstream_destroy(ret->label);
			bstream_destroy(ret->set_name);
			bstream_destroy(ret->counted_value);
			free(ret->variables);
			free(ret);
			return NULL;
		}
	}

	ret->constructed=true;
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
