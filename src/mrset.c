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
mcset_t *mcset_new(int32_t set_name_length,char *set_name,int32_t record_type,int32_t label_length,char *label,int32_t count,int32_t *var_lengths,char **variables){
	if(!set_name || !label || !var_lengths || !variables)
		return NULL;
	
	mcset_t *ret=(mcset_t*)malloc(sizeof(mcset_t));
	ret->set_name_length=set_name_length;
	ret->set_name=(char*)calloc(ret->set_name_length,sizeof(char));
	memcpy(ret->set_name,set_name,ret->set_name_length);
	
	ret->record_type=record_type;
	ret->label_length=label_length;
	ret->label=(char*)calloc(ret->label_length,sizeof(char));
	memcpy(ret->label,label,ret->label_length);

	ret->count=count;
	ret->var_lengths=(int32_t*)calloc(ret->count,sizeof(int32_t));
	memcpy(ret->var_lengths,var_lengths,sizeof(int32_t)*ret->count);

	ret->variables=(char**)calloc(ret->count,sizeof(char*));
	for(int i=0;i<count;i++){
		ret->variables[i]=(char*)calloc(ret->var_lengths[i],sizeof(char));
		memcpy(ret->variables[i],variables[i],ret->var_lengths[i]*sizeof(char));
	}

	ret->constructed=true;
	return ret;

}

mcset_t *mcset_snew(size_t len,char *stream){
	if(len<=2) return NULL;
	if(stream[0]!='$') return NULL;

	//Read the name of the set from the stream
	int32_t set_name_length=0;
	for(set_name_length=1;set_name_length<len && stream[set_name_length]!='=';set_name_length++);
	set_name_length--;
	//unit test code
	printf("set_name_length=%d\n",set_name_length);
	if(stream[set_name_length+1]!='=') return NULL;
	char *set_name=calloc(set_name_length,sizeof(char));
	for(int i=1;i<=set_name_length;i++) set_name[i]=stream[i];

	//Verify formatting
	int seek_index=set_name_length+2;
	if(seek_index>=len){
		printf("seek_index>=len at 90\n");
		free(set_name);
		return NULL;
	}
	if(stream[seek_index]!='C'){
		printf("stream[seek_index]!=\'C\' at 95\n");
		free(set_name);
		return NULL;
	}
	++seek_index;
	if(seek_index>=len){
		printf("error at 100\n");
		free(set_name);
		return NULL;
	}
	if(stream[seek_index]!=' '){
		printf("error at 105\n");
		free(set_name);
		return NULL;
	}

	//Verify and read the label
	seek_index++;
	int buffer_length=0;
	char *label_length_char=(char*)calloc((size_t)buffer_length,sizeof(char));
	while(seek_index<len && stream[seek_index]!=' '){
		++buffer_length;
		label_length_char=(char*)realloc(label_length_char,sizeof(char)*buffer_length);
		label_length_char[buffer_length-1]=stream[seek_index];
		++seek_index;
	}
	label_length_char=(char*)realloc(label_length_char,buffer_length+1);
	label_length_char[buffer_length]='\0';
	int32_t label_length=(int32_t)atoi(label_length_char);
	free(label_length_char);
	if(seek_index>=len || stream[seek_index]!=' '){
		printf("error at 125\n");
		free(set_name);
		free(label_length_char);
		return NULL;
	}
	printf("label length = %d\n",label_length);
	char *label=calloc(label_length,sizeof(char));
	if((seek_index+label_length)>=len){
		printf("error at 133\n");
		free(set_name);
		free(label);
		return NULL;
	}
	printf("label=");
	for(int i=1;i<=label_length;i++){
		label[i]=stream[seek_index+i];
		printf("%c",label[i]);
	}
	printf("\n");
	
	//Verify formatting
	seek_index+=label_length;
	++seek_index;
	if(seek_index>=len || stream[seek_index]!=' '){
		printf("error at 144\n");
		free(set_name);
		free(label);
		return NULL;
	}

	//Seek until the end of the stream to get the length of variable name section
	int end_pos=0;
	for(end_pos=seek_index;end_pos<len && stream[end_pos]!=0x0A;end_pos++);
	printf("end_pos=%d\n",end_pos);
	if(stream[end_pos]!=0x0A){
		printf("Error in the seeking at 161\n");
		free(label);
		free(set_name);
		return NULL;
	}
	//memory allocation happens within the subset function
	char *var_label_string=subset(stream,seek_index+1,(end_pos-seek_index)-1);
	//test code
	printf("var label string = \n");
	for(int i=0;i<((end_pos-seek_index)-1);i++)
		printf("%c",var_label_string[i]);
	printf("\n");

	return (mcset_t*)0x01;

}
