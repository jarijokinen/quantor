#include <curl/curl.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "http.h"
#include "store.h"
#include "store_import.h"
#include "util.h"

int store_import(const char *ticker)
{
  Buffer res;
  char url[512];
  
  struct tm start = {0};
  struct tm end = {0};

  // Start date: 2018-01-01
  start.tm_year = 2018 - 1900;
  start.tm_mon = 1 - 1;
  start.tm_mday = 1;
  start.tm_hour = 12;
  start.tm_isdst = -1;

  // End date: 2025-12-31
  end.tm_year = 2025 - 1900;
  end.tm_mon = 12 - 1;
  end.tm_mday = 31;
  end.tm_hour = 12;
  end.tm_isdst = -1;

  time_t end_date = mktime(&end);

  if (curl_global_init(CURL_GLOBAL_DEFAULT) != CURLE_OK) {
    fprintf(stderr, "curl_global_init failed\n");
    return -1;
  }

  CURL *curl = curl_easy_init();
  if (curl == NULL) {
    fprintf(stderr, "curl_easy_init failed\n");
    curl_global_cleanup();
    return -1;
  }

  while (mktime(&start) <= end_date) {
    int year = start.tm_year + 1900;
    int month = start.tm_mon + 1;
    int last_day = last_day_of_month(year, start.tm_mon);

    snprintf(url, sizeof(url), 
        "https://api.tiingo.com/iex/%s/prices?resampleFreq=1min"
        "&startDate=%04d-%02d-01&endDate=%04d-%02d-%02d" 
        "&afterHours=false&forceFill=true&format=csv"
        "&token=%s",
        ticker, year, month, year, month, last_day, getenv("TIINGO_API_TOKEN"));
     
    if (http_get(curl, url, &res) != 0) {
      fprintf(stderr, "http_get() failed");
      free(res.data);
      return -1;
    }

    fprintf(stdout, "Saving data for %s-%04d-%02d... ", ticker, year, month);
    char datafile[512];
    snprintf(datafile, sizeof(datafile), "data/%s-%04d-%02d.bin", 
        ticker, year, month);
    store_write_from_csv(res.data, strlen(res.data), datafile);
    free(res.data);
    printf("OK\n");
    
    start.tm_mon += 1;
    mktime(&start);
  }

  curl_easy_cleanup(curl);
  curl_global_cleanup();

  return 0;
}
