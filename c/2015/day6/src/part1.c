#include <aoc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#define LINE_MAX 100
#define TOKEN_MAX 30

/**
 * TODO
 * 1. lexer for input files
 * 2. parser to get instructions
 * 3. coordinate_t struct to hold the x,y of that the instructs refer to
 * 4. instruction_t struct to hold the coordinates and the instruction to
 * perform
 */

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

typedef enum {
  INVALID,
  KEYWORD,
  NUMBER,
  COMMA,
} token_type_e;

typedef struct {
  token_type_e type;
  union {
    char *string;
    char character;
    int number;
  } value;
} token_t;

void token_free(void *token) {
  token_t *t = token;
  if (t->type == KEYWORD) {
    free(t->value.string);
  }
  free(t);
}

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

int tokenize(linked_list_t *tokens, char *line, size_t len) {
  char c;

  int token_string_index = 0;
  char token_string[TOKEN_MAX] = {0};

  token_t token = {0};

  for (size_t i = 0; i < len; ++i) {
    c = line[i];
    switch (c) {
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
      if (token.type == INVALID) {
        token.type = NUMBER;
        token.value.number = c - '0';
      } else if (token.type == NUMBER) {
        token.value.number = (token.value.number * 10) + (c - '0');
      } else {
        if (token_string_index < TOKEN_MAX - 1) {
          token_string[token_string_index++] = c;
        } else {
          return AOC_ERR;
        }
      }
      break;
    case ' ':
    case '\n':
    case ',':
      if (token.type == INVALID) {
        return AOC_ERR;
      } else if (token.type == KEYWORD) {
        token_string[token_string_index++] = '\0';
        token.value.string = malloc(sizeof(char) * token_string_index);
        memcpy(token.value.string, token_string, token_string_index);
      }

      linked_list_add(tokens, &token, sizeof(token));

      if (c == ',') {
        token.type = COMMA;
        token.value.character = c;
        linked_list_add(tokens, &token, sizeof(token));
      }

      token.type = INVALID;
      token_string_index = 0;
      bzero(token_string, TOKEN_MAX);
      break;
    default:
      if (token.type != INVALID && token.type != KEYWORD) {
        return AOC_ERR;
      }

      token.type = KEYWORD;
      if (token_string_index < TOKEN_MAX - 1) {
        token_string[token_string_index++] = c;
      } else {
        return AOC_ERR;
      }
      break;
    }
  }

  if (token.type != INVALID) {
    if (token.type == KEYWORD) {
      token_string[token_string_index++] = '\0';
      token.value.string = malloc(sizeof(char) * token_string_index);
      memcpy(token.value.string, token_string, token_string_index);
    }

    linked_list_add(tokens, &token, sizeof(token));
  }

  return AOC_OK;
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
  char line[LINE_MAX] = {0};
  instruction_t instruction = {0};

  while ((len = aoc_get_line(line, LINE_MAX)) != 0) {
    if (parse_line(&instruction, line, len) != AOC_OK) {
      aoc_fatalf("failed to parse line: %s\n", line);
    }
    printf("%s | %s: (%d, %d) (%d, %d)\n", line,
           str_command(instruction.instruction), instruction.left.x,
           instruction.left.y, instruction.right.x, instruction.right.y);
  }

  return 0;
}
