#include <stdio.h>

#include "writer.h"
#include "sysheader.h"

int main(int argc,char *argv[]){
	
	printf("Testing system header....\n");
	if(!write_header("./header.sys")){
		printf("Unabel to write test file....\n");
		return 1;
	}

	FILE *handle=fopen("./header.sys","rb");
	if(!handle){
		printf("Unable to open file for reading....\n");
		return 2;
	}

	sysheader_t *test=sysheader_fnew(handle);
	fclose(handle);

	printf("%s\n",test?"Passed!":"Failed");
	if(test)
		sysheader_destroy(test);

	return 0;
}