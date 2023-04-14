#include <stdio.h>

#include "mrset.h"

bool test_mcset(void){
	char *test_stream="$abcde=c";
	mcset_t *test_set=mcset_snew(8,test_stream);
	if(!test_set) return false;
	return true;
}

int main(int argc,char *argv[]){
	bool success=test_mcset();
	printf("Testing mcset_t....%s\n",success?"Passed":"failed");
	return 0;
}