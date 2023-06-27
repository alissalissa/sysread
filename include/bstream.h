//type to handle a stream of bytes

#ifndef __BSTREAM_H__
#define __BSTREAM_H__

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct BStream {
    size_t length;
    char *stream;
}bstream_t;

//Factory
bstream_t *bstream_new(void);
bstream_t *bstream_cnew(char*,int32_t); //Constructs based on a string
bstream_t *bstream_copy(bstream_t*);
bool bstream_destroy(bstream_t*);

//Manipulate
bstream_t *bstream_push(bstream_t*,char);
bstream_t *bstream_append(bstream_t*,char);
char bstream_pop(bstream_t*);
bstream_t **bstream_split(bstream_t,char);
bstream_t bstream_subset(bstream_t,int,int);
int bstream_find(bstream_t,char);
int bstream_count(bstream_t,char);

//Convert
char *bstream_cstr(bstream_t);
int btoi(bstream_t);

#endif