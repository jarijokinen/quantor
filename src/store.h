#ifndef QUANTOR_STORE_H
#define QUANTOR_STORE_H

#include <stdint.h>
#include <stdio.h>

typedef struct __attribute__((packed)) {
  int64_t ts_ns;
  float   open;
  float   high;
  float   low;
  float   close;
  int64_t volume;
} Bar;

typedef struct {
  uint32_t version;
  uint32_t record_size;
  uint64_t record_count;
  int64_t  first_ts;
  int64_t  last_ts;
} StoreHeader;

int store_write_from_csv(const char *csv, size_t len, const char *path);
int store_check(const char *path);

#endif
