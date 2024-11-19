#include <aoc.h>
#include <assert.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

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
  binary_operation_t *binary_operation = NULL;
  unary_operation_t *unary_operation = NULL;
  assignment_operation_t *assignment_operation = NULL;
  operand_t *operand = NULL;

  for (token = parser_advance(parser); token != NULL;
       token = parser_advance(parser)) {
    switch (token->type) {
    case NUMBER:

      operand = (operand_t *)malloc(sizeof(operand_t));
      operand->type = SIGNAL;
      operand->value.signal = token->value.number;

      token = parser_advance(parser);
      if (token == NULL) {
        parser_errorf(parser, "unexpected end of input\n");
        return AOC_ERR;
      }

      if (token->type == ARROW) {
        assignment_operation =
            (assignment_operation_t *)malloc(sizeof(assignment_operation_t));
        assignment_operation->operand = operand;

        operation = (operation_t *)malloc(sizeof(operation_t));
        operation->type = ASSIGNMENT;
        operation->operation.assignment = assignment_operation;
      } else {
        binary_operation =
            (binary_operation_t *)malloc(sizeof(binary_operation_t));
        binary_operation->left = operand;

        switch (token->type) {
        case LSHIFT:
          binary_operation->operator_type = LSHIFT_O;
          break;
        case RSHIFT:
          binary_operation->operator_type = RSHIFT_O;
          break;
        case AND:
          binary_operation->operator_type = AND_O;
          break;
        case OR:
          binary_operation->operator_type = OR_O;
          break;
        default:
          parser_errorf(parser, "exected arror or binary operation token\n");
          return AOC_ERR;
        }

        token = parser_advance(parser);
        if (token == NULL) {
          parser_errorf(parser, "unexpected end of input\n");
          return AOC_ERR;
        }

        operand = (operand_t *)malloc(sizeof(operand_t));
        switch (token->type) {
        case IDENTIFIER:
          operand->type = WIRE;
          operand->value.wire =
              malloc(sizeof(char) * strlen(token->value.string));
          strncpy(operand->value.wire, token->value.string,
                  strlen(token->value.string));
          break;
        case NUMBER:
          operand->type = LITERAL;
          operand->value.literal = token->value.number;
          break;
        default:
          parser_errorf(parser, "expected identifier or number token\n");
          return AOC_ERR;
        }

        binary_operation->right = operand;
        operation = (operation_t *)malloc(sizeof(operation_t));
        operation->type = BINARY;
        operation->operation.binary = binary_operation;

        token = parser_advance(parser);
        if (token == NULL || token->type != ARROW) {
          parser_errorf(parser, "expected arrow\n");
          return AOC_ERR;
        }
      }

      instruction.input = operation;

      token = parser_advance(parser);
      if (token == NULL || token->type != IDENTIFIER) {
        parser_errorf(parser, "expected identifier (number)\n");
        return AOC_ERR;
      }

      instruction.output_wire =
          malloc(sizeof(char) * strlen(token->value.string));
      strncpy(instruction.output_wire, token->value.string,
              strlen(token->value.string));

      linked_list_add(instructions, &instruction, sizeof(instruction));
      if (operation->type == ASSIGNMENT) {
        printf("Added Assignment Operation: %d | %s\n",
               assignment_operation->operand->value.signal,
               instruction.output_wire);
      } else {
        if (binary_operation->right->type == WIRE) {
          printf("Added Binary Operation: %d %d %s | %s\n",
                 binary_operation->left->value.literal,
                 binary_operation->operator_type,
                 binary_operation->right->value.wire, instruction.output_wire);
        } else {
          printf("Added Binary Operation: %d %d %d | %s\n",
                 binary_operation->left->value.literal,
                 binary_operation->operator_type,
                 binary_operation->right->value.literal,
                 instruction.output_wire);
        }
      }
      break;
    case NOT:
      token = parser_advance(parser);
      if (token == NULL || token->type != IDENTIFIER) {
        parser_errorf(parser, "expected identifier\n");
        return AOC_ERR;
      }
      operand = (operand_t *)malloc(sizeof(operand_t));
      operand->type = WIRE;
      operand->value.wire = malloc(sizeof(char) * strlen(token->value.string));
      strncpy(operand->value.wire, token->value.string,
              strlen(token->value.string));

      unary_operation = (unary_operation_t *)malloc(sizeof(unary_operation_t));
      unary_operation->operator_type = NOT_O;
      unary_operation->operand = operand;

      operation = (operation_t *)malloc(sizeof(operation_t));
      operation->type = UNARY;
      operation->operation.unary = unary_operation;

      instruction.input = operation;

      token = parser_advance(parser);
      if (token == NULL || token->type != ARROW) {
        parser_errorf(parser, "expected arrow\n");
        return AOC_ERR;
      }

      token = parser_advance(parser);
      if (token == NULL || token->type != IDENTIFIER) {
        parser_errorf(parser, "expected identifier (not)\n");
        return AOC_ERR;
      }

      instruction.output_wire =
          malloc(sizeof(char) * strlen(token->value.string));
      strncpy(instruction.output_wire, token->value.string,
              strlen(token->value.string));

      linked_list_add(instructions, &instruction, sizeof(instruction));
      printf("Added Unary Operation: %s %d | %s\n",
             unary_operation->operand->value.wire,
             unary_operation->operand->type, instruction.output_wire);
      break;
    case IDENTIFIER:
      operand = (operand_t *)malloc(sizeof(operand_t));
      operand->type = WIRE;
      operand->value.wire = malloc(sizeof(char) * strlen(token->value.string));
      strncpy(operand->value.wire, token->value.string,
              strlen(token->value.string));

      token = parser_advance(parser);
      if (token == NULL) {
        parser_errorf(parser, "unexpected end of input\n");
        return AOC_ERR;
      }

      if (token->type == ARROW) {
        assignment_operation =
            (assignment_operation_t *)malloc(sizeof(assignment_operation_t));
        assignment_operation->operand = operand;

        operation = (operation_t *)malloc(sizeof(operation_t));
        operation->type = ASSIGNMENT;
        operation->operation.assignment = assignment_operation;
      } else {

        binary_operation =
            (binary_operation_t *)malloc(sizeof(binary_operation_t));
        binary_operation->left = operand;

        switch (token->type) {
        case LSHIFT:
          binary_operation->operator_type = LSHIFT_O;
          break;
        case RSHIFT:
          binary_operation->operator_type = RSHIFT_O;
          break;
        case AND:
          binary_operation->operator_type = AND_O;
          break;
        case OR:
          binary_operation->operator_type = OR_O;
          break;
        default:
          parser_errorf(parser, "expected binary operation token\n");
          return AOC_ERR;
        }

        token = parser_advance(parser);
        if (token == NULL) {
          parser_errorf(parser, "unexpected end of input\n");
          return AOC_ERR;
        }

        operand = (operand_t *)malloc(sizeof(operand_t));
        switch (token->type) {
        case IDENTIFIER:
          operand->type = WIRE;
          operand->value.wire =
              malloc(sizeof(char) * strlen(token->value.string));
          strncpy(operand->value.wire, token->value.string,
                  strlen(token->value.string));
          break;
        case NUMBER:
          operand->type = LITERAL;
          operand->value.literal = token->value.number;
          break;
        default:
          parser_errorf(parser, "expected identifier or number token\n");
          return AOC_ERR;
        }

        binary_operation->right = operand;
        operation = (operation_t *)malloc(sizeof(operation_t));
        operation->type = BINARY;
        operation->operation.binary = binary_operation;

        token = parser_advance(parser);
        if (token == NULL || token->type != ARROW) {
          parser_errorf(parser, "expected arrow\n");
          return AOC_ERR;
        }
      }

      instruction.input = operation;

      token = parser_advance(parser);
      if (token == NULL || token->type != IDENTIFIER) {
        parser_errorf(parser, "expected identifier (identifier)\n");
        return AOC_ERR;
      }

      instruction.output_wire =
          malloc(sizeof(char) * strlen(token->value.string));
      strncpy(instruction.output_wire, token->value.string,
              strlen(token->value.string));

      linked_list_add(instructions, &instruction, sizeof(instruction));
      if (operation->type == ASSIGNMENT) {
        printf("Added Assignment Operation: %d | %s\n",
               assignment_operation->operand->value.signal,
               instruction.output_wire);
      } else {
        if (binary_operation->right->type == WIRE) {
          printf("Added Binary Operation: %s %d %s | %s\n",
                 binary_operation->left->value.wire,
                 binary_operation->operator_type,
                 binary_operation->right->value.wire, instruction.output_wire);
        } else {
          printf("Added Binary Operation: %s %d %d | %s\n",
                 binary_operation->left->value.wire,
                 binary_operation->operator_type,
                 binary_operation->right->value.literal,
                 instruction.output_wire);
        }
      }
      break;
    case ARROW:
      parser_errorf(parser, "unexpected arrow token\n");
      return AOC_ERR;
    case LSHIFT:
      parser_errorf(parser, "unexpected lshift token\n");
      return AOC_ERR;
    case RSHIFT:
      parser_errorf(parser, "unexpected rshift token\n");
      return AOC_ERR;
    case AND:
      parser_errorf(parser, "unexpected and token\n");
      return AOC_ERR;
    case OR:
      parser_errorf(parser, "unexpected or token\n");
      return AOC_ERR;
    }
  }

  return AOC_OK;
}

void parser_destroy(parser_t *parser) {
  if (parser == NULL) {
    return;
  }

  if (parser->error != NULL) {
    free(parser->error);
  }
  free(parser);
}

void instruction_free(void *vinstruction) {
  instruction_t *instruction = vinstruction;
  binary_operation_t *binary_operation = NULL;
  unary_operation_t *unary_operaton = NULL;
  assignment_operation_t *assignment_operation = NULL;

  switch (instruction->input->type) {
  case ASSIGNMENT:
    assignment_operation = instruction->input->operation.assignment;
    if (assignment_operation->operand->type == WIRE) {
      free(assignment_operation->operand->value.wire);
    }
    free(assignment_operation->operand);
    free(assignment_operation);
    /*printf("Freed assignment operation\n");*/
    break;
  case UNARY:
    unary_operaton = instruction->input->operation.unary;
    free(unary_operaton->operand->value.wire);
    free(unary_operaton->operand);
    free(unary_operaton);
    /*printf("Freed unary operation\n");*/
    break;
  case BINARY:
    binary_operation = instruction->input->operation.binary;
    if (binary_operation->left->type == WIRE) {
      free(binary_operation->left->value.wire);
    }
    free(binary_operation->left);
    if (binary_operation->right->type == WIRE) {
      free(binary_operation->right->value.wire);
    }
    free(binary_operation->right);
    free(binary_operation);
    /*printf("Freed binary operation\n");*/
    break;
  default:
    printf("Did not free unknown operation type: %d\n",
           instruction->input->type);
  }

  free(instruction->input);
  free(instruction->output_wire);
  free(instruction);
}
