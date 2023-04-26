#include "sfutil.h"

void mass_free(int arg_num,...){
	va_list to_free;
	va_start(to_free,arg_num);
	for(int i=0;i<arg_num;i++){
		char *a=va_arg(to_free,char*);
		free(a);
	}
	va_end(to_free);
}

bool verify_date_format(char *sample){
	//char day[]={sample[0],sample[1]};
	char month[]={sample[3],sample[4],sample[5],'\0'};
	//char year[]={sample[7],sample[8]};

	if(strcmp(month,"Jan") && strcmp(month,"Feb") && strcmp(month,"Mar") && strcmp(month,"Apr") && strcmp(month,"May") && strcmp(month,"Jun") && strcmp(month,"Jul") && strcmp(month,"Aug") && strcmp(month,"Sep") && strcmp(month,"Oct") && strcmp(month,"Nov") && strcmp(month,"Dec"))
		return false;

	return true;

}

bool verify_time_format(char *sample){
	if(sample[2]!=':' || sample[5]!=':')
		return false;
	return true;
}

char *subset(char *haystack,int start,int length){
	char *ret=calloc(length,sizeof(char));
	for(int index=start;index<(start+length);index++)
		ret[index-start]=haystack[index];
	return ret;
}

int *split(char **dest,char *haystack,int haystack_length,char delimiter){
	int count=0;
	char *modable=(char*)calloc(haystack_length,sizeof(char));
	memcpy(modable,haystack,haystack_length);
	int modable_length=haystack_length;
	for(int i=0;i<modable_length;i++)
		if(modable[i]==delimiter)
			count++;
	dest=calloc(count+1,sizeof(char*));
	int *ret=(int*)calloc(count+2,sizeof(int));
	ret[0]=count+1;
	for(int i=0;i<count;i++){
		dest[i]=calloc(find_first(modable,modable_length,delimiter),sizeof(char));
		memcpy(dest[i],subset(modable,0,find_first(modable,modable_length,delimiter)),sizeof(char)*find_first(modable,modable_length,delimiter));
		char *modable_buffer=(char*)calloc(modable_length-(find_first(modable,modable_length,delimiter)-1),sizeof(char));
		memcpy(modable_buffer,subset(haystack,find_first(haystack,haystack_length,delimiter),haystack_length-find_first(haystack,haystack_length,delimiter)),haystack_length-find_first(haystack,haystack_length,delimiter));
		free(modable);
		modable_length-=find_first(modable_buffer,modable_length,delimiter);
		modable_length--;
		modable=(char*)calloc(modable_length,sizeof(char));
		memcpy(modable,modable_buffer,modable_length);
		ret[i+1]=modable_length;
		free(modable_buffer);
	}
	dest[count]=(char*)calloc(modable_length,sizeof(char));
	ret[count+1]=modable_length;
	memcpy(dest[count],modable,modable_length);
	free(modable);
	return ret;
}

int find_first(char *haystack,int haystack_length,char delimiter){
	for(int i=0;i<haystack_length;i++)
		if(haystack[i]==delimiter) return i;
	return -1;
}
