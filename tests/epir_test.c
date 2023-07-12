//Implement unit test for sysepir_t
#include "writer.h"
#include "epir.h"

int main(int argc,char *argv[]){
	const char path[]="./epir.sys";
	printf("Testing sysepir.....");
	write_epir(path);
	FILE *epir_handle=fopen(path,"r");
	sysepir_t *test_epir=sysepir_fnew(epir_handle);
	fclose(epir_handle);
	printf("%s\n",(test_epir)?"Passed!":"Failed");
	if(test_epir) sysepir_destroy(test_epir);
	return 0;
}