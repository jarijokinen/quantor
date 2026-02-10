#ifndef QUANTOR_RESEARCH_H
#define QUANTOR_RESEARCH_H

#include "store.h"

typedef struct Quantor {
  const char *symbol;
  const char *start_date;
  const char *end_date;
  uint64_t file_count;
  uint64_t bar_count;
} Quantor;

void on_init(Quantor *q);
void on_bar(Quantor *q, const char *symbol, const Bar *bar);
void on_done(Quantor *q);
void log_msg(Quantor *q, const char *fmt, ...);

#endif
