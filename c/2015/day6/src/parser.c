#include <aoc.h>
#include <stdlib.h>
#include <string.h>

#include "lexer.h"
#include "parser.h"

char *str_command(command_e command) {
  switch (command) {
  case TOGGLE:
    return "toggle";
  case ON:
    return "on";
  case OFF:
    return "off";
  }

  return "invalid";
}

void token_free(void *token) {
  token_t *t = token;
  if (t->type == KEYWORD) {
    free(t->value.string);
  }
  free(t);
}

int parse(instruction_t *instruction, linked_list_t *tokens) {
  enum {
    LEFT_X,
    LEFT_Y,
    RIGHT_X,
    RIGHT_Y,
  };

  int number_state = LEFT_X;
  node_t *node;
  token_t *token;
  for (node = tokens->node; node != NULL; node = node->next) {
    token = node->data;
    switch (token->type) {
    case KEYWORD:
      if (strncmp(token->value.string, "through", TOKEN_MAX) == 0 ||
          strncmp(token->value.string, "turn", TOKEN_MAX) == 0) {
        continue;
      } else if (strncmp(token->value.string, "toggle", TOKEN_MAX) == 0) {
        instruction->instruction = TOGGLE;
      } else if (strncmp(token->value.string, "on", TOKEN_MAX) == 0) {
        instruction->instruction = ON;
      } else if (strncmp(token->value.string, "off", TOKEN_MAX) == 0) {
        instruction->instruction = OFF;
      } else {
        return AOC_ERR;
      }
      break;
    case NUMBER:
      switch (number_state) {
      case LEFT_X:
        instruction->left.x = token->value.number;
        break;
      case LEFT_Y:
        instruction->left.y = token->value.number;
        break;
      case RIGHT_X:
        instruction->right.x = token->value.number;
        break;
      case RIGHT_Y:
        instruction->right.y = token->value.number;
        break;
      default:
        return AOC_ERR;
      }
      ++number_state;
    case COMMA:
      continue;
    case INVALID:
      return AOC_ERR;
    }
  }

  return AOC_OK;
}
