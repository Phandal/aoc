#include <aoc.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

void aoc_fatalf(char *fmt, ...) {
  va_list args;

  va_start(args, fmt);
  vfprintf(stderr, fmt, args);
  va_end(args);

  exit(1);
}

int aoc_get_line(char line[], size_t max) {
  int c;
  size_t i = 0;

  while (i < max - 1 && (c = getchar()) != EOF && c != '\n') {
    line[i++] = c;
  }

  line[i] = '\0';

  return i;
}

char *aoc_hello(void) { return "Hello AOC!"; }
