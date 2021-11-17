/**
 * Simplest stack library. Creates a simple stack and controls its
 * size and contents, and provides interoperability with other programs.
 */

/**
 * The library implements the functions of creating and destroying a
 * stack, relocating a stack, adding new elements and deleting them
 * 
 * The library was written within the course of I.R.Dedinsky, as the
 * third project [10.2021]. 
 */
#include <stdio.h>
#include <stdlib.h>
#include <cstdint>
#include <assert.h>
#include <string.h>

#include "stack.h"

StackError stackCtor (Stack *stack, unsigned int start_capacity)
{
  if (stack == nullptr) {
    printf ("fatal error func: %s(), check stack ptr\n", __func__);

    return StackError::structPtrErr;
  }

  stack->size = 0;
  stack->capacity = start_capacity;

  stack->data = (stack_t *)calloc (stack->capacity, sizeof (stack_t));

  if (stack->data == nullptr) {
      return StackError::structPtrErr;
  }

  return StackError::noErr;
}

//---------------------------------------------------------------------------------------

StackError stackPush (Stack *stack, stack_t new_elem)
{
  if (stack == nullptr) {
    printf ("fatal error func: %s(), check stack ptr\n", __func__);

    return StackError::structPtrErr;
  }

  if(stack->capacity < 0) {
    return StackError::badCap;
  }

  if(stack->size < 0) {
    return StackError::badSz;
  }

  if (stack->size >= stack->capacity) {
    stackResize (stack, stack->capacity * REALLOC_COEFF);
  }

  stack->data[stack->size] = new_elem;
  stack->size++;

  return StackError::noErr;
}

//---------------------------------------------------------------------------------------

StackError stackResize (Stack *stack, size_t increase_by)
{
    if (stack == nullptr) {
      printf ("fatal error in func: %s(), check stack ptr \n", __func__);

      return StackError::structPtrErr;
    }

    if (stack->capacity < 0) {
      return StackError::badCap;
    }

    if (stack->size < 0) {
      return StackError::badSz;
    }

    stack->capacity = (int)increase_by;
    stack_t *temporary_data = (stack_t *) realloc(stack->data, increase_by * sizeof(stack_t));

    if (stack->data == nullptr) {
        return StackError::dataErr;
    }

    stack->data = temporary_data;

    return StackError::noErr;
}

//---------------------------------------------------------------------------------------

StackError stackTop (Stack *stack, stack_t *top_element)
{
  if (stack == nullptr) {
    printf ("fatal error func: %s(), check stack ptr\n", __func__);

    return StackError::structPtrErr;
  }

  if(stack->capacity < 0) {
    return StackError::badCap;
  }

  if(stack->size < 0) {
    return StackError::badSz;
  }

  if (stack->data == nullptr) {
    return StackError::dataErr;
  }

  *top_element = stack->data[stack->size - 1];

  return StackError::noErr;
}

//---------------------------------------------------------------------------------------

StackError stackPop (Stack *stack)
{
  if (stack == nullptr) {
    return StackError::structPtrErr;

    printf ("fatal error: BAD STACK PTR, please check entered parametres of function: %s\n", __func__);
  }

  if(stack->capacity < 0) {
    return StackError::badCap;
  }

  if(stack->size < 0) {
    return StackError::badSz;
  }

  stack->size--;

  if (stack->size <= (stack->capacity / REALLOC_COEFF / REALLOC_COEFF) ) {
    if (stack->size != 0) {
      stackResize (stack, stack->capacity / REALLOC_COEFF);
    }
    if (stack->size < 0) {
      printf ("WRONG STACK SIZE");
    }
  }

  return StackError::noErr;
}

//---------------------------------------------------------------------------------------

StackError stackDtor(Stack *stack)
{
  if (stack == nullptr) {
    printf ("fatal error: BAD STACK PTR, please check entered parametres of function: %s\n", __func__);

    return StackError::structPtrErr;
  }

  if (stack->size < 0) {
    return StackError::badSz;
  }

  if (stack->capacity < 0) {
    return StackError::badCap;
  }

  for (int index = 0; index < stack->capacity - 1; index++) {
    stack->data[index] = 0;
  }

  stack->size     = 0;
  stack->capacity = 0;

  free (stack->data);

  return StackError::noErr;
}
