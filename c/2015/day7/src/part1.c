#include <aoc.h>
#include <stdio.h>

#include "lexer.h"

#define MAX_LINE 100

int main(void) {
  size_t len;
  char line[MAX_LINE] = {0};
  linked_list_t tokens = {0, NULL};
  lexer_t *lexer = lexer_init();
  if (lexer == NULL) {
    aoc_fatalf("failed to initialize lexer\n");
  }
  printf("lexer initialized...\n");

  while ((len = aoc_get_line(line, MAX_LINE)) != 0) {
    printf("%s\n", line);
    if (tokenize(&tokens, lexer, line, len) != AOC_OK) {
      aoc_fatalf("%s\n", lexer->error);
    }
    printf("number of tokens: %lu\n", tokens.length);
    break;
  }

  /*if (parse(&ast, &tokens) != AOC_OK) {*/
  /*  aoc_fatalf("%s\n", parser->error);*/
  /*}*/

  linked_list_free(&tokens, token_free);
  lexer_destroy(lexer);
  return 0;
}
