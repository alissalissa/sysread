#include <stdbool.h>
#include <stdio.h>

#include "vlsr.h"
#include "writer.h"

int main(int argc,char *argv[]){
    printf("Writing vlsr.sys....");
    if(!write_vlsr("./vlsr.sys")){
        printf("Writing failed :-(\n");
        return 1;
    }
    
    printf("Testing vlsr....\n");
    FILE *handle=fopen("./vlsr.sys","rb");
	if(!handle){
		printf("Failed\n");
		return 1;
	}
	vlsr_t *test=vlsr_fnew(handle);
	fclose(handle);
	if(!test){
		printf("Failed :-(\n");
		return 1;
	}
	vlsr_destroy(test);
	printf("Successful!\n");
	return 0;
}