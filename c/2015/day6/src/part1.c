#include <aoc.h>
#include <stdio.h>
#include <stdlib.h>

#include "lexer.h"
#include "parser.h"

#define LINE_MAX 100
#define LIGHTS_LEN 1000

int init_lights(int ***lights, size_t len) {
  if ((*lights = malloc(sizeof(int *) * len)) == NULL) {
    return AOC_ERR;
  }

  for (size_t i = 0; i < len; ++i) {
    if (((*lights)[i] = malloc(sizeof(int) * len)) == NULL) {
      return AOC_ERR;
    }
  }

  for (size_t i = 0; i < len; ++i) {
    for (size_t j = 0; j < len; ++j) {
      (*lights)[i][j] = 0;
    }
  }

  return AOC_OK;
}

void execute_instruction(instruction_t *instruction, int ***lights, int len) {
  for (int i = 0; i < len; ++i) {
    for (int j = 0; j < len; ++j) {
      if ((i >= instruction->left.x) && (i <= instruction->right.x) &&
          (j >= instruction->left.y) && (j <= instruction->right.y)) {
        switch (instruction->instruction) {
        case ON:
          (*lights)[i][j] = 1;
          break;
        case OFF:
          (*lights)[i][j] = 0;
          break;
        case TOGGLE:
          if ((*lights)[i][j] == 0) {
            (*lights)[i][j] = 1;
          } else {
            (*lights)[i][j] = 0;
          }
          break;
        }
      }
    }
  }
}

void print_lights(int ***lights, size_t len) {
  for (size_t i = 0; i < len; ++i) {
    for (size_t j = 0; j < len; ++j) {
      printf("%d", (*lights)[i][j]);
    }
    putchar('\n');
  }
}

int calculate_number_of_on_lights(int ***lights, size_t len) {
  int number_of_lights_on = 0;

  for (size_t i = 0; i < len; ++i) {
    for (size_t j = 0; j < len; ++j) {
      if ((*lights)[i][j] == 1) {
        ++number_of_lights_on;
      }
    }
  }

  return number_of_lights_on;
}

void free_lights(int ***lights, size_t len) {
  for (size_t i = 0; i < len; ++i) {
    free((*lights)[i]);
  }

  free(*lights);
}

int parse_line(instruction_t *instruction, char *line, size_t len) {
  linked_list_t tokens = {0, NULL};

  if (tokenize(&tokens, line, len) != AOC_OK) {
    return AOC_ERR;
  }

  /*node_t *node;*/
  /*token_t *token;*/
  /*for (node = tokens.node; node != NULL; node = node->next) {*/
  /*  token = node->data;*/
  /*  switch (token->type) {*/
  /*  case NUMBER:*/
  /*    printf("NUMBER: %d\n", token->value.number);*/
  /*    break;*/
  /*  case KEYWORD:*/
  /*    printf("KEYWORD: %s\n", token->value.string);*/
  /*    break;*/
  /**/
  /*  case COMMA:*/
  /*    printf("COMMA: %c\n", token->value.character);*/
  /*    break;*/
  /**/
  /*  case INVALID:*/
  /*    printf("INVALID\n");*/
  /*    break;*/
  /*  }*/
  /*}*/

  if (parse(instruction, &tokens) != AOC_OK) {
    return AOC_ERR;
  }

  linked_list_free(&tokens, token_free);

  return AOC_OK;
}

int main(void) {
  size_t len;
  size_t number_of_lights_on;
  char line[LINE_MAX] = {0};
  int **lights = NULL;
  instruction_t instruction = {0};

  if (init_lights(&lights, LIGHTS_LEN) != AOC_OK) {
    aoc_fatalf("failed to initialize the lights matrix\n");
  }

  while ((len = aoc_get_line(line, LINE_MAX)) != 0) {
    if (parse_line(&instruction, line, len) != AOC_OK) {
      aoc_fatalf("failed to parse line: %s\n", line);
    }
    printf("%s | %s: (%d, %d) (%d, %d)\n", line,
           str_command(instruction.instruction), instruction.left.x,
           instruction.left.y, instruction.right.x, instruction.right.y);

    execute_instruction(&instruction, &lights, LIGHTS_LEN);
  }

  /*print_lights(&lights, LIGHTS_LEN);*/

  if ((number_of_lights_on =
           calculate_number_of_on_lights(&lights, LIGHTS_LEN)) == AOC_ERR) {
    aoc_fatalf("failed to calculate the number of lights turned on\n");
  }

  printf("Numer of turned on lights: %lu\n", number_of_lights_on);

  free_lights(&lights, LIGHTS_LEN);

  return 0;
}
