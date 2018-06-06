#ifndef UTIL_H
#define UTIL_H

#include "gf.h"
#include <stdint.h>

void store2(unsigned char *, gf);
uint16_t load2(const unsigned char *);

void store8(unsigned char *, uint64_t );
uint64_t load8(const unsigned char *);

gf bitrev(gf);

#endif

