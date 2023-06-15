#include <stdio.h>
#include <string.h>

#include "mrset.h"

bool test_mcset(void){
	char test_string[]="$abcde=C 10 my mcgroup a b c";
	bstream_t *test_stream=bstream_cnew(test_string,28);
	mcset_t *test_set=mcset_snew(test_stream);
	if(!test_set) return false;
	mcset_destroy(test_set);
	return true;
}

bool test_mdset(void){
	char test_string[]="$b=D 55 0  g e f d";
	bstream_t *test_stream=bstream_cnew(test_string,20);
	mdset_t *test_set=mdset_snew(test_stream);
	if(!test_set) return false;
	mdset_destroy(test_set);
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