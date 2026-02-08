#include <curl/curl.h>
#include <stdlib.h>
#include <string.h>

#include "http.h"

static size_t write_cb(char *ptr, size_t size, size_t nmemb, void *userdata)
{
  size_t n = size * nmemb;
  Buffer *buf = (Buffer *)userdata;

  char *p = (char *)realloc(buf->data, buf->size + n + 1);
  if (!p) return 0;

  buf->data = p;
  memcpy(buf->data + buf->size, ptr, n);
  buf->size += n;
  buf->data[buf->size] = '\0';

  return n;
}

int http_get(CURL *curl, const char *url, Buffer *out)
{
  out->data = NULL;
  out->size = 0;

  curl_easy_setopt(curl, CURLOPT_URL, url);
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_cb);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, out);
  
  CURLcode ret = curl_easy_perform(curl);
  if (ret != CURLE_OK) {
    fprintf(stderr, "curl_easy_perform failed: %s\n", curl_easy_strerror(ret));
    return -1;
  }
    
  return 0;
}
