#include "indicator/rsi.h"
#include "indicator/sma.h"
#include "research.h"

static struct State {
  SMA sma200;
  RSI rsi14;
} S;

void on_init(Quantor *q)
{
  sma_init(&S.sma200, 200);
  rsi_init(&S.rsi14, 14);

  log_msg(q, "Probability study: RSI(14) when SPY close crosses above SMA(200)");
}

void on_bar(Quantor *q, const char *symbol, const Bar *bar)
{
  double sma = sma_update(&S.sma200, 0);
  double rsi = rsi_update(&S.rsi14, 0);
}

void on_done(Quantor *q)
{
  sma_free(&S.sma200);
}
