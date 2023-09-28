#include <stdio.h>

#include "lvnr.h"
#include "writer.h"

int main(int argc,char *argv[]){
	char path[]="./lvnr.sys";
	printf("Testing lvnr.....\n");
	if(!write_lvnr(path)){
		printf("Error writing test system file...\n");
		return 1;
	}
	FILE *handle=fopen(path,"rb");
	if(!handle){
		printf("Unable to open system handle for reading....");
		return 1;
	}
	lvnr_t *foo=lvnr_fnew(handle);
	fclose(handle);
	printf("%s\n",(foo)?"Success!":"Failed :-(");
	if(foo)
		lvnr_destroy(foo);
	return 0;
}