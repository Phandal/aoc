#include <aoc.h>
#include <stdio.h>

int main(void) {
  int c;
  long floor = 0;

  while ((c = getchar()) != EOF) {
    if (c == '(') {
      ++floor;
    }
    if (c == ')') {
      --floor;
    }
  }

  printf("Floor: %ld\n", floor);

  return 0;
}
