#include <aoc.h>
#include <stdio.h>

#define MAX_LINE 100
#define VOWEL_COUNT 3
#define CONSECUTIVE_COUNT 2

int check_vowel_count(char *line, int len) {
  int vowel_count = 0;
  for (int i = 0; i < len; ++i) {
    char c = line[i];
    if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u') {
      ++vowel_count;
    }
  }

  return vowel_count;
}

int check_consecutive_letter_count(char *line, int len) {
  int max_consecutive_count = 0;
  int consecutive_count = 1;
  char past_char = line[0];

  for (int i = 1; i < len; ++i) {
    if (line[i] == past_char) {
      ++consecutive_count;
    } else {
      if (consecutive_count > max_consecutive_count) {
        max_consecutive_count = consecutive_count;
      }
      consecutive_count = 1;
    }
    past_char = line[i];
  }

  if (consecutive_count > max_consecutive_count) {
    max_consecutive_count = consecutive_count;
  }

  return max_consecutive_count;
}

int check_contains_invalid_string(char *line, int len) {
  char past_char = line[0];
  char current_char;

  for (int i = 1; i < len; ++i) {
    current_char = line[i];
    if (past_char == 'a' && current_char == 'b') {
      return 1;
    }
    if (past_char == 'c' && current_char == 'd') {
      return 1;
    }
    if (past_char == 'p' && current_char == 'q') {
      return 1;
    }
    if (past_char == 'x' && current_char == 'y') {
      return 1;
    }

    past_char = current_char;
  }

  return 0;
}

int main(void) {
  unsigned long nice_count = 0;
  int len;
  char line[MAX_LINE] = {0};
  int vowel_count;
  int consecutive_count;
  int contains_invalid_string;

  while ((len = aoc_get_line(line, MAX_LINE)) != 0) {
    vowel_count = check_vowel_count(line, len);
    consecutive_count = check_consecutive_letter_count(line, len);
    contains_invalid_string = check_contains_invalid_string(line, len);

    printf("%s | %-5d | %-5d | %-5d\n", line, vowel_count, consecutive_count,
           contains_invalid_string);

    if (vowel_count >= VOWEL_COUNT && consecutive_count >= CONSECUTIVE_COUNT &&
        !contains_invalid_string) {
      nice_count++;
    }
  }

  printf("Nice word count: %lu\n", nice_count);

  return 0;
}
