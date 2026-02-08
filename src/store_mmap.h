#ifndef QUANTOR_STORE_MMAP_H
#define QUANTOR_STORE_MMAP_H

#include <stdint.h>
#include <stddef.h>

#include "store.h"

typedef struct {
  int fd;
  size_t file_size;
  void *map;
  StoreHeader hdr;
  const Bar *bars;
} StoreMmapView;

int store_mmap_open(const char *path, StoreMmapView *out);
void store_mmap_close(StoreMmapView *v);

#endif
