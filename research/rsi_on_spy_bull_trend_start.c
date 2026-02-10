#include "indicator/rsi.h"
#include "indicator/sma.h"
#include "research.h"

static struct State {
  SMA sma200;
  RSI rsi14;
} S;

void on_init(Quantor *q)
{
  q->symbol = "SPY";
  q->start_date = "2019-01-01";
  q->end_date = "2020-12-31";

  sma_init(&S.sma200, 200);
  rsi_init(&S.rsi14, 14);

  log_msg(q, "Probability study: RSI(14) when SPY close crosses above SMA(200)");
}

void on_bar(Quantor *q, const char *symbol, const Bar *bar)
{
  (void)symbol;

  double sma200 = sma_update(&S.sma200, bar->close);
  double rsi14 = rsi_update(&S.rsi14, bar->close);

  if (!sma_ready(&S.sma200) || !rsi_ready(&S.rsi14)) return;

  log_msg(q, "SMA200: %.2f", sma200);
  log_msg(q, "RSI14: %.2f", rsi14);
}

void on_done(Quantor *q)
{
  sma_free(&S.sma200);
  log_msg(q, "Research done");
}
