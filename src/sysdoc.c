#include "sysdoc.h"

sysdoc_t *sysdoc_new(int32_t record_type,int32_t n_lines,bstream_t *lines){
	if(record_type!=6) return NULL;

	sysdoc_t *ret=(sysdoc_t*)malloc(sizeof(sysdoc_t));
	ret->record_type=record_type;
	ret->n_lines=n_lines;
	ret->lines=(bstream_t*)calloc(ret->n_lines,sizeof(bstream_t));
	if(!ret->lines){
		free(ret);
		return NULL;
	}

	for(int i=0;i<ret->n_lines;i++){
		//We don't use the copy factory (cnew) here because that produces a pointer
		ret->lines[i].stream=(char*)calloc(DOC_LINE_LENGTH,sizeof(char));
		if(!ret->lines[i].stream){
			for(int j=0;j<=i;j++)
				free(ret->lines[j].stream);
			free(ret->lines);
			free(ret);
			return NULL;
		}
		ret->lines[i].length=DOC_LINE_LENGTH;
		memcpy(ret->lines[i].stream,lines[i].stream,DOC_LINE_LENGTH);
	}
	ret->constructed=true;
	return ret;
}

sysdoc_t *sysdoc_fnew(FILE *handle){
	if(!handle) return NULL;
	
	int32_t record_type=-1;
	fread(&record_type,sizeof(int32_t),1,handle);
	if(ferror(handle) || feof(handle) || record_type!=6){
		printf("Document record read error / corruption...\n");
		return NULL;
	}

	int32_t n_lines=-1;
	fread(&n_lines,sizeof(int32_t),1,handle);
	if(n_lines<=0 || ferror(handle) || feof(handle)){
		printf("Document record n_lines corrupted or invalid...\n");
		return NULL;
	}

	bstream_t *lines=(bstream_t*)calloc(n_lines,sizeof(bstream_t));
	for(int i=0;i<n_lines;i++){
		lines[i].stream=(char*)calloc(DOC_LINE_LENGTH,sizeof(char));
		if(!lines[i].stream){
			for(int j=0;j<=i;j++)
				free(lines[i].stream);
			free(lines);
			return NULL;
		}
		fread(lines[i].stream,sizeof(char),DOC_LINE_LENGTH,handle);
		lines[i].length=DOC_LINE_LENGTH;
		if(ferror(handle)){
			for(int j=0;j<=i;j++){
				free(lines[j].stream);
			}
			free(lines);
			printf("Error / corruption in document record lines...\n");
			return NULL;
		}
	}

	sysdoc_t *ret=sysdoc_new(record_type,n_lines,lines);
	for(int i=0;i<n_lines;i++)
		free(lines[i].stream);
	free(lines);
	return ret;
}

bool sysdoc_destroy(sysdoc_t *haystack){
	if(!haystack || !haystack->constructed)
		return false;
	else if(!haystack->constructed) return false;
	for(int i=0;i<haystack->n_lines;i++)
		free(haystack->lines[i].stream); //we don't use destructor because that's for pointers
	free(haystack->lines);
	haystack->constructed=false;
	free(haystack);
	return true;
}
