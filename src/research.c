#include <stdarg.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "research.h"
#include "store_mmap.h"
#include "util.h"

static uint64_t lower_bound_ts(const Bar *b, uint64_t n, int64_t ts)
{
  uint64_t lo = 0, hi = n;
  while (lo < hi) {
    uint64_t mid = (lo + hi) >> 1;
    if (b[mid].ts_ns < ts)
      lo = mid + 1;
    else
      hi = mid;
  }
  return lo;
}

static uint64_t upper_bound_ts(const Bar *b, uint64_t n, int64_t ts)
{
  uint64_t lo = 0, hi = n;
  while (lo < hi) {
    uint64_t mid = (lo + hi) >> 1;
    if (b[mid].ts_ns <= ts)
      lo = mid + 1;
    else
      hi = mid;
  }
  return lo;
}

static int process_symbol(Quantor *q)
{
  char path[512];

  int64_t start_ts = date_to_utc_ns(q->start_date);
  int64_t end_ts = date_to_utc_ns(q->end_date) + (86400LL * 1000000000LL - 1);
  
  int sy, sm, ey, em;
  sscanf(q->start_date, "%d-%d", &sy, &sm);
  sscanf(q->end_date, "%d-%d", &ey, &em);

  for (int y = sy; y <= ey; y++) {
    int m0 = (y == sy) ? sm : 1;
    int m1 = (y == ey) ? em : 12;

    for (int m = m0; m < m1; m++) {
      snprintf(path, sizeof(path), "data/%s-%04d-%02d.bin", q->symbol, y, m);
      StoreMmapView v;
      if (store_mmap_open(path, &v) != 0) continue;

      if (v.hdr.record_count == 0 || 
          v.hdr.last_ts < start_ts || 
          v.hdr.first_ts > end_ts) {
        store_mmap_close(&v);
        continue;
      }

      const Bar *bars = v.bars;
      uint64_t n = v.hdr.record_count;

      uint64_t i0 = lower_bound_ts(bars, n, start_ts);
      uint64_t i1 = upper_bound_ts(bars, n, end_ts);

      for (uint64_t i = i0; i < i1; i++) {
        on_bar(q, q->symbol, &bars[i]);
      }

      store_mmap_close(&v);
    }
  }

  return 0;
}

void log_msg(Quantor *q, const char *fmt, ...)
{
  (void)q;

  va_list args;
  va_start(args, fmt);
  vfprintf(stdout, fmt, args);
  fputc('\n', stdout);
  va_end(args);
}

int main(int argc, char **argv)
{
  if (argc != 1) {
    fprintf(stderr, "Usage:\n  %s\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  Quantor q;
  memset(&q, 0, sizeof(q));

  log_msg(&q, "Running on_init() hook...");
  on_init(&q);
  log_msg(&q, "Processing bars...");
  process_symbol(&q);
  log_msg(&q, "Running on_done() hook...");
  on_done(&q);

  exit(EXIT_SUCCESS);
}
