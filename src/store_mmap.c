#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>

#include "store.h"
#include "store_io.h"
#include "store_mmap.h"

int store_mmap_open(const char *path, StoreMmapView *out)
{
  memset(out, 0, sizeof(*out));
  out->fd = -1;

  const uint16_t x = 1;
  if (*(const uint8_t *)&x != 1) {
    fprintf(stderr, "Host not supported, little-endian host is needed\n");
  }

  enum { HEADER_SIZE = 40 };

  int fd = open(path, O_RDONLY);
  if (fd < 0) {
    perror("Failed to open file\n");
    return -1;
  }

  struct stat st;
  if (fstat(fd, &st) != 0) {
    fprintf(stderr, "fstat failed\n");
    close(fd);
    return -1;
  }

  size_t size = (size_t)st.st_size;
  if (size < HEADER_SIZE) {
    fprintf(stderr, "Header size mismatch\n");
    close(fd);
    return -1;
  }

  void *map = mmap(NULL, size, PROT_READ, MAP_PRIVATE, fd, 0);
  if (map == MAP_FAILED) {
    fprintf(stderr, "mmap failed\n");
    close(fd);
    return -1;
  }

  const uint8_t *p = (const uint8_t *)map;
  if (memcmp(p, "QNTR", 4) != 0) {
    fprintf(stderr, "Invalid magic\n");
    munmap(map, size); 
    close(fd);
    return -1; 
  }

  StoreHeader h;
  h.version      = r32le(p + 4);
  h.record_size  = r32le(p + 8);
  h.record_count = r64le(p + 12);
  h.first_ts     = ri64le(p + 20);
  h.last_ts      = ri64le(p + 28);

  if (h.version != 1) {
    fprintf(stderr, "Unsupported version\n");
    munmap(map, size);
    close(fd);
    return -1;
  }

  if (h.record_size != (uint32_t)sizeof(Bar)) {
    fprintf(stderr, "Record size mismatch\n");
    munmap(map, size);
    close(fd);
    return -1;
  }

  size_t needed = HEADER_SIZE + (size_t)h.record_count * (size_t)h.record_size;
  if (needed > size) {
    fprintf(stderr, "Record size mismatch\n");
    munmap(map, size);
    close(fd);
    return -1;
  }

  out->fd = fd;
  out->file_size = size;
  out->map = map;
  out->hdr = h;
  out->bars = (const Bar *)(p + HEADER_SIZE);

  return 0;
}

void store_mmap_close(StoreMmapView *v)
{
  if (!v) return;
  if (v->map && v->map != MAP_FAILED) munmap(v->map, v->file_size);
  if (v->fd >= 0) close(v->fd);
  memset(v, 0, sizeof(*v));
  v->fd = -1;
}
