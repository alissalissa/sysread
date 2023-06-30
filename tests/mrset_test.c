#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "bstream.h"
#include "mrset.h"
#include "writer.h"

bool test_mcset(void){
	char test_string[]="$abcde=C 10 my mcgroup a b c";
	bstream_t *test_stream=bstream_char_new(test_string,28);
	mcset_t *test_set=mcset_snew(test_stream);
	if(!test_set) return false;
	mcset_destroy(test_set);
	bstream_destroy(test_stream);
	return true;
}

bool test_mdset(void){
	char test_string1[]="$b=D 55 0  g e f d";
	bstream_t *test_stream1=bstream_char_new(test_string1,18);
	mdset_t *test_set1=mdset_snew(test_stream1);
	if(!test_set1) return false;
	mdset_destroy(test_set1);
	bstream_destroy(test_stream1);

	printf("\n");

	char test_string2[]="$d=E 1 2 34 13 third mdgroup k l m";
	bstream_t *test_stream2=bstream_char_new(test_string2,34);
	mdset_t *test_set2=mdset_snew(test_stream2);
	if(!test_set2) return false;
	mdset_destroy(test_set2);
	bstream_destroy(test_stream2);

	printf("\n");

	char test_string3[]="$e=E 11 6 choice 0  n o p";
	bstream_t *test_stream3=bstream_char_new(test_string3,25);
	mdset_t *test_set3=mdset_snew(test_stream3);
	if(!test_set3) return false;
	mdset_destroy(test_set3);
	bstream_destroy(test_stream3);

	printf("\n");
	return true;
}

bool test_mrset(void){
	char p[256];
	getcwd(p,256);
	strcat(p,"/test.sys");
	printf("Printing test data to %s\n",p);
	write_mrset(p);
	FILE *sys_handle=fopen(p,"r");
	mrset_t *test_set=mrset_fnew(sys_handle);
	if(!test_set) return false;
	mrset_destroy(test_set);
	return true;
}

//TODO write test for one mrset struct to rule them all
int main(int argc,char *argv[]){
	printf("Testing mcset_t...\n");
	bool success=test_mcset();
	printf("%s\n",success?"Passed!":"failed");

	printf("\nTesting mdset_t....\n");
	printf("%s\n",(test_mdset())?"Passed!":"Failed :-(");
	
	printf("Testing mrset one struct to rule them all....\n");

	return 0;
}