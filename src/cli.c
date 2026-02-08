#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cli.h"
#include "store.h"
#include "store_import.h"

static void print_usage(const char *prog)
{
  fprintf(stderr, "Usage:\n"
      "  %s import <TICKER>\n"
      "  %s check <PATH>\n",
      prog, prog);
}

static int load_env(void)
{
  FILE *fp = fopen(".env", "r");
  if (fp == NULL) {
    perror("Failed to open .env file");
    return -1;
  }

  char line[80];

  while (fgets(line, sizeof(line), fp)) {
    line[strcspn(line, "\r\n")] = 0;

    char *eq = strchr(line, '=');
    if (!eq) continue;

    *eq = '\0';
    char *key = line;
    char *val = eq + 1;

    setenv(key, val, 1);
  }

  fclose(fp);

  return 0;
}

static int run_import(int argc, char **argv)
{
  if (argc != 3) {
    fprintf(stderr, "Usage: %s import <TICKER>\n", argv[0]);
    return -1;
  }

  if (getenv("TIINGO_API_TOKEN") == 0) {
    fprintf(stderr, "TIINGO_API_TOKEN missing\n");
    return -1;
  }

  return store_import(argv[2]);
}

static int run_check(int argc, char **argv)
{
  if (argc != 3) {
    fprintf(stderr, "Usage: %s check <PATH>\n", argv[0]);
    return -1;
  }

  return store_check(argv[2]);
}

int main(int argc, char **argv)
{
  if (argc < 2) {
    print_usage(argv[0]);
    exit(EXIT_FAILURE);
  }

  if (strcmp(argv[1], "import") == 0) {
    if (load_env() != 0) {
      exit(EXIT_FAILURE);
    }

    if (run_import(argc, argv) != 0) {
      exit(EXIT_FAILURE);
    }
  }
  
  if (strcmp(argv[1], "check") == 0) {
    if (run_check(argc, argv) != 0) {
      exit(EXIT_FAILURE);
    }
  }

  exit(EXIT_SUCCESS);
}
