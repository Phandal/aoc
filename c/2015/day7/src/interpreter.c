#include <aoc.h>
#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "interpreter.h"
#include "parser.h"

int wire_compare(void *vwire1, void *vwire2) {
  assert(vwire1 != NULL);
  assert(vwire2 != NULL);

  wire_t *wire1 = vwire1;
  wire_t *wire2 = vwire2;

  if (strlen(wire1->name) != strlen(wire2->name)) {
    return AOC_LL_NOT_EQUAL;
  }

  if (strncmp(wire1->name, wire2->name, strlen(wire1->name)) == 0) {
    return AOC_LL_EQUAL;
  }

  return AOC_LL_NOT_EQUAL;
}

void interpreter_errorf(interpreter_t *interpreter, char *fmt, ...) {
  assert(interpreter != NULL);
  if (interpreter->error != NULL) {
    return;
  }

  interpreter->error = (char *)malloc(sizeof(char) * INTERPRETER_ERROR_SIZE);
  if (interpreter->error == NULL) {
    // TODO: there must be a way that we can signify an error to the caller here
    return;
  }

  va_list(args);
  va_start(args, fmt);
  vsnprintf(interpreter->error, INTERPRETER_ERROR_SIZE, fmt, args);
  va_end(args);
}

interpreter_t *interpreter_init(linked_list_t *instructions) {
  interpreter_t *interpreter = (interpreter_t *)malloc(sizeof(interpreter_t));
  if (interpreter == NULL) {
    return NULL;
  }

  interpreter->instructions = instructions;
  interpreter->error = NULL;
  interpreter->current = NULL;
  interpreter->index = 0;

  return interpreter;
}

instruction_t *interpreter_advance(interpreter_t *interpreter) {
  assert(interpreter != NULL);

  if (interpreter->index >= interpreter->instructions->length) {
    return NULL;
  }

  if (interpreter->current == NULL && interpreter->index == 0) {
    interpreter->current = interpreter->instructions->node;
  } else {
    interpreter->current = interpreter->current->next;
  }
  interpreter->index += 1;

  if (interpreter->current != NULL &&
      ((instruction_t *)interpreter->current->data)->execution == EXECUTED) {
    interpreter_advance(interpreter);
  }

  return (instruction_t *)interpreter->current->data;
}

// NOTE: There is definitely a better way to do this with an associative array
int interpret(linked_list_t *wires, interpreter_t *interpreter) {
  assert(wires != NULL);
  assert(interpreter != NULL);
  instruction_t *instruction;
  wire_t *found_wire;
  unsigned int operand1;
  unsigned int operand2;
  int all_executed = 0;

  wire_t *wire = (wire_t *)malloc(sizeof(wire_t));

  while (!all_executed) {
    // Reset interpreter
    interpreter->current = NULL;
    interpreter->index = 0;

    all_executed = 1;
    while ((instruction = interpreter_advance(interpreter)) != NULL) {
      switch (instruction->input->type) {
      case ASSIGNMENT:
        switch (instruction->input->operation.assignment->operand->type) {
        case SIGNAL:
          wire->signal =
              instruction->input->operation.assignment->operand->value.signal;
          break;
        case WIRE:
          wire->name =
              instruction->input->operation.assignment->operand->value.wire;
          found_wire = linked_list_find(wires, wire, wire_compare);
          if (found_wire != NULL) {
            wire->signal = found_wire->signal;
          } else {
            all_executed = 0;
            continue;
            /*interpreter_errorf(*/
            /*    interpreter, "could not find wire %s for assignemnt",
             * wire->name);*/
            /*return AOC_ERR;*/
          }
          break;
        default:
          interpreter_errorf(interpreter, "unknown assigment error");
          return AOC_ERR;
          break;
        }
        break;
      case UNARY:
        switch (instruction->input->operation.unary->operand->type) {
        case WIRE:
          wire->name = instruction->input->operation.unary->operand->value.wire;
          found_wire = linked_list_find(wires, wire, wire_compare);
          if (found_wire != NULL) {
            wire->signal = ~(found_wire->signal);
          } else {
            all_executed = 0;
            continue;
            /*interpreter_errorf(interpreter, "could not find wire %s for
             * unary",*/
            /*                   wire->name);*/
            /*return AOC_ERR;*/
          }
          break;
        default:
          interpreter_errorf(interpreter, "unknown unary error");
          return AOC_ERR;
        }
        break;
      case BINARY:
        // Left
        switch (instruction->input->operation.binary->left->type) {
        case SIGNAL:
          operand1 = instruction->input->operation.binary->left->value.signal;
          break;
        case LITERAL:
          operand1 = instruction->input->operation.binary->left->value.literal;
          break;
        case WIRE:
          wire->name = instruction->input->operation.binary->left->value.wire;
          found_wire = linked_list_find(wires, wire, wire_compare);
          if (found_wire != NULL) {
            operand1 = found_wire->signal;
          } else {
            all_executed = 0;
            continue;
            /*interpreter_errorf(interpreter,*/
            /*                   "could not find wire %s for binary left",*/
            /*                   wire->name);*/
            /*return AOC_ERR;*/
          }
          break;
        }

        // Right
        switch (instruction->input->operation.binary->right->type) {
        case SIGNAL:
          operand2 = instruction->input->operation.binary->right->value.signal;
          break;
        case LITERAL:
          operand2 = instruction->input->operation.binary->right->value.literal;
          break;
        case WIRE:
          wire->name = instruction->input->operation.binary->right->value.wire;
          found_wire = linked_list_find(wires, wire, wire_compare);
          if (found_wire != NULL) {
            operand2 = found_wire->signal;
          } else {
            all_executed = 0;
            continue;
            /*interpreter_errorf(interpreter,*/
            /*                   "could not find wire %s for binary right",*/
            /*                   wire->name);*/
            /*return AOC_ERR;*/
          }
          break;
        }

        switch (instruction->input->operation.binary->operator_type) {
        case LSHIFT_O:
          wire->signal = operand1 << operand2;
          break;
        case RSHIFT_O:
          wire->signal = operand1 >> operand2;
          break;
        case AND_O:
          wire->signal = operand1 & operand2;
          break;
        case OR_O:
          wire->signal = operand1 | operand2;
          break;
        }
      }

      wire->name = instruction->output_wire;

      found_wire = linked_list_find(wires, wire, wire_compare);
      if (found_wire != NULL) {
        found_wire->signal = wire->signal;
      } else {
        linked_list_add(wires, wire, sizeof(wire_t));
      }

      instruction->execution = EXECUTED;
    }
  }

  free(wire);
  return AOC_OK;
}

void interpreter_destroy(interpreter_t *interpreter) {
  if (interpreter == NULL) {
    return;
  }

  if (interpreter->error) {
    free(interpreter->error);
  }
  free(interpreter);
}

void wire_free(void *vwire) {
  wire_t *wire = vwire;
  if (wire->name != NULL) {
    free(wire->name);
  }

  free(wire);
}
