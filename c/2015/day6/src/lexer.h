#ifndef LEXER_H
#define LEXER_H

#include <aoc.h>

#define TOKEN_MAX 30

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

int tokenize(linked_list_t *tokens, char *line, size_t len);

#endif // LEXER_H
