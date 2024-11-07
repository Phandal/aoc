#ifndef AOC_H
#define AOC_H

#include <stdio.h>

enum {
  AOC_ERR = -1,
  AOC_OK,
};

void aoc_fatalf(char *fmt, ...);
int aoc_get_line(char line[], size_t max);
char *aoc_hello(void);

#endif // AOC_H
