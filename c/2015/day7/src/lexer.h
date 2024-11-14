#ifndef LEXER_H
#define LEXER_H

#include <aoc.h>

#define LEXER_ERROR_SIZE 1000
#define BUFFER_MAX_SIZE 100

typedef enum {
  NUMBER,
  IDENTIFIER,
  ARROW,
  NOT,
  AND,
  OR,
  LSHIFT,
  RSHIFT,
} token_type_e;

typedef struct {
  token_type_e type;
  union {
    int number;
    char *string;
  } value;
} token_t;

typedef struct {
  char *input;
  size_t len;
  size_t line;
  size_t index;
  char *error;
} lexer_t;

/**
 * Initializes a new `lexer_t` context for use in the lexing process.
 */
lexer_t *lexer_init(void);

/**
 * Converts the string `line` with length `len` to a linked_list_t `tokens` of
 * tokens.
 *
 * `lexer` is the context used to store information about the current lexing
 * process and any errors that may have occured.
 */
int tokenize(linked_list_t *tokens, lexer_t *lexer, char *line, size_t len);

/**
 * Destroys a `lexer_t`.
 */
void lexer_destroy(lexer_t *lexer);

/**
 * Destroys a `token_t`.
 */
void token_free(void *vtoken);

#endif // LEXER_H
