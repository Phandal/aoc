/**
 * Steps to complete
 * - Create a Linked List in the AOC library
 * - Store each coordinate in the list { int x, int y}
 * - As coordinates are parsed, if a coordinate already exists in the list,
 *     skip it and move on
 * - Show the length of the list as the number of houses that have received at
 *      least one present.
 */

#include <aoc.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  int x;
  int y;
} coordinate_t;

int coordinate_equal(coordinate_t *left, coordinate_t *right) {
  if (left == NULL || right == NULL) {
    return AOC_LL_ERROR; // Error case
  }
  int x = left->x - right->x;
  int y = left->y - right->y;

  if (x == 0 && y == 0) {
    return AOC_LL_EQUAL; // Equal case
  } else {
    return AOC_LL_NOT_EQUAL; // Non-Equal case
  }
}

int main(void) {
  linked_list_t list = {
      0,
      NULL,
  };
  coordinate_t coord = {0, 0};
  int c;

  /*printf("Adding { %d, %d}\n", coord.x, coord.y);*/
  linked_list_add(&list, &coord, sizeof(coordinate_t));
  /*printf("List Length: %lu\n", list.length);*/

  while ((c = getchar()) != EOF) {
    if (c == '<') {
      --coord.x;
    } else if (c == '^') {
      --coord.y;
    } else if (c == '>') {
      ++coord.x;
    } else if (c == 'v') {
      ++coord.y;
    } else {
      continue;
    }

    /*printf("c = %c\n", c);*/

    if (linked_list_find(&list, &coord, (equal_fn)coordinate_equal) != NULL) {
      /*printf("Found { %d, %d}\n", coord.x, coord.y);*/
      continue;
    } else {
      /*printf("Adding { %d, %d}\n", coord.x, coord.y);*/
      linked_list_add(&list, &coord, sizeof(coordinate_t));
    }

    /*printf("List Length: %lu\n", list.length);*/
  }

  printf("Number of houses that have received one present: %lu\n", list.length);

  linked_list_free(&list, NULL);

  return 0;
}
