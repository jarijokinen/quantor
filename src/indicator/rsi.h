#ifndef QUANTOR_INDICATOR_RSI_H
#define QUANTOR_INDICATOR_RSI_H

#include <math.h>

typedef struct RSI {
  int period;

  int have_prev;
  double prev_price;

  int warm_count;
  double warm_gain;
  double warm_loss;

  int ready;
  double avg_gain;
  double avg_loss;
} RSI;

static inline void rsi_reset(RSI *r)
{
  if (!r) return;

  r->have_prev = 0;
  r->prev_price = 0.0;
  r->warm_count = 0;
  r->warm_gain = 0.0;
  r->warm_loss = 0.0;
  r->ready = 0;
  r->avg_gain = 0.0;
  r->avg_loss = 0.0;
}

static inline int rsi_init(RSI *r, int period)
{
  if (!r || period <= 0) return 0;
  r->period = period;
  rsi_reset(r);

  return 1;
}

static inline int rsi_ready(const RSI *r)
{
  return (r && r->ready);
}

static inline double rsi_update(RSI *r, double price)
{
  if (!r || r->period <= 0) return NAN;

  if (!r->have_prev) {
    r->prev_price = price;
    r->have_prev = 1;
    return NAN;
  }

  double change = price - r->prev_price;
  r->prev_price = price;

  double gain = (change > 0.0) ? change : 0.0;
  double loss = (change < 0.0) ? -change : 0.0;

  if (!r->ready) {
    r->warm_gain += gain;
    r->warm_loss += loss;
    r->warm_count++;

    if (r->warm_count < r->period) return NAN;

    r->avg_gain = r->warm_gain / (double)r->period;
    r->avg_loss = r->warm_loss / (double)r->period;
    r->ready = 1;

    if (r->avg_loss == 0.0) return 100.0;
    double rs = r->avg_gain / r->avg_loss;
    return 100.0 - (100.0 / (1.0 + rs));
  }

  r->avg_gain = 
    (r->avg_gain * (double)(r->period - 1) + gain) / (double)r->period;
  r->avg_loss = 
    (r->avg_loss * (double)(r->period - 1) + loss) / (double)r->period;

  if (r->avg_loss == 0.0) return 100.0;
  double rs = r->avg_gain / r->avg_loss;

  return 100.0 - (100.0 / (1.0 + rs));
}

#endif
