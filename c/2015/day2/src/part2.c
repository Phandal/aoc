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

int calculate_required_ribbon(dimensions_t *dimensions) {
  enum { false, true };

  int calculated_required_ribbon;
  int shortest_sides[3] = {dimensions->length, dimensions->height,
                           dimensions->width};

  int sorted = false;
  int temp;
  while (!sorted) {
    sorted = true;
    if (shortest_sides[0] > shortest_sides[1]) {
      sorted = false;
      temp = shortest_sides[0];
      shortest_sides[0] = shortest_sides[1];
      shortest_sides[1] = temp;
    }

    if (shortest_sides[1] > shortest_sides[2]) {
      sorted = false;
      temp = shortest_sides[1];
      shortest_sides[1] = shortest_sides[2];
      shortest_sides[2] = temp;
    }
  }

  calculated_required_ribbon =
      ((2 * shortest_sides[0]) + (2 * shortest_sides[1]) +
       (shortest_sides[0] * shortest_sides[1] * shortest_sides[2]));

  /*printf("%d\t%d\t%d = %d\n", shortest_sides[0], shortest_sides[1],*/
  /*       shortest_sides[2], calculated_required_ribbon);*/

  return calculated_required_ribbon;
}

int main(void) {
  int len;
  char line[MAX_LINE];
  unsigned long box_required_ribbon;
  unsigned long total_required_ribbon = 0;
  dimensions_t dimensions = {0};

  while ((len = aoc_get_line(line, MAX_LINE)) > 0) {
    if (parse_dimensions(&dimensions, line, len) != AOC_OK) {
      aoc_fatalf("error parsing line: %s\n", line);
    }

    if ((box_required_ribbon = calculate_required_ribbon(&dimensions)) ==
        AOC_ERR) {
      aoc_fatalf("error calculating required ribbon\n");
    }

    total_required_ribbon += box_required_ribbon;
  }

  printf("Total Required Paper: %ld\n", total_required_ribbon);

  return 0;
}
