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

enum {
  SANTA,
  ROBOT,
};

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
  coordinate_t santa_coord = {0, 0};
  coordinate_t robot_coord = {0, 0};
  coordinate_t *active_coord = &santa_coord;
  int state = SANTA;
  int c;

  /*printf("Adding { %d, %d}\n", coord.x, coord.y);*/
  linked_list_add(&list, &santa_coord, sizeof(coordinate_t));
  /*printf("List Length: %lu\n", list.length);*/

  while ((c = getchar()) != EOF) {
    if (c == '<') {
      --(active_coord->x);
    } else if (c == '^') {
      --(active_coord->y);
    } else if (c == '>') {
      ++(active_coord->x);
    } else if (c == 'v') {
      ++(active_coord->y);
    } else {
      continue;
    }

    /*printf("c = %c\n", c);*/
    /*printf("State: %s\n", state == SANTA ? "Santa" : "Robot");*/

    if (linked_list_find(&list, active_coord, (equal_fn)coordinate_equal) !=
        NULL) {
      /*printf("Found { %d, %d}\n", active_coord->x, active_coord->y);*/
    } else {
      /*printf("Adding { %d, %d}\n", active_coord->x, active_coord->y);*/
      linked_list_add(&list, active_coord, sizeof(coordinate_t));
    }

    /*printf("List Length: %lu\n", list.length);*/
    if (state == SANTA) {
      active_coord = &robot_coord;
      state = ROBOT;
    } else {
      active_coord = &santa_coord;
      state = SANTA;
    }
  }

  printf("Number of houses that have received one present: %lu\n", list.length);

  linked_list_free(&list, free);

  return 0;
}
