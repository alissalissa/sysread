//
// Created by alissa on 8/2/24.
//Extended Case Record -- Allows number of cases to be expressed in 64 bits instead of 32
//

#ifndef SYSREAD_ENCR_H
#define SYSREAD_ENCR_H

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#define ENCR_SIZE_CHECK 8
#define ENCR_DATA_COUNT 2

typedef struct ExtendedCaseRecord {
	int32_t record_type;
	int32_t subtype;
	int64_t ncases;
	bool constructed;
}encr_t;

encr_t *encr_new(int32_t,int32_t,int64_t);
encr_t *encr_fnew(FILE*);
bool encr_destroy(encr_t*);

#endif //SYSREAD_ENCR_H
