#include <aoc.h>
#include <stdio.h>

#include "lexer.h"
#include "parser.h"

#define LINE_MAX 100

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
