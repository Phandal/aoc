#include <aoc.h>
#include <stdio.h>

#define MAX_LINE 100

int main(void) {
  unsigned long nice_count = 0;
  int len;
  char line[MAX_LINE] = {0};

  while ((len = aoc_get_line(line, MAX_LINE)) != 0) {
    printf("%s\n", line);
  }

  printf("Nice word count: %lu\n", nice_count);

  return 0;
}
