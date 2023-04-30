//type to handle a stream of bytes

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

typedef struct BStream {
    size_t length;
    char *stream;
}bstream_t;

//Factory
bstream_t *bstream_new(void);
bstream_t *bstream_copy(bstream_t*);
bool bstream_destroy(bstream_t*);

//Manipulate
bool bstream_push(bstream_t*,char);
char bstream_pop(bstream_t*);
int bstream_split(bstream_t**,bstream_t,char);
bstream_t bstream_subset(bstream_t,int,int);
int bstream_find(bstream_t,char);
int bstream_count(bstream_t,char);