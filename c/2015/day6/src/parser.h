#ifndef PARSER_H
#define PARSER_H

#include <aoc.h>

typedef enum {
  TOGGLE,
  ON,
  OFF,
} command_e;

typedef struct {
  int x;
  int y;
} coordinate_t;

typedef struct {
  command_e instruction;
  coordinate_t left;
  coordinate_t right;
} instruction_t;

char *str_command(command_e command);
void token_free(void *token);
int parse(instruction_t *instruction, linked_list_t *tokens);

#endif // PARSER_H
