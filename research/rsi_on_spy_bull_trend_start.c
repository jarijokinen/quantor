#include "indicator/rsi.h"
#include "indicator/sma.h"
#include "research.h"

enum { RSI_BINS = 10 };

static struct State {
  SMA sma200;
  RSI rsi14;
  int have_prev;
  double prev_close;
  double prev_sma200;
  uint64_t samples;
  double sum_rsi;
  double mean;
  uint64_t hist[RSI_BINS];
} S;

void on_init(Quantor *q)
{
  q->symbol = "SPY";
  q->start_date = "2019-01-01";
  q->end_date = "2020-12-31";

  sma_init(&S.sma200, 200);
  rsi_init(&S.rsi14, 14);
  S.have_prev = 0;
  S.prev_close = 0.0;
  S.prev_sma200 = 0.0;
  S.samples = 0;
  S.sum_rsi = 0.0;
  S.mean = 0.0;
  for (int i = 0; i < RSI_BINS; i++) S.hist[i] = 0;

  log_msg(q, "Probability study: RSI(14) when SPY close crosses above SMA(200)");
}

void on_bar(Quantor *q, const char *symbol, const Bar *bar)
{
  (void)q;
  (void)symbol;

  const double close = (double)bar->close;
  const double sma200 = sma_update(&S.sma200, close);
  const double rsi14 = rsi_update(&S.rsi14, close);

  if (!sma_ready(&S.sma200) || !rsi_ready(&S.rsi14)) return;

  if (S.have_prev && S.prev_close <= S.prev_sma200 && close > sma200) {
    const double bin_width = 100.0 / (double)RSI_BINS;
    int bin = (int)(rsi14 / bin_width);

    if (bin < 0) bin = 0;
    if (bin >= RSI_BINS) bin = RSI_BINS - 1;

    S.hist[bin]++;
    S.samples++;
    S.sum_rsi += rsi14;
  }

  S.prev_close = close;
  S.prev_sma200 = sma200;
  S.have_prev = 1;
}

void on_done(Quantor *q)
{
  log_msg(q, "Samples: %llu", (unsigned long long)S.samples);

  if (S.samples > 0) {
    int best = 0;
    const double bin_width = 100.0 / (double)RSI_BINS;

    S.mean = S.sum_rsi / (double)S.samples;

    for (int i = 1; i < RSI_BINS; i++) {
      if (S.hist[i] > S.hist[best]) best = i;
    }

    double lo = (double)best * bin_width;
    double hi = lo + bin_width;

    log_msg(q, "Mean RSI at bull-start: %.2f", S.mean);
    log_msg(q, "Most probable RSI bin: [%.1f, %.1f)  count=%llu",
            lo, hi, (unsigned long long)S.hist[best]);

    for (int i = 0; i < RSI_BINS; i++) {
      lo = (double)i * bin_width;
      hi = lo + bin_width;
      double probability = ((double)S.hist[i] * 100.0) / (double)S.samples;

      log_msg(q, "RSI [%.1f, %.1f): %.2f%% (%llu)",
              lo, hi, probability, (unsigned long long)S.hist[i]);
    }
  } else {
    log_msg(q, "Mean RSI at bull-start: n/a");
    log_msg(q, "Most probable RSI bin: n/a");
  }

  sma_free(&S.sma200);
}
