//Defines a type to hold an entire System file in memory

#ifndef SYSFILE_H
#define SYSFILE_H

#include "sysheader.h"

typedef struct sysfile{
	sysheader_t *header;
}sysfile_t;

#endif