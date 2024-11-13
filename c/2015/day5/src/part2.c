#include <aoc.h>
#include <stdio.h>

#define MAX_LINE 100

typedef struct {
  char first;
  char second;
} character_pair_t;

int check_contains_two_pairs(char *line, size_t len) {
  if (len < 4) {
    return 0;
  }

  character_pair_t first_pair = {0};
  character_pair_t second_pair = {0};

  for (size_t i = 0; i < (len - 1); ++i) {
    first_pair.first = line[i];
    first_pair.second = line[i + 1];

    for (size_t j = 0; j < (len - 1); ++j) {
      /*printf("%lu | %lu\n", i, j);*/
      if (j == i || (j - 1) == i || (j + 1) == i) {
        continue;
      }

      second_pair.first = line[j];
      second_pair.second = line[j + 1];

      /*printf("%c %lu %c %lu | %c %lu %c %lu\n", first_pair.first, i,*/
      /*       first_pair.second, i + 1, second_pair.first, j,
       * second_pair.second,*/
      /*       j + 1);*/

      if (first_pair.first == second_pair.first &&
          first_pair.second == second_pair.second) {
        return 1;
      }
    }
  }

  return 0;
}

int check_contains_duplicate_letter_split_by_one(char *line, size_t len) {
  /*printf("Len: %lu\n", len);*/
  if (len < 3) {
    return 0;
  }

  char first;
  char third;

  for (size_t i = 0; i < (len - 2); ++i) {
    first = line[i];
    third = line[i + 2];

    /*printf("%c %lu | %c %lu\n", first, i, third, i + 2);*/

    if (first == third) {
      return 1;
    }
  }

  return 0;
}

int main(void) {
  unsigned long nice_count = 0;
  int len;
  char line[MAX_LINE] = {0};

  while ((len = aoc_get_line(line, MAX_LINE)) != 0) {
    int contains_two_pairs = check_contains_two_pairs(line, len);
    int contains_duplicate_letter_split_by_one =
        check_contains_duplicate_letter_split_by_one(line, len);

    printf("%s | %-5d | %-5d\n", line, contains_two_pairs,
           contains_duplicate_letter_split_by_one);

    if (contains_two_pairs && contains_duplicate_letter_split_by_one) {
      ++nice_count;
    }
  }

  printf("Nice word count: %lu\n", nice_count);

  return 0;
}
