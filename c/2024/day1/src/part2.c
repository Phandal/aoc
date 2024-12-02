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

typedef struct {
  int key;
  unsigned long value;
} key_value_t;

int equal_ass(void *av, void *bv) {
  assert(av != NULL);
  assert(bv != NULL);

  key_value_t *a = av;
  key_value_t *b = bv;

  if (a->key == b->key) {
    return AOC_LL_EQUAL;
  }

  return AOC_LL_NOT_EQUAL;
}

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

int make_associative_array(linked_list_t *ass, linked_list_t *list) {
  node_t *node;
  key_value_t *asskey_value;
  key_value_t keyvalue = {0};

  for (node = list->node; node != NULL; node = node->next) {
    keyvalue.key = *((int *)node->data);
    if ((asskey_value = linked_list_find(ass, &keyvalue, equal_ass)) != NULL) {
      (asskey_value)->value++;
    } else {
      keyvalue.key = *((int *)node->data);
      keyvalue.value = 1;
      linked_list_add(ass, &keyvalue, sizeof(key_value_t));
    }
  }

  return AOC_OK;
}

int main(void) {
  char line_buf[LINE_MAX] = {0};
  int first, second;
  int c;
  unsigned long total = 0;

  linked_list_t beginning = {0, NULL};
  linked_list_t ending = {0, NULL};
  linked_list_t ending_arr = {0, NULL};

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

  if (make_associative_array(&ending_arr, &ending) != AOC_OK) {
    aoc_fatalf("could not make associative array for ending");
  }

  node_t *node;
  key_value_t kv = {0};
  key_value_t *ass_kv = {0};
  for (node = beginning.node; node != NULL; node = node->next) {
    int *num = node->data;
    kv.key = *num;

    if ((ass_kv = linked_list_find(&ending_arr, &kv, equal_ass)) != NULL) {
      total += (ass_kv->value * (*num));
    }
  }

  printf("Total: %lu\n", total);

  linked_list_free(&beginning, NULL);
  linked_list_free(&ending, NULL);
  linked_list_free(&ending_arr, NULL);

  return 0;
}
