//type to handle a stream of bytes

#ifndef __BSTREAM_H__
#define __BSTREAM_H__

#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct BStream {
    size_t length;
    char *stream;
}bstream_t;

//Factory
bstream_t *bstream_new();
bstream_t *bstream_new_wl(size_t);
bstream_t *bstream_char_new(char*,int32_t); //Constructs based on a string
bstream_t *bstream_cnew(bstream_t*);
bool bstream_destroy(bstream_t*);
bool bstream_mass_destroy(int,...);

//Manipulate
bstream_t *bstream_push(bstream_t*,char);
bstream_t *bstream_append(bstream_t*,char);
char bstream_pop(bstream_t*);
bstream_t **bstream_split(bstream_t,char);
bstream_t **bstream_split_str(bstream_t,bstream_t);
bstream_t **bstream_split_count(bstream_t,int); //Splits off every n characters
bstream_t bstream_subset(bstream_t,int,int);
int bstream_find(bstream_t,char);
int bstream_find_str(bstream_t,bstream_t);
int bstream_count(bstream_t,char);
int bstream_count_str(bstream_t,bstream_t);

//Convert
char *bstream_cstr(bstream_t);
int btoi(bstream_t);

//other operations
//are the streams equal?
bool bstream_cmp(bstream_t,bstream_t);

//debug print a bstream
void bstream_print(bstream_t);

#endif