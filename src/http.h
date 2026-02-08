#ifndef QUANTOR_HTTP_H
#define QUANTOR_HTTP_H

#include <curl/curl.h>

typedef struct {
  char *data;
  size_t size;
} Buffer;

int http_get(CURL *curl, const char *url, Buffer *out);

#endif
