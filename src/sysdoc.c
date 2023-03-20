#include "sysdoc.h"

sysdoc_t *sysdoc_new(int32_t record_type,int32_t n_lines,char **lines){
	if(record_type!=6) return NULL;
	sysdoc_t *ret=(sysdoc_t*)malloc(sizeof(sysdoc_t));
	ret->record_type=record_type;
	ret->n_lines=n_lines;
	ret->lines=(char**)calloc(ret->n_lines,sizeof(char*));
	for(int i=0;i<ret->n_lines;i++){
		ret->lines[i]=(char*)calloc(DOC_LINE_LENGTH,sizeof(char));
		memcpy(ret->lines[i],lines[i],DOC_LINE_LENGTH);
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

	char **lines=(char**)calloc(n_lines,sizeof(char*));
	for(int i=0;i<n_lines;i++){
		lines[i]=(char*)calloc(DOC_LINE_LENGTH,sizeof(char));
		fread(lines[i],sizeof(char),DOC_LINE_LENGTH,handle);
		if(ferror(handle)){
			for(int j=0;j<=i;j++){
				free(lines[j]);
			}
			free(lines);
			printf("Error / corruption in document record lines...\n");
			return NULL;
		}
	}

	sysdoc_t *ret=sysdoc_new(record_type,n_lines,lines);
	for(int i=0;i<n_lines;i++)
		free(lines[i]);
	free(lines);
	return ret;
}
