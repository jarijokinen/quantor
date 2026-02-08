#include <stdio.h>
#include <time.h>

#include "util.h"

int last_day_of_month(int year, int month)
{
  struct tm t = {0};

  t.tm_year = year - 1900;
  t.tm_mon  = month + 1;
  t.tm_mday = 0;
  t.tm_hour = 12;
  t.tm_isdst = -1;

  mktime(&t);

  return t.tm_mday;
}

int64_t days_from_civil(int y, unsigned m, unsigned d)
{
  y -= (m <= 2);
  const int era = (y >= 0 ? y : y - 399) / 400;
  const unsigned yoe = (unsigned)(y - era * 400);
  const unsigned doy = (153 * (m + (m > 2 ? -3 : 9)) + 2) / 5 + d - 1;
  const unsigned doe = yoe * 365 + yoe / 4 - yoe / 100 + doy;
  
  return (int64_t)(era * 146097 + (int)doe - 719468);
}

int64_t parse_ts_utc_ns(const char *s)
{
  int Y, M, D, h, m, sec, offh, offm;
  char sign;

  sscanf(s, "%d-%d-%d %d:%d:%d%c%d:%d", 
      &Y, &M, &D, &h, &m, &sec, &sign, &offh, &offm);

  int64_t days = days_from_civil(Y, (unsigned)M, (unsigned)D);
  int64_t local_sec = days * 86400LL + (int64_t)h * 3600 + (int64_t)m * 60 + sec;

  int offset = offh * 3600 + offm * 60;
  if (sign == '-') offset = -offset;

  int64_t utc_sec = local_sec - offset;
  return utc_sec * 1000000000LL;
}
