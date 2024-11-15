#include <aoc.h>
#include <stdio.h>

#include "lexer.h"
#include "parser.h"

#define MAX_LINE 100

int main(void) {
  size_t len;
  char line[MAX_LINE] = {0};
  linked_list_t tokens = {0, NULL};
  linked_list_t instructions = {0, NULL};
  lexer_t *lexer;
  parser_t *parser;

  if ((lexer = lexer_init()) == NULL) {
    aoc_fatalf("failed to initialize lexer\n");
  }
  printf("lexer initialized...\n");

  if ((parser = parser_init(&tokens)) == NULL) {
    aoc_fatalf("failed to initialize parser\n");
  }
  printf("parser initialized...\n");

  while ((len = aoc_get_line(line, MAX_LINE)) != 0) {
    printf("%s\n", line);
    if (tokenize(&tokens, lexer, line, len) != AOC_OK) {
      aoc_fatalf("%s\n", lexer->error);
    }
  }
  printf("number of tokens: %lu\n\n", tokens.length);

  if (parse(&instructions, parser) != AOC_OK) {
    aoc_fatalf("%s\n", parser->error);
  }
  printf("number of instructions: %lu\n\n", instructions.length);

  /*node_t *node;*/
  /*for (node = instructions.node; node != NULL; node = node->next) {*/
  /*  instruction_t *ins = node->data;*/
  /*  printf("type: %d | output: %s\n", ins->input->type, ins->output_wire);*/
  /*}*/
  /*putchar('\n');*/

  linked_list_free(&tokens, token_free);
  linked_list_free(&instructions, instruction_free);
  lexer_destroy(lexer);
  parser_destroy(parser);
  return 0;
}
