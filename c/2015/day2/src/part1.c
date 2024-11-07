#include <aoc.h>
#include <stdio.h>

#define MAX_LINE 100

typedef struct {
  int length;
  int width;
  int height;
} dimensions_t;

enum {
  LENGTH,
  WIDTH,
  HEIGHT,
};

int set_dimension_value(dimensions_t *dimensions, int value, int state) {
  switch (state) {
  case LENGTH:
    dimensions->length = value;
    break;
  case WIDTH:
    dimensions->width = value;
    break;
  case HEIGHT:
    dimensions->height = value;
    break;
  default:
    return AOC_ERR;
  }

  return AOC_OK;
}

int parse_dimensions(dimensions_t *dimensions, char line[], int len) {
  int state = LENGTH;
  int value = 0;

  for (int i = 0; i < len; ++i) {
    if (line[i] == 'x') {
      if (set_dimension_value(dimensions, value, state) != AOC_OK) {
        return AOC_ERR;
      }
      state = state + 1;
      value = 0;
    } else {
      value = value * 10 + (line[i] - '0');
    }
  }

  if (state == HEIGHT) {
    if (set_dimension_value(dimensions, value, state) != AOC_OK) {
      return AOC_ERR;
    }
  }

  return AOC_OK;
}

int calculate_required_wrapping_paper(dimensions_t *dimensions) {
  unsigned long total_paper = 0;
  unsigned long shortest_side;
  unsigned long side_amount;

  // First Calc
  side_amount = (dimensions->length * dimensions->width);
  shortest_side = side_amount;
  total_paper += (side_amount * 2);

  // Second Calc
  side_amount = (dimensions->width * dimensions->height);
  if (side_amount < shortest_side) {
    shortest_side = side_amount;
  }
  total_paper += (side_amount * 2);

  // Third Calc
  side_amount = (dimensions->height * dimensions->length);
  if (side_amount < shortest_side) {
    shortest_side = side_amount;
  }
  total_paper += (side_amount * 2);

  // Add shortest side
  total_paper += shortest_side;

  return total_paper;
}

int main(void) {
  int len;
  char line[MAX_LINE];
  unsigned long box_required_paper;
  unsigned long total_required_paper = 0;
  dimensions_t dimensions = {0};

  while ((len = aoc_get_line(line, MAX_LINE)) > 0) {
    box_required_paper = 0;
    if (parse_dimensions(&dimensions, line, len) != AOC_OK) {
      aoc_fatalf("error parsing line: %s\n", line);
    }

    if ((box_required_paper = calculate_required_wrapping_paper(&dimensions)) ==
        AOC_ERR) {
      aoc_fatalf("error calculating required paper\n");
    }

    total_required_paper += box_required_paper;
  }

  printf("Total Required Paper: %ld\n", total_required_paper);

  return 0;
}
