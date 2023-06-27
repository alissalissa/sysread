#include <stdio.h>
#include <string.h>

#include "mrset.h"
#include "bstream.h"

bool test_mcset(void){
	char test_string[]="$abcde=C 10 my mcgroup a b c";
	bstream_t *test_stream=bstream_cnew(test_string,28);
	mcset_t *test_set=mcset_snew(test_stream);
	if(!test_set) return false;
	mcset_destroy(test_set);
	bstream_destroy(test_stream);
	return true;
}

bool test_mdset(void){
	char test_string1[]="$b=D 55 0  g e f d";
	bstream_t *test_stream1=bstream_cnew(test_string1,20);
	mdset_t *test_set1=mdset_snew(test_stream1);
	if(!test_set1) return false;
	mdset_destroy(test_set1);
	bstream_destroy(test_stream1);

	char test_string2[]="$d=E 1 2 34 13 third mdgroup k l m";
	bstream_t *test_stream2=bstream_cnew(test_string2,34);
	mdset_t *test_set2=mdset_snew(test_stream2);
	if(!test_set2) return false;
	mdset_destroy(test_set2);
	bstream_destroy(test_stream2);

	return true;
}

int main(int argc,char *argv[]){
	printf("Testing mcset_t...\n");
	bool success=test_mcset();
	printf("%s\n",success?"Passed!":"failed");

	printf("\nTesting mdset_t....\n");
	printf("%s\n",(test_mdset())?"Passed!":"Failed :-(");
	return 0;
}