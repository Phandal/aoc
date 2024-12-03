#include <aoc.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_ARRAY 100

typedef enum {
  SAFE,
  UNSAFE,
} safe_e;

typedef enum {
  UNSET,
  DESC,
  ASC,
} state_e;

typedef struct {
  char *line;
  size_t line_len;
  int numbers[MAX_ARRAY];
  size_t numbers_len;
  safe_e safe;
} report_t;

int parse_line(report_t *report) {
  size_t i = 0;
  int num;

  while (i < report->line_len) {
    num = 0;
    while (i < report->line_len && isspace(report->line[i])) {
      i += 1;
    }

    while (i < report->line_len && isdigit(report->line[i])) {
      num = (num * 10) + (report->line[i] - '0');
      i++;
    }
    printf("%d\n", num);
    report->numbers[report->numbers_len++] = num;

    if (isalpha(report->line[i])) {
      return AOC_ERR;
    }
  }

  return AOC_OK;
}

int check_if_report_is_safe(report_t *report) {
  state_e state = UNSET;
  report->safe = SAFE;

  if (parse_line(report) != AOC_OK) {
    return AOC_ERR;
  }

  for (size_t i = 1; i < (report->numbers_len); ++i) {
    int prev = report->numbers[i - 1];
    int current = report->numbers[i];
    printf("%d | %d\n", prev, current);

    int distance = abs(prev - current);
    if (distance < 1 || distance > 3) {
      report->safe = UNSAFE;
      return AOC_OK;
    }

    switch (state) {
    case UNSET:
      if (prev > current) {
        state = DESC;
      } else {
        state = ASC;
      }
      break;
    case DESC:
      if (prev < current) {
        report->safe = UNSAFE;
        return AOC_OK;
      }
      break;
    case ASC:
      if (prev > current) {
        report->safe = UNSAFE;
        return AOC_OK;
      }
    }
  }

  return AOC_OK;
}

int main(void) {
  size_t len;
  char line[MAX_ARRAY] = {0};
  long total_safe_reports = 0;

  while ((len = aoc_get_line(line, MAX_ARRAY)) != 0) {
    report_t report = {
        .line = line,
        .line_len = len,
        .numbers = {0},
        .numbers_len = 0,
        .safe = SAFE,
    };

    if (check_if_report_is_safe(&report) != AOC_OK) {
      aoc_fatalf("Failed to check if report is safe");
    };

    if (report.safe == SAFE) {
      total_safe_reports += 1;
      printf("SAFE\n");
    } else {
      printf("UNSAFE\n");
    }

    putchar('\n');
  }

  printf("Total safe reports: %ld\n", total_safe_reports);

  return 0;
}
