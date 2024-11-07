#include <aoc.h>
#include <stdio.h>

int main(void) {
  int c;
  long floor = 0;
  long position = 0;

  while ((c = getchar()) != EOF) {
    ++position;
    if (c == '(') {
      ++floor;
    }
    if (c == ')') {
      --floor;
    }

    if (floor < 0) {
      break;
    }
  }

  printf("Floor: %ld\n", floor);
  printf("Position: %ld\n", position);

  return 0;
}
