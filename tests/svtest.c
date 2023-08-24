//Tester for variable set records
#include <stdbool.h>
#include <stdio.h>

#include "bstream.h"
#include "svset.h"
#include "writer.h"

int main(int argc,char *argv[]){
    printf("Testing svset.c......\n");
    char ts[]="label= a b c d e f";
    bstream_t *tbs=bstream_char_new(ts,18);
    svset_t *test=svset_new(tbs);
	bool svset_success=(test)?true:false;
    if(test)
        svset_destroy(test);

	bool write_success=write_svset("./svtest.sys");
	printf("%s to write test file\n",write_success?"Able":"Unable");

	FILE *handle=fopen("./svtest.sys","r");
	if(!handle){
		printf("Error opening the svtest handle for reading! :(\n");
		return 1;
	}
	svsetlist_t *test_list=svsetlist_fnew(handle);
	bool list_succcess=(test_list)?true:false;
	if(test_list)
		svsetlist_destroy(test_list);

	printf("Test %s\n",list_succcess?"Passed!":"Failed :(");

    return 0;
}