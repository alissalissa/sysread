//Tester for variable set records
#include <stdio.h>

#include "bstream.h"
#include "svset.h"

int main(int argc,char *argv[]){
    printf("Testing svset.c......\n");
    char ts[]="label=a b c d e f";
    bstream_t *tbs=bstream_char_new(ts,17);
    svset_t *test=svset_new(tbs);
    printf("%s\n",(test)?"Passed!":"Failed :(");
    svset_destroy(test);
    return 0;
}