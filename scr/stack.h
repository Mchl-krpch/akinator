#ifndef STACK_2_STACK_H
#define STACK_2_STACK_H

#include <cmath>
#include <cstdint>

#define REALLOC_COEFF 2

/**
 * @brief Special stack's type which used in stack
 */
typedef struct Node* stack_t;

enum class StackError: char {
  noErr = 0,
  structPtrErr,
  badCap,
  badSz,
  dataErr
};

/**
 * @brief it is a structure of the stack
 * 
 * @param capacity max number of elements in stack
 * @param size current number of elements
 * @param data content of the stack
 */
typedef  struct {
    int	 capacity;   // max size
    int		   size;   // cur number of element
    stack_t *data;   // stack path
}Stack;

/**
 * @brief create stack
 * 
 * @param new_stack stack example 
 * @param start_capacity start max number of elements in stack
 */
StackError stack_ctor (Stack *new_stack, unsigned int start_capacity);

/**
 * @brief changes capacity of stack
 * 
 * @param new_stack stack example 
 * @param increase_by in what capacity we change current capacity of stack
 */
StackError stack_resize (Stack *stack, size_t increase_by);

/**
 * @brief changes capacity of stack
 * 
 * @param new_stack stack example 
 * @param increase_by in what capacity we change current capacity of stack
 */
StackError stack_push (Stack *stack, stack_t value);

/**
 * @brief returns last element in stack
 * 
 * @param new_stack stack example 
 */
StackError stack_top (Stack *stack, stack_t *top_element);

/**
 * @brief delete last element
 * 
 * @param new_stack stack example 
 */
StackError stack_pop (Stack *stack);

/**
 * @brief delete stack example
 * 
 * @param new_stack stack example 
 */
StackError stack_dtor (Stack *stack);

#endif //STACK_2_STACK_H
