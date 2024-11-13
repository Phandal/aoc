#include <aoc.h>
#include <stdlib.h>
#include <string.h>

#include "lexer.h"

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
