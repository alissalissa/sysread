#include <stdio.h>
#include <string.h>

#include "mrset.h"

bool test_mcset(void){
	char test_string[]="$abcde=C 10 my mcgroup a b c ";
	bstream_t *test_stream=bstream_cnew(test_string,29);
	//size_t last=strlen(test_stream)-1;
	test_stream->stream[28]=(char)0x0a;
	mcset_t *test_set=mcset_snew(test_stream);
	if(!test_set) return false;
	mcset_destroy(test_set);
	return true;
}

int main(int argc,char *argv[]){
	printf("Testing mcset_t...\n");
	bool success=test_mcset();
	printf("%s\n",success?"Passed":"failed");
	return 0;
}