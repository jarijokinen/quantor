#ifndef QUANTOR_STORE_IO_H
#define QUANTOR_STORE_IO_H

#include <stdint.h>
#include <stdio.h>

void w32le(FILE *f, uint32_t v);
void w64le(FILE *f, uint64_t v);
void wi64le(FILE *f, int64_t v);
void wf32le(FILE *f, float v);
uint32_t r32le(const uint8_t *p);
uint64_t r64le(const uint8_t *p);
int64_t ri64le(const uint8_t *p);

#endif
