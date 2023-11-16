//Functions for writing data in the service of unit tests
#ifndef WRITER_H
#define WRITER_H

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "disppar.h"
#include "svset.h"
#include "sysheader.h"
#include "bstream.h"
#include "vlsr.h"

bool write_mrset(const char*);
bool write_epir(const char*);
bool write_disppar(const char*);
bool write_svset(const char*);
bool write_lvnr(const char*);
bool write_header(const char*);
bool write_vlsr(const char*);

#endif