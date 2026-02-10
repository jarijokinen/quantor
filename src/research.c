#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include "research.h"

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
  log_msg(&q, "Running on_done() hook...");
  on_done(&q);

  exit(EXIT_SUCCESS);
}
