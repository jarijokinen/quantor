#ifndef QUANTOR_INDICATOR_SMA_H
#define QUANTOR_INDICATOR_SMA_H

#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct SMA {
  int period;
  int count;
  int idx;
  double sum;
  double *buf;
} SMA;

static inline void sma_reset(SMA *s)
{
  if (!s) return;

  s->count = 0;
  s->idx = 0;
  s->sum = 0.0;

  if (s->buf && s->period > 0) {
    memset(s->buf, 0, (size_t)s->period * sizeof(double));
  }
}

static inline int sma_init(SMA *s, int period)
{
  if (!s || period <= 0) return 0;

  s->period = period;
  s->buf = (double*)malloc((size_t)period * sizeof(double));
  if (!s->buf) return 0;

  sma_reset(s);

  return 1;
}

static inline void sma_free(SMA *s)
{
  if (!s) return;
  free(s->buf);
  s->buf = NULL;
  s->period = 0;
  s->count = 0;
  s->idx = 0;
  s->sum = 0.0;
}

static inline double sma_update(SMA *s, double x)
{
  if (!s || !s->buf || s->period <= 0) return NAN;

  if (s->count < s->period) {
    s->buf[s->idx] = x;
    s->sum += x;
    s->count++;
    s->idx = (s->idx + 1) % s->period;

    if (s->count < s->period) return NAN;
    return s->sum / (double)s->period;
  }

  double outgoing = s->buf[s->idx];
  s->buf[s->idx] = x;
  s->sum += x - outgoing;
  s->idx = (s->idx + 1) % s->period;

  return s->sum / (double)s->period;
}

static inline int sma_ready(const SMA *s)
{
  return (s && s->count >= s->period);
}

#endif
