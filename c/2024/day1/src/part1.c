#include <aoc.h>
#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#define LINE_MAX 100

typedef struct {
  char *line;
  int len;
  int postition;
} line_t;

int get_num(line_t *line) {
  assert(line != NULL);
  assert(line->line != NULL);

  int c;
  int result = 0;

  while (line->postition < line->len) {
    c = line->line[line->postition++];

    if (!isdigit(c)) {
      break;
    }

    result = (result * 10) + (c - '0');
  }

  return result;
}

int sort_int(void *a, void *b) {
  int *ai = a;
  int *bi = b;

  return *ai - *bi;
}

int main(void) {
  char line_buf[LINE_MAX] = {0};
  int first, second;
  int c;
  unsigned long total = 0;

  linked_list_t beginning = {0, NULL};
  linked_list_t ending = {0, NULL};

  line_t line = {
      .line = line_buf,
      .len = 0,
      .postition = 0,
  };

  while ((line.len = aoc_get_line(line_buf, LINE_MAX)) != 0) {
    line.postition = 0;

    if ((first = get_num(&line)) == AOC_ERR) {
      aoc_fatalf("Could not read first number");
    }
    linked_list_add(&beginning, &first, sizeof(int));

    // Chew the characters until the next number
    while (line.postition < line.len) {
      c = line.line[line.postition];
      if (isdigit(c)) {
        break;
      }
      ++line.postition;
    }

    if ((second = get_num(&line)) == AOC_ERR) {
      aoc_fatalf("Could not read second number");
    }
    linked_list_add(&ending, &second, sizeof(int));
  }

  if (linked_list_sort(&beginning, sort_int) != AOC_OK) {
    aoc_fatalf("Could not sort beginning linked list");
  }

  if (linked_list_sort(&ending, sort_int) != AOC_OK) {
    aoc_fatalf("Could not sort ending linked list");
  }

  /*node_t *node;*/
  /*int *d;*/
  /*for (node = beginning.node; node != NULL; node = node->next) {*/
  /*  d = node->data;*/
  /*  printf("%d\n", *d);*/
  /*}*/
  /**/
  /*putchar('\n');*/
  /**/
  /*for (node = ending.node; node != NULL; node = node->next) {*/
  /*  d = node->data;*/
  /*  printf("%d\n", *d);*/
  /*}*/

  node_t *a = beginning.node;
  node_t *b = ending.node;
  while (a != NULL && b != NULL) {
    total += abs(*(int *)a->data - *(int *)b->data);

    a = a->next;
    b = b->next;
  }

  printf("Total: %lu\n", total);

  linked_list_free(&beginning, NULL);
  linked_list_free(&ending, NULL);

  return 0;
}
