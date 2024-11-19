#include <aoc.h>
#include <assert.h>
#include <ctype.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include "lexer.h"

int lexer_advance(lexer_t *lexer) {
  assert(lexer != NULL);

  if (lexer->index >= lexer->len) {
    return EOF;
  }

  return lexer->input[lexer->index++];
}

int lexer_peek(lexer_t *lexer) {
  assert(lexer != NULL);

  if (lexer->index >= lexer->len) {
    return EOF;
  }

  return lexer->input[lexer->index];
}

int lexer_reverse(lexer_t *lexer) {
  assert(lexer != NULL);

  if (lexer_peek(lexer) != EOF && --(lexer->index) < 0) {
    lexer->index = 0;
  }

  return lexer->index;
}

void lexer_errorf(lexer_t *lexer, char *fmt, ...) {
  assert(lexer != NULL);
  if (lexer->error != NULL) {
    return;
  }

  lexer->error = (char *)malloc(sizeof(char) * LEXER_ERROR_SIZE);
  if (lexer->error == NULL) {
    // TODO: there must be a way that we can signify an error to the caller here
    return;
  }

  va_list args;
  va_start(args, fmt);
  vsnprintf(lexer->error, LEXER_ERROR_SIZE, fmt, args);
  va_end(args);
}

lexer_t *lexer_init(void) {
  lexer_t *lexer = (lexer_t *)malloc(sizeof(lexer_t));
  if (lexer == NULL) {
    return NULL;
  }

  lexer->len = 0;
  lexer->index = 0;
  lexer->line = 0;
  lexer->input = NULL;
  lexer->error = NULL;

  return lexer;
}

int tokenize(linked_list_t *tokens, lexer_t *lexer, char *line, size_t len) {
  assert(lexer != NULL);
  assert(tokens != NULL);

  token_t token = {0};
  int c;
  unsigned int number;
  char buffer[BUFFER_MAX_SIZE];
  int buffer_index = 0;

  if (lexer->input == NULL || strlen(lexer->input) < len) {
    lexer->input = realloc(lexer->input, sizeof(char) * len);
    if (lexer->input == NULL) {
      lexer_errorf(lexer, "out of memory");
      return AOC_ERR;
    }
  }

  if (strncpy(lexer->input, line, len) == NULL) {
    return AOC_ERR;
  }

  lexer->len = len;
  lexer->index = 0;
  lexer->line += 1;

  while ((c = lexer_advance(lexer)) != EOF) {
    switch (c) {
    case '-':
      c = lexer_advance(lexer);
      if (c != '>') {
        lexer_errorf(lexer, "expected '>' got '%c': (%lu, %lu)", c,
                     lexer->index, lexer->line);
        return AOC_ERR;
      }
      token.type = ARROW;
      linked_list_add(tokens, &token, sizeof(token));
      printf("Added ARROW token\n");
      break;
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
      number = 0;
      while (c != EOF && isdigit(c)) {
        number = (number * 10) + (c - '0');
        c = lexer_advance(lexer);
      }
      if (!isspace(c)) {
        lexer_errorf(lexer, "expected whitespace or EOF got '%c': (%lu, %lu)",
                     c, lexer->index, lexer->line);
        return AOC_ERR;
      }

      token.type = NUMBER;
      token.value.number = number;
      linked_list_add(tokens, &token, sizeof(token));
      printf("Added NUMBER token: %d\n", number);
      lexer_reverse(lexer);
      break;
    case ' ':
    case '\t':
    case '\n':
      continue;
    default:
      if (isalpha(c)) {
        buffer_index = 0;
        while (c != EOF && !isspace(c)) {
          if (buffer_index >= BUFFER_MAX_SIZE - 1) {
            lexer_errorf(lexer, "identifier buffer out of memery: (%lu, %lu)",
                         lexer->index, lexer->line);
            return AOC_ERR;
          }
          buffer[buffer_index++] = c;
          c = lexer_advance(lexer);
        }
        buffer[buffer_index] = '\0';

        if (strncmp("LSHIFT", buffer, buffer_index) == 0) {
          printf("Added LSHIFT token\n");
          token.type = LSHIFT;
        } else if (strncmp("RSHIFT", buffer, buffer_index) == 0) {
          printf("Added RSHIFT token\n");
          token.type = RSHIFT;
        } else if (strncmp("NOT", buffer, buffer_index) == 0) {
          printf("Added NOT token\n");
          token.type = NOT;
        } else if (strncmp("AND", buffer, buffer_index) == 0) {
          printf("Added AND token\n");
          token.type = AND;
        } else if (strncmp("OR", buffer, buffer_index) == 0) {
          printf("Added OR token\n");
          token.type = OR;
        } else {
          token.type = IDENTIFIER;
          token.value.string = malloc(sizeof(char) * buffer_index);
          strncpy(token.value.string, buffer, buffer_index);
          printf("Added IDENTIFIER token: %s\n", buffer);
        }

        linked_list_add(tokens, &token, sizeof(token));
        lexer_reverse(lexer);

      } else {
        lexer_errorf(lexer, "unexpected character '%c': (%lu, %lu)", c,
                     lexer->index, lexer->line);
        return AOC_ERR;
      }
    }
  }

  return AOC_OK;
}

void lexer_destroy(lexer_t *lexer) {
  if (lexer == NULL) {
    return;
  }

  if (lexer->input != NULL) {
    free(lexer->input);
  }

  if (lexer->error != NULL) {
    free(lexer->error);
  }

  free(lexer);
}

void token_free(void *vtoken) {
  token_t *token = vtoken;
  if (token->type == IDENTIFIER) {
    free(token->value.string);
  }
  free(token);
}
