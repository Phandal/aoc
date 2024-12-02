#include <aoc.h>
#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void aoc_fatalf(char *fmt, ...) {
  va_list args;

  va_start(args, fmt);
  vfprintf(stderr, fmt, args);
  va_end(args);

  exit(1);
}

int aoc_get_line(char line[], size_t max) {
  assert(line != NULL);

  int c;
  size_t i = 0;

  while (i < max - 1 && (c = getchar()) != EOF && c != '\n') {
    line[i++] = c;
  }

  line[i] = '\0';

  return i;
}

char *aoc_hello(void) { return "Hello AOC!"; }

size_t linked_list_add(linked_list_t *list, void *data, size_t nbytes) {
  assert(list != NULL);

  node_t *node;
  node_t *temp;

  temp = (node_t *)malloc(sizeof(node_t));
  if (temp == NULL) {
    return AOC_ERR;
  }

  temp->data = malloc(nbytes);
  if (temp->data == NULL) {
    free(temp);
    return AOC_ERR;
  }
  memcpy(temp->data, data, nbytes);
  temp->next = NULL;

  if (list->node == NULL) {
    temp->prev = NULL;
    list->node = temp;
  } else {
    for (node = list->node; node->next != NULL; node = node->next)
      ;

    temp->prev = node;
    node->next = temp;
  }
  return ++(list->length);
}

void *linked_list_find(linked_list_t *list, void *data, equal_fn equal_fn) {
  assert(list != NULL);

  for (node_t *node = list->node; node != NULL; node = node->next) {
    int result = equal_fn(data, node->data);
    if (result == AOC_LL_ERROR) {
      return NULL;
    }

    if (result == AOC_LL_EQUAL) {
      return node->data;
    }
  }

  return NULL;
}

void linked_list_free(linked_list_t *list, free_fn free_fn) {
  assert(list != NULL);

  if (free_fn == NULL) {
    free_fn = free;
  }

  node_t *temp = NULL;
  for (node_t *node = list->node; node != NULL; node = temp) {
    temp = node->next;
    free_fn(node->data);
    free(node);
  }
}
