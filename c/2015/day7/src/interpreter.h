#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <aoc.h>

#define INTERPRETER_ERROR_SIZE 1000

typedef struct {
  linked_list_t *instructions;
  char *error;
  node_t *current;
  size_t index;
} interpreter_t;

typedef struct {
  char *name;
  unsigned int signal;
} wire_t;

/**
 * Initialize a new interpreter_t with linked_list_t `instuctions` as input.
 */
interpreter_t *interpreter_init(linked_list_t *instructions);

/**
 * Executes a list of instructions from the `interpreter` into a list of wire_t
 * `wires`.
 */
int interpret(linked_list_t *wires, interpreter_t *interpreter);

/**
 * Destroys an interpreter_t object.
 * @note: It is the programmers responsibility to free the internal
 * `instructions` linked_list_t.
 */
void interpreter_destroy(interpreter_t *interpreter);

/**
 * Destroys a wire_t object.
 */
void wire_free(void *vwire);

#endif // INTERPRETER_H
