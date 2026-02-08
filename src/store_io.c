#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "store_io.h"

void w32le(FILE *f, uint32_t v)
{
  uint8_t b[4] = {
    (uint8_t)v, (uint8_t)(v>>8), (uint8_t)(v>>16), (uint8_t)(v>>24)
  };
  fwrite(b, 1, 4, f);
}

void w64le(FILE *f, uint64_t v)
{
  uint8_t b[8] = {
    (uint8_t)v, (uint8_t)(v>>8), (uint8_t)(v>>16), (uint8_t)(v>>24),
    (uint8_t)(v>>32), (uint8_t)(v>>40), (uint8_t)(v>>48), (uint8_t)(v>>56)
  };
  fwrite(b, 1, 8, f);
}

void wi64le(FILE *f, int64_t v)
{
  w64le(f, (uint64_t)v); 
}

void wf32le(FILE *f, float v)
{
  uint32_t bits;
  memcpy(&bits, &v, 4);
  w32le(f, bits);
}

uint32_t r32le(const uint8_t *p)
{
  return (uint32_t)p[0]
      | ((uint32_t)p[1] << 8)
      | ((uint32_t)p[2] << 16)
      | ((uint32_t)p[3] << 24);
}

uint64_t r64le(const uint8_t *p)
{
  return (uint64_t)p[0]
      | ((uint64_t)p[1] << 8)
      | ((uint64_t)p[2] << 16)
      | ((uint64_t)p[3] << 24)
      | ((uint64_t)p[4] << 32)
      | ((uint64_t)p[5] << 40)
      | ((uint64_t)p[6] << 48)
      | ((uint64_t)p[7] << 56);
}

int64_t ri64le(const uint8_t *p)
{
  return (int64_t)r64le(p);
}
