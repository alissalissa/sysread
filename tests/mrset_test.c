#include <stdio.h>
#include <string.h>

#include "mrset.h"

bool test_mcset(void){
	char test_stream[]="$abcde=C 10 my mcgroup a b c ";
	//size_t last=strlen(test_stream)-1;
	test_stream[strlen(test_stream)-1]=(char)0x0a;
	mcset_t *test_set=mcset_snew(strlen(test_stream),test_stream);
	if(!test_set) return false;
	return true;
}

int main(int argc,char *argv[]){
	printf("Testing mcset_t...\n");
	bool success=test_mcset();
	printf("%s\n",success?"Passed":"failed");
	return 0;
}