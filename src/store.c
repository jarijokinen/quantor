#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "store.h"
#include "store_io.h"
#include "store_mmap.h"
#include "util.h"

int store_write_from_csv(const char *csv, size_t len, const char *path)
{
  FILE *fp = fopen(path, "wb+");
  if (fp == NULL) {
    perror("Failed to open file");
    return -1;
  }

  enum { HEADER_SIZE = 40 };
  enum { RECORD_SIZE = 32 };

  fwrite("QNTR", 1, 4, fp);         // Magic
  w32le(fp, 1u);                    // Version
  w32le(fp, (uint32_t)RECORD_SIZE); // Record size
  w64le(fp, 0u);                    // Record count
  wi64le(fp, 0);                    // first_ts
  wi64le(fp, 0);                    // last_ts
  w32le(fp, 0u);                    // reserved/padding

  uint64_t count = 0;
  int64_t first_ts = 0;
  int64_t last_ts = 0;

  // Skip CSV header line
  size_t i = 0;
  while (i < len && csv[i] != '\n') i++;
  if (i < len) i++;

  char line[2048];

  while (i < len) {
    size_t j = i;
    while (j < len && csv[j] != '\n') j++;
    size_t L = j - i;
    if (L >= sizeof(line)) L = sizeof(line) - 1;
    memcpy(line, csv + i, L);
    line[L] = '\0';
    i = (j < len) ? (j + 1) : j;

    if (line[0] == '\0' || line[0] == '\r') continue;

    char *c0 = line;                       // ts
    char *c1 = strchr(c0, ','); *c1++ = 0; // close
    char *c2 = strchr(c1, ','); *c2++ = 0; // high
    char *c3 = strchr(c2, ','); *c3++ = 0; // low
    char *c4 = strchr(c3, ','); *c4++ = 0; // open
    char *c5 = strchr(c4, ','); *c5++ = 0; // volume
    char *cr = strchr(c5, '\r');
    if (cr) *cr = 0;

    int64_t ts = parse_ts_utc_ns(c0);
    if (count == 0) first_ts = ts;
    last_ts = ts;

    float close = strtof(c1, NULL);
    float high = strtof(c2, NULL);
    float low = strtof(c3, NULL);
    float open = strtof(c4, NULL);
    int64_t volume = strtoll(c5, NULL, 10);

    wi64le(fp, ts);
    wf32le(fp, open);
    wf32le(fp, high);
    wf32le(fp, low);
    wf32le(fp, close);
    wi64le(fp, volume);

    count++;
  }

  fseek(fp, 12, SEEK_SET);
  w64le(fp, count);
  wi64le(fp, first_ts);
  wi64le(fp, last_ts);

  fclose(fp);

  return 0;
}

int store_check(const char *path)
{
  StoreMmapView v;

  if (store_mmap_open(path, &v) != 0) {
    fprintf(stderr, "Failed to open file\n");
    return -1;
  }

  printf("File: %s\n", path);
  printf("Format version: %u\n", v.hdr.version);
  printf("Record size: %u bytes\n", v.hdr.record_size);
  printf("Record count: %" PRIu64 "\n", v.hdr.record_count);
  printf("First ts: %" PRIu64 "\n", v.hdr.first_ts);
  printf("Last ts: %" PRIu64 "\n", v.hdr.last_ts);

  if (v.hdr.record_count > 0) {
    const Bar *first = &v.bars[0];
    const Bar *last  = &v.bars[v.hdr.record_count - 1];

    printf("\nFirst bar:\n");
    printf("ts_ns  = %" PRId64 "\n", first->ts_ns);
    printf("open   = %.4f\n", first->open);
    printf("high   = %.4f\n", first->high);
    printf("low    = %.4f\n", first->low);
    printf("close  = %.4f\n", first->close);
    printf("volume = %" PRId64 "\n", first->volume);

    printf("\nLast bar:\n");
    printf("ts_ns  = %" PRId64 "\n", last->ts_ns);
    printf("open   = %.4f\n", last->open);
    printf("high   = %.4f\n", last->high);
    printf("low    = %.4f\n", last->low);
    printf("close  = %.4f\n", last->close);
    printf("volume = %" PRId64 "\n", last->volume);
  }

  store_mmap_close(&v);

  return 0;
}
