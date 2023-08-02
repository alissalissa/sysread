//Functions for writing data in the service of unit tests
#ifndef WRITER_H
#define WRITER_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "disppar.h"

bool write_mrset(const char*);
bool write_epir(const char*);
bool write_disppar(const char*);

#endif