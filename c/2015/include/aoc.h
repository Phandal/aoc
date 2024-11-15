#ifndef AOC_H
#define AOC_H

#include <stdio.h>

enum {
  AOC_ERR = -1,
  AOC_OK,
};

void aoc_fatalf(char *fmt, ...);
int aoc_get_line(char line[], size_t max);
char *aoc_hello(void);

enum {
  AOC_LL_ERROR = -1,
  AOC_LL_EQUAL,
  AOC_LL_NOT_EQUAL,
};

typedef struct node {
  void *data;
  struct node *next;
  struct node *prev;
} node_t;

typedef struct {
  size_t length;
  node_t *node;
} linked_list_t;

typedef int (*equal_fn)(void *, void *);
typedef void (*free_fn)(void *);

size_t linked_list_add(linked_list_t *list, void *data, size_t nbytes);
void *linked_list_find(linked_list_t *list, void *data, equal_fn equals_fn);
void linked_list_free(linked_list_t *list, free_fn free_fn);

#endif // AOC_H
