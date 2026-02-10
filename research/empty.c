#include "research.h"

void on_init(Quantor *q)
{
  (void)q;
}

void on_bar(Quantor *q, const char *symbol, const Bar *bar)
{
  (void)q;
  (void)symbol;
  (void)bar;
}

void on_done(Quantor *q)
{
  (void)q;
}
