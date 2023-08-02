//Unit tests for variable display parameters
#include <stdbool.h>
#include <stdio.h>

#include "writer.h"
#include "disppar.h"

int main(int argc,char *argv[]){
	char *path="./disppar.sys";
	printf("Testing display parameters.....\n");
	if(!write_disppar(path)){
		printf("Failed to write test data....\n");
		return 1;
	}
	FILE *sys_handle=fopen(path,"r");
	if(!path){
		printf("Failed to open system file for reading....\n");
		return 2;
	}
	disppar_t *dis=disppar_fnew(sys_handle);
	bool success=(dis)?true:false;
	if(dis) disppar_destroy(dis);
	printf("%s\n",(success)?"Success!":"Failed");
	return 0;
}