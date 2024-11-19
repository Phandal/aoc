#include <aoc.h>
#include <stdio.h>

#include "interpreter.h"
#include "lexer.h"
#include "parser.h"

#define MAX_LINE 100

int main(void) {
  size_t len;
  char line[MAX_LINE] = {0};
  linked_list_t tokens = {0, NULL};
  linked_list_t instructions = {0, NULL};
  linked_list_t wires = {0, NULL};
  lexer_t *lexer;
  parser_t *parser;
  interpreter_t *interpreter;

  if ((lexer = lexer_init()) == NULL) {
    aoc_fatalf("failed to initialize lexer\n");
  }
  printf("lexer initialized...\n");

  if ((parser = parser_init(&tokens)) == NULL) {
    aoc_fatalf("failed to initialize parser\n");
  }
  printf("parser initialized...\n");

  if ((interpreter = interpreter_init(&instructions)) == NULL) {
    aoc_fatalf("failed to initialize interpreter\n");
  }
  printf("interpreter initialized...\n");

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

  if (interpret(&wires, interpreter) != AOC_OK) {
    aoc_fatalf("%s\n", interpreter->error);
  }
  printf("number of wires: %lu\n\n", wires.length);
  node_t *node;
  for (node = wires.node; node != NULL; node = node->next) {
    wire_t *wire = node->data;
    printf("name: %s | signal: %hu\n", wire->name, wire->signal);
  }

  linked_list_free(&tokens, token_free);
  /*printf("Freed tokens\n");*/
  linked_list_free(&instructions, instruction_free);
  /*printf("Freed instructions\n");*/
  linked_list_free(&wires, wire_free);
  /*printf("Freed wires\n");*/
  lexer_destroy(lexer);
  printf("Destroyed lexer\n");
  parser_destroy(parser);
  printf("Destroyed parser\n");
  interpreter_destroy(interpreter);
  printf("Destroyed interpreter\n");
  return 0;
}
