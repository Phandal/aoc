#include <aoc.h>
#include <assert.h>
#include <stdarg.h>
#include <stdlib.h>

#include "lexer.h"
#include "parser.h"

parser_t *parser_init(linked_list_t *tokens) {
  parser_t *parser = (parser_t *)malloc(sizeof(parser_t));
  if (parser == NULL) {
    return NULL;
  }

  parser->tokens = tokens;
  parser->index = 0;
  parser->current = NULL;
  parser->error = NULL;

  return parser;
}

void parser_errorf(parser_t *parser, char *fmt, ...) {
  assert(parser != NULL);
  if (parser->error != NULL) {
    return;
  }

  parser->error = (char *)malloc(sizeof(char) * PARSER_ERROR_SIZE);
  if (parser->error == NULL) {
    // TODO: there must be a way that we can signify an error to the caller here
    return;
  }

  va_list(args);
  va_start(args, fmt);
  vsnprintf(parser->error, PARSER_ERROR_SIZE, fmt, args);
  va_end(args);
}

token_t *parser_advance(parser_t *parser) {
  assert(parser != NULL);

  if (parser->index >= parser->tokens->length) {
    return NULL;
  }

  if (parser->current == NULL && parser->index == 0) {
    parser->current = parser->tokens->node;
  } else {
    parser->current = parser->current->next;
  }
  parser->index += 1;

  return parser->current->data;
}

int parse(linked_list_t *instructions, parser_t *parser) {
  assert(parser != NULL);

  token_t *token;
  instruction_t instruction;
  operation_t *operation = NULL;
  assignment_operation_t *assignment_operation = NULL;
  operand_t *operand = NULL;

  for (token = parser_advance(parser); token != NULL;
       token = parser_advance(parser)) {
    switch (token->type) {
    case NUMBER:

      operand = (operand_t *)malloc(sizeof(operand_t));
      operand->signal = token->value.number;

      assignment_operation =
          (assignment_operation_t *)malloc(sizeof(assignment_operation_t));
      assignment_operation->operand = operand;

      operation = (operation_t *)malloc(sizeof(operation_t));
      operation->type = ASSIGNMENT;
      operation->operation.assignment = assignment_operation;

      instruction.input = operation;
      token = parser_advance(parser);

      if (token == NULL || token->type != ARROW) {
        parser_errorf(parser, "expected arrow\n");
      }

      token = parser_advance(parser);
      if (token == NULL || token->type != IDENTIFIER) {
        parser_errorf(parser, "expected identifier\n");
      }

      instruction.output_wire = token->value.string;

      linked_list_add(instructions, &instruction, sizeof(instruction));
    }
  }

  return AOC_OK;
}

void parser_destroy(parser_t *parser) { free(parser); }
