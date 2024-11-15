#include <aoc.h>
#include <assert.h>
#include <stdlib.h>

#include "lexer.h"
#include "parser.h"

parser_t *parser_init(linked_list_t *tokens) {
  parser_t *parser = (parser_t *)malloc(sizeof(parser_t));
  if (parser == NULL) {
    return NULL;
  }

  (parser->tokens) = tokens;

  return parser;
}

int parse(linked_list_t *instructions, parser_t *parser) {
  assert(parser != NULL);

  node_t *node;
  token_t *token;
  for (node = parser->tokens->node; node != NULL; node = node->next) {
    token = (token_t *)node->data;
    printf("Token type: %d\n", token->type);
  }

  return AOC_OK;
}

void parser_destroy(parser_t *parser) { free(parser); }
