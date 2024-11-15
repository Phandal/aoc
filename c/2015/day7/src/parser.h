#ifndef PARSER_H
#define PARSER_H

#define PARSER_ERROR_SIZE 1000

#include <aoc.h>

typedef enum {
  BINARY,
  UNARY,
  ASSIGNMENT,
} operation_type_e;

typedef enum {
  WIRE,
  SIGNAL,
  LITERAL,
} operand_type_e;

typedef enum {
  LSHIFT_O,
  RSHIFT_O,
  AND_O,
  OR_O,
} binary_operator_type_e;

typedef enum {
  NOT_O,
} unary_operator_type_e;

typedef struct {
  operand_type_e type;
  union {
    char *wire;
    int signal;
    int literal;
  };
} operand_t;

typedef struct {
  binary_operator_type_e operator_type;
  operand_t *left;
  operand_t *right;
} binary_operation_t;

typedef struct {
  unary_operator_type_e operator_type;
  operand_t *operand;
} unary_operation_t;

typedef struct {
  operand_t *operand;
} assignment_operation_t;

typedef struct {
  operation_type_e type;
  union {
    binary_operation_t *binary;
    unary_operation_t *unary;
    assignment_operation_t *assignment;
  } operation;
} operation_t;

typedef struct {
  operation_t *input;
  char *output_wire;
} instruction_t;

typedef struct {
  linked_list_t *tokens;
  node_t *current;
  size_t index;
  char *error;
} parser_t;

/**
 * Initializes a new parser_t with linked_list_t `tokens` as input.
 */
parser_t *parser_init(linked_list_t *tokens);

/**
 * Parses a list of tokens from the `parser` into a list of instruction_t
 * `instructions`
 */
int parse(linked_list_t *instructions, parser_t *parser);

/**
 * Destorys a parser_t object.
 * @note: It is the programmers responsibliity to free the internal `tokens`
 * linked_list_t.
 */
void parser_destroy(parser_t *parser);

#endif // PARSER_H
