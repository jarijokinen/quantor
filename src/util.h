#ifndef QUANTOR_UTIL_H
#define QUANTOR_UTIL_H

#include <stdint.h>

int last_day_of_month(int year, int month);
int64_t days_from_civil(int y, unsigned m, unsigned d);
int64_t parse_ts_utc_ns(const char *s);

#endif
