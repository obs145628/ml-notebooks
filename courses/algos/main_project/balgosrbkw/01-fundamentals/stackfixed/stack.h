#ifndef STACK_H_
#define STACK_H_

#include "lestd.h"

// Create a new stack with fixed capacity `cap`
int_t stack_new(int_t cap);

// Clear memory of stack object
void stack_free(int_t stack);

// Push `val` to the top of the stack
void stack_push(int_t stack, int_t val);

// Pop and return one item from the stack
int_t stack_pop(int_t stack);

// Returns the number of items currently on the stack
int_t stack_size(int_t stack);

#endif //! STACK_H_
